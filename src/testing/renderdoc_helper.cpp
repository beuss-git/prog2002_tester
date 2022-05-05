#include "renderdoc_helper.hpp"
#include <fmt/core.h>
#include <fmt/format.h>
#include <magic_enum.hpp>
#ifdef _WIN32
#include <Windows.h>
#endif

rdcstr conv(const std::string &s) {
	return { s.c_str(), s.size() };
}

std::string conv(const rdcstr &s) {
	return { s.begin(), s.end() };
}

RenderDocHelper::RenderDocHelper(std::string rdc_path) : m_rdc_path(std::move(rdc_path)) { }

RenderDocHelper::~RenderDocHelper() {
	if (m_capture_file) {
		m_capture_file->Shutdown();
	}
	if (m_controller) {
		m_controller->Shutdown();
	}
	RENDERDOC_ShutdownReplay();
}

bool RenderDocHelper::open_capture() {

	auto global_env = GlobalEnvironment{};
	global_env.enumerateGPUs = false;
#ifdef _WIN32
	// Disable error reporting on windows
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#endif

	RENDERDOC_InitialiseReplay(global_env, {});

	m_capture_file = RENDERDOC_OpenCaptureFile();
	if (!m_capture_file) {
		fmt::print("Failed to open capture file");
		return false;
	}

    ReplayStatus st = m_capture_file->OpenFile(conv(m_rdc_path), "rdc",nullptr);
	if (st != ReplayStatus::Succeeded) {
		fmt::print("Failed to open file: {}", m_rdc_path);
		return false;
	}

	if (m_capture_file->LocalReplaySupport() != ReplaySupport::Supported) {
		fmt::print("Local replay not supported\n");
		return false;
	}

	const auto& [status, controller] = m_capture_file->OpenCapture(ReplayOptions{}, nullptr);
	if (status != ReplayStatus::Succeeded) {
		fmt::print("Failed to open capture: {}\n", static_cast<int>(status));
		return false;
	}

	m_controller = controller;

	m_structured_data = &m_capture_file->GetStructuredData();

	return true;
}

void RenderDocHelper::dump_actions() const {
	for (const auto& action : m_controller->GetRootActions()) {
		fmt::print("{}\n", action.GetName(*m_structured_data).c_str());
	}
}

size_t RenderDocHelper::drawcalls_count() const {
	return m_controller->GetRootActions().size();
}

/**
 * \brief Crudely checks the vertex buffer of an action byte for byte for equality
 * \param action action that uses the vertex buffer
 * \param vertices reference vertex data
 * \return true if the match
 */
bool RenderDocHelper::check_vertex_data(ActionDescription action, std::vector<float> vertices) const {
	m_controller->SetFrameEvent(action.eventId, true);

	const auto postvs_data = m_controller->GetPostVSData(0, 0, MeshDataStage::VSOut);
	const auto buffer_data = m_controller->GetBufferData(postvs_data.vertexResourceId, postvs_data.vertexByteOffset, 0);

	// Could also check if equal if we want to be more strict
	if (buffer_data.size() < vertices.size() * sizeof(float)) {
		return false;
	}

	const auto* data_begin = reinterpret_cast<byte*>(vertices.data());
	const auto* data_end = data_begin + vertices.size() * sizeof(float);

	if (!std::equal(data_begin, data_end, buffer_data.data())) {
		return false;
	}

	return true;
}

ActionDescription RenderDocHelper::find_action_by_name(std::string_view name) const {
	for (const auto& action : m_controller->GetRootActions()) {
		if (action.GetName(*m_structured_data).c_str() == name) {
			return action;
		}
	}
	return ActionDescription{};
}

ActionDescription RenderDocHelper::find_action_by_event_id(uint32_t event_id) const {
	for (const auto& action : m_controller->GetRootActions()) {
		if (action.eventId == event_id) {
			return action;
		}
	}
	return ActionDescription{};
}

void RenderDocHelper::print_textures() {
	const auto& textures = m_controller->GetTextures();
	for (auto& texture : textures) {
		fmt::print("[{}] {}\n", *(uint64_t*)&texture.resourceId, magic_enum::enum_name(texture.type));
	}
	fmt::print("\n");
}
void RenderDocHelper::print_buffers() {
	const auto& buffers = m_controller->GetBuffers();
	for (auto& buffer : buffers) {
		fmt::print("[{}] {}, len: {}\n", *(uint64_t*)&buffer.resourceId, magic_enum::enum_name(buffer.creationFlags), buffer.length);
	}



	const auto& resources = m_controller->GetResources();
	for (auto& resource : resources) {
		if (resource.type == ResourceType::Buffer) {
			print_buffer(resource, {});
		}
	}

	fmt::print("\n");
}

void RenderDocHelper::print_resources() {
	const auto& resources = m_controller->GetResources();
	for (auto& resource : resources) {
		fmt::print("[{}] {}: {}\n", *(uint64_t*)&resource.resourceId, magic_enum::enum_name(resource.type), resource.name.c_str());
	}
	fmt::print("\n");
}

void RenderDocHelper::print_buffer(ResourceDescription buf, BufferDescription buffer) const {
	const auto chunks = buf.initialisationChunks;
	for (auto chunk : chunks) {
		const auto obj = m_structured_data->chunks[chunk];
		fmt::print("Name: {}\n", 
			obj->name.c_str());

		fmt::print("Type name: {}\n", 
			obj->type.name.c_str());

		fmt::print("Type data: {}\n", 
			obj->data.str.c_str());
		
		//auto data_children = obj->data.c
		int index = 0;
		for (;;) {
			auto child = obj->GetChild(index++);
			if (!child) {
				break;
			}
			//if (child->name.c_str() == std::string("bytesize")) {
			//	int bp = 0;
			//	child->data.basic.u
			//}
			fmt::print("\tchild: {}\n", child->name.c_str());
			fmt::print("\t\ttype: {}\n", child->type.name.c_str());
			fmt::print("\t\tdata: {}\n", child->data.str.c_str());
			fmt::print("\t\tdata val: {}\n", sdobject_tostring(child));
		}

		auto usages = m_controller->GetUsage(buf.resourceId);
		// This holds all the usages (for example this could be used in drawarrays, and this gets us that event id
		if (!usages.empty()) {
			fmt::print("Usages:\n");
			for (auto usage : usages) {
				auto action = find_action_by_event_id(usage.eventId);
				auto action_name = action.GetName(*m_structured_data);
				fmt::print("[] {}\n", usage.eventId, action_name.c_str());
			}
		}
	}
}

bool RenderDocHelper::contains_vertex_array() {
	// Mutiple different types of state objects, so have do do it by type and partial name
	return contains_resource(ResourceType::StateObject, "Vertex Array");
}

bool RenderDocHelper::contains_vertex_buffer() {
	return contains_resource(ResourceType::Buffer);
}

bool RenderDocHelper::contains_shader() {
	return contains_resource(ResourceType::Shader);
}

bool RenderDocHelper::has_transferred_buffer_data() {
	const auto vertex_buffers = get_vertex_buffers();

	for (auto& vertex_buffer : vertex_buffers) {
		// TODO: check if order matters if directly indexing children (if need be)

		// Chunks containing calls to glGenBuffers, glBindBuffer, glBufferData etc
		const auto chunk_ids = vertex_buffer.initialisationChunks;
		for (auto chunk_id : chunk_ids) {
			const auto& chunk = m_structured_data->chunks[chunk_id];
			if (chunk->name == "glBufferData") {
				return true;
			}
		}
	}
	return false;
}

bool RenderDocHelper::contains_resource(ResourceType type, std::string_view name) const {
	const auto& resources = m_controller->GetResources();
	for (auto& resource : resources) {
		if (resource.type == type && (name.empty() || resource.name.contains(rdcstr(name.data())))) {
			return true;
		}
	}
	return false;
}

std::vector<ResourceDescription> RenderDocHelper::get_vertex_buffers() const {
	// NOTE: Not sure if this will work when other buffer types are introduced
	return get_resources_by_type(ResourceType::Buffer);
}

std::vector<ResourceDescription> RenderDocHelper::get_shaders() const {
	return get_resources_by_type(ResourceType::Shader);
}

std::vector<ResourceDescription> RenderDocHelper::get_resources_by_type(ResourceType type) const {
	std::vector<ResourceDescription> resources{};
	const auto& all_resources = m_controller->GetResources();
	for (auto& resource : all_resources) {
		if (resource.type == type) {
			resources.push_back(resource);
		}
	}
	return resources;
}

std::string RenderDocHelper::sdobject_tostring(SDObject* obj) const {

	if (obj->IsUInt()) {
		return fmt::format("{}", obj->AsUInt64());
	}
	if (obj->IsInt()) {
		return fmt::format("{}", obj->AsInt64());
	}
	if (obj->IsFloat()) {
		return fmt::format("{}", obj->AsFloat());
	}
	if (obj->IsResource()) {
		auto resource_id = obj->AsResourceId();
		return fmt::format("{}", *(uint64_t*)&resource_id);
	}
	if (obj->type.basetype == SDBasic::Boolean) {
		return fmt::format("{}", obj->data.basic.b);
	}
	return "UNK";
}

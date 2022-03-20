#include "renderdoc_helper.hpp"
#include <fmt/core.h>
#include <Windows.h>

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
	SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	RENDERDOC_InitialiseReplay(global_env,{});

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

void RenderDocHelper::dump_actions() {
	for (auto action : m_controller->GetRootActions()) {
		fmt::print("{}\n", action.GetName(*m_structured_data).c_str());
	}
}

size_t RenderDocHelper::drawcalls_count() const {
	return m_controller->GetRootActions().size();
}

/**
 * \brief Crudely checks a vertex buffer byte for byte for equality
 * \param action action that uses the vertex buffer
 * \param vertices reference vertex data
 * \return true if the match
 */
bool RenderDocHelper::check_vertex_data(ActionDescription action, std::vector<float> vertices) {
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
	for (auto action : m_controller->GetRootActions()) {
		if (action.GetName(*m_structured_data).c_str() == name) {
			return action;
		}
	}
	return ActionDescription{};
}

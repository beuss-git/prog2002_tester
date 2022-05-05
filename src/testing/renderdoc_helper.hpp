#pragma once
#include <string>
#include <renderdoc/api/replay/renderdoc_replay.h>

class RenderDocHelper {
public:
	RenderDocHelper(std::string rdc_path);
	~RenderDocHelper();
	bool open_capture();

	void dump_actions() const;
	size_t drawcalls_count() const;
	bool check_vertex_data(ActionDescription action, std::vector<float> vertices) const;
	ActionDescription find_action_by_name(std::string_view name) const;
	ActionDescription find_action_by_event_id(uint32_t event_id) const;

	void print_textures();
	void print_buffers();
	void print_resources();

	void print_buffer(ResourceDescription desc, BufferDescription buffer) const;

	bool contains_vertex_array();
	bool contains_vertex_buffer();
	bool contains_shader();

	bool has_transferred_buffer_data();
private:
	bool contains_resource(ResourceType type, std::string_view name = "") const;
	std::vector<ResourceDescription> get_vertex_buffers() const;
	std::vector<ResourceDescription> get_shaders() const;
	std::vector<ResourceDescription> get_resources_by_type(ResourceType type) const;
	std::string sdobject_tostring(SDObject* obj) const;

private:
	std::string m_rdc_path{};
	ICaptureFile* m_capture_file{ nullptr };
	IReplayController* m_controller{ nullptr };
	const SDFile* m_structured_data{ nullptr };
};
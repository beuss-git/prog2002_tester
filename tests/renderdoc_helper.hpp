#pragma once
#include <string>
#include "../external/renderdoc/api/replay/renderdoc_replay.h"


class RenderDocHelper {
public:
	RenderDocHelper(std::string rdc_path);
	~RenderDocHelper();
	bool open_capture();

	void dump_actions();
	size_t drawcalls_count() const;
	bool check_vertex_data(ActionDescription action, std::vector<float> vertices);
	ActionDescription find_action_by_name(std::string_view name) const;

private:
	std::string m_rdc_path{};
	ICaptureFile* m_capture_file{ nullptr };
	IReplayController* m_controller{ nullptr };
	const SDFile* m_structured_data{ nullptr };
};
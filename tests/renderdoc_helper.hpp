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
private:
	ActionDescription find_action_by_name(std::string_view name) const {
		for (auto action : m_controller->GetRootActions()) {
			if (action.GetName(*m_structured_data).c_str() == name) {
				return action;
			}
		}
		return ActionDescription{};
	}

private:
	std::string m_rdc_path{};
	ICaptureFile* m_capture_file{ nullptr };
	IReplayController* m_controller{ nullptr };
	const SDFile* m_structured_data{ nullptr };
};
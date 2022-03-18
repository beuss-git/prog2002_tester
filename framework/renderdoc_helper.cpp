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

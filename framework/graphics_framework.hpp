#pragma once
#include <string>
#include <iostream>

#include "renderdoc/api/app/renderdoc_app.h"

namespace argparse {
	class ArgumentParser;
}

class GraphicsFramework {
public:
	GraphicsFramework(int argc, char* argv[]);
	virtual ~GraphicsFramework() = default;

	int main();
protected:
	virtual bool setup() = 0;
	virtual bool run() = 0;
	virtual bool cleanup() = 0;
private:
	void set_marker(const std::string& name);
	bool should_capture();
	bool running();
	void init_renderdoc();

	void start_capture();
	void end_capture();

	bool m_testing = false;
	int m_max_frames = 0;

	int m_current_frame = 0;

	bool m_debug_output = false;

	RENDERDOC_API_1_1_2* m_rdoc_api = nullptr;
};
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
	virtual ~GraphicsFramework() {
		// might just want to call from run function
		//cleanup();
	}


	void set_marker(const std::string& name);
	int main() {

		if (!setup()) {
			return EXIT_FAILURE;
		}

		bool run_state = true;
		while (running()) {
			if (should_capture()) {
				start_capture();
			}
			run_state = run();

			if (should_capture()) {
				end_capture();
			}

			if (!run_state) {
				break;
			}
		}
		cleanup();

		return run_state ? EXIT_SUCCESS : EXIT_FAILURE;
	}
protected:
	virtual bool setup() = 0;
	virtual bool run() = 0;
	virtual bool cleanup() = 0;
private:
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
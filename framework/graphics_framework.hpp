#pragma once
#include <string>

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

	bool running();

	void set_marker(const std::string& name);
private:
	bool m_testing = false;
	int m_max_frames = 0;

	int m_current_frame = 0;

	bool m_debug_output = false;
};
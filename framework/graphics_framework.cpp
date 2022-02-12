#include "graphics_framework.hpp"
#include <argparse/argparse.hpp>

#include "glad/glad.h"

GraphicsFramework::GraphicsFramework(int argc, char* argv[]) {
	argparse::ArgumentParser parser("test_framework");
	parser.add_argument("--frame").help("frame to capture on").scan<'i', int>().default_value(0);
	parser.add_argument("--framecount").help("amount of frames to capture").scan<'i', int>().default_value(0);
	parser.add_argument("--test").help("run tests").default_value(false).implicit_value(true);
	parser.add_argument("--verbose", "-v").default_value(false).implicit_value(true);
	parser.add_argument("--log");
	//program.add_argument("framecount");
	//program.add_argument("max-frames");
	//program.add_argument("validate");
	//program.add_argument("log");

	try {
		parser.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cerr << err.what() << std::endl;
		std::cerr << parser;
		std::exit(1);
	}
	m_debug_output = parser.get<bool>("verbose");


	// Used in case we want to capture multiple frames
	const auto frame_count = parser.get<int>("framecount");

	m_max_frames = parser.get<int>("frame") + frame_count;

	m_testing = parser.get<bool>("test");

	if (m_debug_output) {
		std::cout << "Testing: " << std::boolalpha << m_testing << "\n";
		if (m_testing) {
			std::cout << "Max frames: " << m_max_frames << "\n";
		}
	}
}


bool GraphicsFramework::running() {
	// Always running if we are not performing tests
	if (!m_testing) {
		return true;
	}

	m_current_frame++;

	if (m_max_frames > 0 && m_current_frame >= m_max_frames) {
		return false;
	}

	return true;
}

void GraphicsFramework::set_marker(const std::string& name) {
	if (glDebugMessageInsert)
		glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0,
			GL_DEBUG_SEVERITY_LOW, -1, name.c_str());

}

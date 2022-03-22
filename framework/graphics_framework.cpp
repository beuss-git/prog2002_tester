#include "graphics_framework.hpp"
#include <argparse/argparse.hpp>
#include <glad/glad.h>
#include <renderdoc/api/app/renderdoc_app.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

GraphicsFramework::GraphicsFramework(int argc, char* argv[]) {
	argparse::ArgumentParser parser("test_framework");
	parser.add_argument("--frame").help("frame to capture on").scan<'i', int>().default_value(0);
	parser.add_argument("--framecount").help("amount of frames to capture").scan<'i', int>().default_value(0);
	parser.add_argument("--test").help("run tests").default_value(false).implicit_value(true);
	parser.add_argument("--verbose", "-v").default_value(false).implicit_value(true);
	parser.add_argument("--log");

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

	init_renderdoc();
}


// TODO: support multiple frames here
bool GraphicsFramework::should_capture() {
	if (!m_rdoc_api || !m_testing) {
		return false;
	}

	return m_current_frame == m_max_frames - 1;
	//return m_current_frame == m_max_frames;
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

int GraphicsFramework::main() {

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

void GraphicsFramework::init_renderdoc() {

	// TODO: Handle initialization error if we are testing
#ifdef _WIN32
	// At init, on windows
	if(HMODULE mod = GetModuleHandleA("renderdoc.dll"))
	{
		std::cout << "Found renderdoc lib!\n";
		pRENDERDOC_GetAPI RENDERDOC_GetAPI =
			(pRENDERDOC_GetAPI)GetProcAddress(mod, "RENDERDOC_GetAPI");
		if (!RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&m_rdoc_api)) {
			std::cout << "Failed to initialize renderdoc api!\n";
		}
	}
#else

	// At init, on linux/android.
	// For android replace librenderdoc.so with libVkLayer_GLES_RenderDoc.so
	if(void *mod = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD))
	{
		std::cout << "Found renderdoc lib!\n";
		pRENDERDOC_GetAPI RENDERDOC_GetAPI = (pRENDERDOC_GetAPI)dlsym(mod, "RENDERDOC_GetAPI");
		if (!RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_2, (void **)&m_rdoc_api)) {
			std::cout << "Failed to initialize renderdoc api!\n";
		}
	}
#endif
}

void GraphicsFramework::start_capture() {
	std::cout << "Starting capture!\n";
	m_rdoc_api->StartFrameCapture(nullptr, nullptr);
}

void GraphicsFramework::end_capture() {
	std::cout << "Ending capture!\n";
	if (!m_rdoc_api->EndFrameCapture(nullptr, nullptr)) {
		std::cout << "Failed to capture!\n";
	}
}

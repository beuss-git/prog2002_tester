#include "student.hpp"
#include "../framework/renderdoc.hpp"
#include "../framework/renderdoc_helper.hpp"

// Add this to test driver (before)
REPLAY_PROGRAM_MARKER()

int main(int argc, char* argv[]) {

#if 1
	//RenderDoc::Dump renderdoc(R"(G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.xml)");
	//if (!renderdoc.parse()) {
	//	return -1;
	//}
	//renderdoc.dump_header();
	//renderdoc.dump_chunks();
	//const auto action = renderdoc.find_action("glDrawArrays");
	//if (action.empty()) {
	//	return -1;
	//}
	// Load reference image to compare

	RenderDocHelper helper(R"(G:\repos\test_framework_tester\tmp\TestFramework\capture_frame5.rdc)");
	helper.open_capture();
	helper.dump_actions();
	
	return 0;
#else
	Lab01 lab01(argc, argv);
	//example.run();

	return lab01.main();
#endif
	
}

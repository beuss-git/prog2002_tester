#include <argparse/argparse.hpp>
#include <fmt/core.h>
#include "renderdoc_helper.hpp"


REPLAY_PROGRAM_MARKER();

bool parse_arguments(argparse::ArgumentParser& parser, int argc, char* argv[]) {
	parser.add_argument("--drawcalls_count").help("target drawcalls count").scan<'i', int>().default_value(-1);
	parser.add_argument("--capture_file").help("the capture file to analyze").default_value(std::string(""));

	try {
		parser.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		std::cout << err.what() << std::endl;
		std::cout << parser << "\n";
		return false;
	}
	return true;
}

int renderdoc_test(int argc, char* argv[]) {
	argparse::ArgumentParser parser("test_framework");
	if (!parse_arguments(parser, argc, argv))
		return -1;

	const auto expected_drawcalls_count = parser.get<int>("drawcalls_count");
	const auto capture_file = parser.get<std::string>("capture_file");
	if (capture_file.empty()) {
		fmt::print("No capture file!\n");
		return -1;
	}

	RenderDocHelper helper(capture_file);
	if (!helper.open_capture()) {
		fmt::print("Failed to open the capture file, exiting!\n");
		return -1;
	}

	if (expected_drawcalls_count != -1) {
		if (const auto drawcalls_count = helper.drawcalls_count();
			static_cast<size_t>(expected_drawcalls_count) != drawcalls_count) {
			fmt::print("found {} drawcalls, expected {}", drawcalls_count, expected_drawcalls_count);
			return -1;
		}
	}

	auto drawcalls_action = helper.find_action_by_name("glDrawArrays()");
	if (!helper.check_vertex_data(drawcalls_action, {
		-0.5f, -0.5f, 0.f, 1.f,
		0.5f, -0.5f, 0.f, 1.f,
		0.0f, 0.5f, 0.f, 1.f
		})) {
		fmt::print("vertex data does not match the expected vertex data");
		return -1;
	}

	return 0;
}

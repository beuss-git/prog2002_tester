#include "testing/renderdoc_helper.hpp"
#include <fmt/core.h>
#include <fmt/format.h>


REPLAY_PROGRAM_MARKER()
int main() {
	//std::string capture_file = R"(G:\captures\capture.rdc)";
	std::string capture_file = R"(G:/repos/PROG2002/template/out/build/x64-debug/bin/LabXX_capture.rdc)";
	//std::string capture_file = R"(G:\captures\capture_no_vertex_array.rdc)";
	//std::string capture_file = R"(G:\captures\capture_no_drawarrays_call.rdc)";
	//std::string capture_file = R"(G:\captures\capture_no_bufferdata_call.rdc)";
	//std::string capture_file = R"(G:\captures\capture_no_genbuffers_call.rdc)";
	//std::string capture_file = R"(G:\captures\capture_no_genbuffers_call_with_buffderdata_call.rdc)";

	RenderDocHelper helper(capture_file);
	if (!helper.open_capture()) {
		fmt::print("Failed to open the capture file, exiting!\n");
		return -1;
	}

	//const auto action = helper.find_action_by_name("glDrawArrays(3)");
	//
	//fmt::print("{}", action.actionId);

	//fmt::print("Resources:\n");
	//helper.print_resources();
	//fmt::print("Buffers:\n");
	//helper.print_buffers();
	//fmt::print("Textures:\n");
	//helper.print_textures();

	fmt::print("Does contain vertex array: {}\n", helper.contains_vertex_array());
	fmt::print("Does contain shader: {}\n", helper.contains_shader());
	fmt::print("Does contain vertex buffer: {}\n", helper.contains_vertex_buffer());
	fmt::print("Does contain drawcall: {}\n", helper.contains_drawcall());
	fmt::print("Has transferred buffer data: {}\n", helper.has_transferred_buffer_data());

	helper.print_actions();
}
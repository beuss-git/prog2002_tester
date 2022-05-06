#include "tester.hpp"

bool Tester::perform_tests() {
	// Open the capture file
	if (!m_helper.open_capture()) {
		fmt::print("Failed to open the capture file, exiting!\n");
		return false;
	}

	if (!drawcalls_count_test()) {
		return false;
	}
	if (!vao_count_test()) {
		return false;
	}
	if (!vbo_count_test()) {
		return false;
	}
	if (!vertex_buffer_transfer_count_test()) {
		return false;
	}

	return true;
}

bool Tester::drawcalls_count_test() {
	const auto expected_drawcalls_count = m_args.get<int>("drawcalls_count");
	if (expected_drawcalls_count != DISABLED) {
		if (const auto drawcalls_count = m_helper.drawcalls_count();
			static_cast<size_t>(expected_drawcalls_count) != drawcalls_count) {
			fmt::print(fg(fmt::color::red), "found {} drawcalls, expected {}\n", drawcalls_count, expected_drawcalls_count);
			return false;
		}
	}
	return true;
}

bool Tester::vao_count_test() {
	const auto vao_count = m_args.get<int>("vao_count");
	if (vao_count != DISABLED) {
		if (const auto count = m_helper.resource_count(ResourceType::StateObject, "Vertex Array"); count != vao_count) {
			fmt::print(fg(fmt::color::red), "found {} vertex arrays, expected {}\n", count, vao_count);
			return false;
		}
	}
	return true;
}


bool Tester::vbo_count_test() {
	const auto vbo_count = m_args.get<int>("vbo_count");
	if (vbo_count != DISABLED) {
		if (const auto count = m_helper.resource_count(ResourceType::Buffer); count != vbo_count) {
			fmt::print(fg(fmt::color::red), "found {} vertex buffer objects, expected {}\n", count, vbo_count);
			return false;
		}
	}
	return true;
}

bool Tester::vertex_buffer_transfer_count_test() {
	const auto transfer_count = m_args.get<int>("vbo_transfer_count");
	if (transfer_count != DISABLED) {
		if (const auto count = m_helper.vertex_buffer_transfer_count(); count != transfer_count) {
			fmt::print(fg(fmt::color::red), "found {} vertex buffer transfers, expected {}\n", count, transfer_count);
			return false;
		}
	}
	return true;
}

bool Tester::shader_count_test() {
	const auto shader_count = m_args.get<int>("shader_count");
	if (shader_count != DISABLED) {
		if (const auto count = m_helper.vertex_buffer_transfer_count(); count != shader_count) {
			fmt::print(fg(fmt::color::red), "found {} shaders, expected {}\n", count, shader_count);
			return false;
		}
	}
	return true;
}

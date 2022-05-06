#pragma once
#include <fmt/core.h>
#include <fmt/color.h>
#include "renderdoc_helper.hpp"
#include "argparse/argparse.hpp"

class Tester {
public:
	Tester(const std::string& capture_file, const argparse::ArgumentParser& parser) : m_helper(capture_file), m_args(parser) { }

	bool perform_tests();

private:
	bool drawcalls_count_test();
	bool vao_count_test();
	bool vbo_count_test();
	bool vertex_buffer_transfer_count_test();
	bool shader_count_test();

private:
	RenderDocHelper m_helper;
	argparse::ArgumentParser m_args;

	static constexpr int DISABLED = -1;
};

#include "file_manager.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

std::string utils::load_file(const std::string_view path) {
	std::ifstream f(path.data());
	if (!f.is_open()) {
		std::cerr << "Failed to load file: " << path.data() <<  "\n";
		return "";
	}

	return { (std::istreambuf_iterator<char>(f)),
		std::istreambuf_iterator<char>() };
}

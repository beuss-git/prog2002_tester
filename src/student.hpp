#pragma once
#include "../framework/graphics_framework.hpp"

class Lab01 final : GraphicsFramework {
public:
	Lab01(int argc, char* argv[]) : GraphicsFramework(argc, argv){}
	//void setup() override;
	//void run() override;
	//void cleanup() override;
	int simplerun();
};

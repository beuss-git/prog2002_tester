#pragma once
#include "../framework/graphics_framework.hpp"

class ExampleGraphics final : GraphicsFramework {
public:
	ExampleGraphics(int argc, char* argv[]) : GraphicsFramework(argc, argv){}
	//void setup() override;
	//void run() override;
	//void cleanup() override;
	int simplerun();
};

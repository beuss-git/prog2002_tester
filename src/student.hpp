#pragma once
#include "../framework/graphics_framework.hpp"

class Lab01 final : public GraphicsFramework {
public:
	Lab01(int argc, char* argv[]) : GraphicsFramework(argc, argv){}
	bool setup() override;
	bool run() override;
	bool cleanup() override;
};

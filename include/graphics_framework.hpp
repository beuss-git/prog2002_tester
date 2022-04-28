#pragma once
#include <string>
#include <iostream>

struct RENDERDOC_API_1_5_0;
typedef RENDERDOC_API_1_5_0 RENDERDOC_API_1_1_2;

class GraphicsFramework {
public:
	GraphicsFramework(int argc, char* argv[]);
	virtual ~GraphicsFramework() = default;

	int main();
protected:
	virtual bool setup() = 0;
	virtual bool run() = 0;
	virtual bool cleanup() = 0;
private:
	bool should_capture();
	bool running();
	void init_renderdoc();

	void start_capture();
	void end_capture();

	bool m_testing = false;
	int m_max_frames = 0;

	int m_current_frame = 0;

	bool m_debug_output = false;

	RENDERDOC_API_1_1_2* m_rdoc_api = nullptr;
};

#define CIMGUI_USE_GLFW
#define CIMGUI_USE_OPENGL3

#include "gui.h"

#include "GLFW/glfw3.h"
#include <cimgui.h>
// #include <cimgui_impl.h>

ImGuiContext* ctx;
ImGuiIO*	  io;

void cimgui_backend_init(void* window);
void cimgui_backend_shutdown();
void cimgui_backend_render(ImDrawData* draw_data);
float cimgui_get_content_scale();

void gui_init(GLFWwindow* mainWindow) {
	ctx = igCreateContext(nullptr);

	ImGuiIO* ioptr = igGetIO_ContextPtr(ctx);
	ioptr->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	cimgui_backend_init(mainWindow);

	float main_scale = cimgui_get_content_scale();

	ImGuiStyle* style = igGetStyle();
	ImGuiStyle_ScaleAllSizes(style, main_scale);


	igStyleColorsDark(NULL);

	igNewFrame();

	// igStyleColorsDark(NULL);
}

void gui_terminate() {
	cimgui_backend_shutdown();
}

void gui_update() {
	// static float f		 = 0.0f;
	// static int	 counter = 0;
	//
	// igBegin("Hello, world!", NULL, 0);
	// igText("This is some useful text");
	// // igCheckbox("Demo window", &showDemoWindow);
	// // igCheckbox("Another window", &showAnotherWindow);
	//
	// igSliderFloat("Float", &f, 0.0f, 1.0f, "%.3f", 0);
	// // igColorEdit3("clear color", (float*) &{0.2f, 0.2f, 0.2f}, 0);
	//
	// ImVec2 buttonSize;
	// buttonSize.x = 0;
	// buttonSize.y = 0;
	// if (igButton("Button", buttonSize))
	// 	counter++;
	// igSameLine(0.0f, -1.0f);
	// igText("counter = %d", counter);
	//
	// igText(
	// 	"Application average %.3f ms/frame (%.1f FPS)",
	// 	1000.0f / igGetIO_ContextPtr(ctx)->Framerate, igGetIO_ContextPtr(ctx)->Framerate
	// );
	// igEnd();
}

void gui_render() {
	igRender();
	cimgui_backend_render(igGetDrawData());
}


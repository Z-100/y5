#include "cimgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

extern "C" {

	void cimgui_backend_init(GLFWwindow* window) {
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void cimgui_backend_shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void cimgui_backend_render(ImDrawData* draw_data) {
		ImGui_ImplOpenGL3_RenderDrawData(draw_data);
	}

	float cimgui_get_content_scale() {
		return ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
	}

}

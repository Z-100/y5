#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

/**
 * See include/imgui_backends_bridge.h
 */
extern "C" {

// ========================
// = GLFW backend bridges =
// ========================

float cimGui_ImplGlfw_GetContentScaleForMonitor(GLFWmonitor* glfw_wmonitor) {
	return ImGui_ImplGlfw_GetContentScaleForMonitor(glfw_wmonitor);
}

bool cimgui_ImplGlfw_InitForOpenGL(GLFWwindow* window, bool install_callbacks) {
	return ImGui_ImplGlfw_InitForOpenGL(window, install_callbacks);
}

void cimgui_ImplGlfw_NewFrame() {
	ImGui_ImplGlfw_NewFrame();
}

void cimgui_ImplGlfw_Shutdown() {
	ImGui_ImplGlfw_Shutdown();
}

void cimgui_ImplGlfw_WindowFocusCallback(GLFWwindow* window, int focused) {
	ImGui_ImplGlfw_WindowFocusCallback(window, focused);
}

void cimgui_ImplGlfw_CursorPosCallback(GLFWwindow* window, double x, double y) {
	ImGui_ImplGlfw_CursorPosCallback(window, x, y);
}

// ==========================
// = OpenGL backend bridges =
// ==========================

bool cimgui_ImplOpenGL3_Init(const char* glsl_version) {
	return ImGui_ImplOpenGL3_Init(glsl_version);
}

void cimgui_ImplOpenGL3_NewFrame() {
	ImGui_ImplOpenGL3_NewFrame();
}

void cimgui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data) {
	ImGui_ImplOpenGL3_RenderDrawData(draw_data);
}

void cimgui_ImplOpenGL3_Shutdown() {
	ImGui_ImplOpenGL3_Shutdown();
}
}

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// =================
// = Struct clones =
// =================
struct GLFWwindow;
struct GLFWmonitor;
struct ImDrawData;

// ========================
// = GLFW backend bridges =
// ========================
float cimGui_ImplGlfw_GetContentScaleForMonitor(struct GLFWmonitor* glfw_wmonitor);
bool  cimgui_ImplGlfw_InitForOpenGL(struct GLFWwindow* window, bool install_callbacks);
void  cimgui_ImplGlfw_NewFrame(void);
void  cimgui_ImplGlfw_Shutdown(void);
void  cimgui_ImplGlfw_WindowFocusCallback(struct GLFWwindow* window, int focused);
void  cimgui_ImplGlfw_CursorPosCallback(struct GLFWwindow* window, double x, double y);

// ==========================
// = OpenGL backend bridges =
// ==========================
bool cimgui_ImplOpenGL3_Init(const char* glsl_version);
void cimgui_ImplOpenGL3_NewFrame();
void cimgui_ImplOpenGL3_RenderDrawData(struct ImDrawData* draw_data);
void cimgui_ImplOpenGL3_Shutdown();

#ifdef __cplusplus
}
#endif

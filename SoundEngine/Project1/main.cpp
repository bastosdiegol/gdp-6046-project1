#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "FModManager.h"

GLFWwindow* window;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods) {
}

int main(int argc, char* argv[]) {
	// Initialize the FModManager Class
	FModManager fmod_manager(FMOD_INIT_NORMAL);

	// Creates Channel Groups
	fmod_manager.createChannelGroup("master");
	fmod_manager.createChannelGroup("music");
	fmod_manager.createChannelGroup("fx");

	// Creates Parent Child relation between channels
	fmod_manager.setChannelGroupParent("master", "music");
	fmod_manager.setChannelGroupParent("master", "fx");

	// Sets initial volume for master channel to 20%
	// WARNING: DO NOT PASS VOLUME VALUE ABOVE 1.0f
	fmod_manager.setChannelGroupVolume("master", 0.3f);

	// Load all sounds from the XML File
	fmod_manager.loadSoundsFromFile();

	// Plays a BGM Sound
	fmod_manager.playSound("Moonlight Sonata", "master");

	//initialize glfw/glad
	glfwInit();
	window = glfwCreateWindow(800, 600, "Sound Engine - Project #1", nullptr, nullptr);

	if (!window) {
		return 1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return 2;
	}

	glfwSetKeyCallback(window, key_callback);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	//platform / render bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460")) {
		return 3;
	}

	//imgui style (dark mode for the win)
	ImGui::StyleColorsDark();

	//game loop
	while (!glfwWindowShouldClose(window)) {
		//poll for user events
		glfwPollEvents();

		//clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//present to the user
		glfwSwapBuffers(window);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
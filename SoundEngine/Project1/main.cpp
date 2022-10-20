#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/IconsFontaudio.h"
#include "FModManager.h"
#include "SoundUI.h"
#include "TicTacToeGame.h"
#include "PlyFileReader.h"

TicTacToeGame* ttt;
GLFWwindow* window;
FModManager* fmod_manager;
bool lastMoveResult = true;

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods) {
	if (action == GLFW_PRESS) {
		if (!ttt->m_isGameOver) {
			switch (key) {
			case GLFW_KEY_Q:
				lastMoveResult = ttt->makeAMove('q');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position Q. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_W:
				lastMoveResult = ttt->makeAMove('w');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position W. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_E:
				lastMoveResult = ttt->makeAMove('e');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position E. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_A:
				lastMoveResult = ttt->makeAMove('a');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position A. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_S:
				lastMoveResult = ttt->makeAMove('s');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position S. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_D:
				lastMoveResult = ttt->makeAMove('d');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position D. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_Z:
				lastMoveResult = ttt->makeAMove('z');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position Z. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_X:
				lastMoveResult = ttt->makeAMove('x');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position X. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			case GLFW_KEY_C:
				lastMoveResult = ttt->makeAMove('c');
				if (!lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position C. Try again!" << std::endl;
				else if (!ttt->m_isGameOver)
					ttt->nextTurn();
				break;
			}
		}
		if(key == GLFW_KEY_N)
			ttt->newGame();
	}
}

int main(int argc, char* argv[]) {
	// Initialize the FModManager Class
	fmod_manager = new FModManager(FMOD_INIT_NORMAL);
	ttt = new TicTacToeGame(fmod_manager);

	// Creates Channel Groups
	fmod_manager->createChannelGroup("ch0 master");
	fmod_manager->createChannelGroup("ch1 music");
	fmod_manager->createChannelGroup("ch2 fx");

	// Creates Parent Child relation between channels
	fmod_manager->setChannelGroupParent("ch0 master", "ch1 music");
	fmod_manager->setChannelGroupParent("ch0 master", "ch2 fx");

	// Sets initial volume for master channel to 20%
	// WARNING: DO NOT PASS VOLUME VALUE ABOVE 1.0f
	fmod_manager->setChannelGroupVolume("ch0 master", 1.0f);
	fmod_manager->setChannelGroupVolume("ch1 music", 0.2f);
	fmod_manager->setChannelGroupVolume("ch2 fx", 0.5f);

	// Load all sounds from the XML File
	fmod_manager->loadSoundsFromFile();

	// Load all DSP
	fmod_manager->loadDSPs();

	//initialize glfw/glad
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(1922, 1030, "Sound Engine - Project #1", nullptr, nullptr);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return 2;
	}
	glfwSwapInterval(1);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//initialize imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Sets the Default font for ImGui
	io.Fonts->AddFontDefault();

	// Merge in icons from Font Audio
	static const ImWchar icons_ranges[] = { ICON_MIN_FAD, ICON_MAX_16_FAD, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAD, 16.0f, &icons_config, icons_ranges);

	//platform / render bindings
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460")) {
		return 3;
	}

	//imgui style (dark mode for the win)
	ImGui::StyleColorsDark();

	//create sound ui
	SoundUI sound_ui(fmod_manager);

	// Loading Ply Models From the File
	PlyFileReader* pfr;
	std::vector<PlyFileReader*> v_PlyModels;
	pfr = new PlyFileReader("./assets/models/tictactoeBoard.ply");
	pfr->loadMeshFromFile();
	v_PlyModels.push_back(pfr);
	pfr = new PlyFileReader("./assets/models/tictactoeX.ply");
	pfr->loadMeshFromFile();
	v_PlyModels.push_back(pfr);
	pfr = new PlyFileReader("./assets/models/tictactoeO.ply");
	pfr->loadMeshFromFile();
	v_PlyModels.push_back(pfr);

	// Starts new TicTacToe Game
	ttt->newGame();

	//game loop
	while (!glfwWindowShouldClose(window)) {
		//poll for user events
		glfwPollEvents();

		//clear the back buffer
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		if(sound_ui.is_my_tool_active)
			sound_ui.render();

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
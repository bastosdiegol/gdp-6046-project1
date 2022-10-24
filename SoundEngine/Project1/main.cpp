#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/IconsFontaudio.h"
#include "FModManager.h"
#include "SoundUI.h"
#include "TicTacToeGame.h"
#include "PlyFileReader.h"
#include "MeshObject.h"
#include "Localization.h"

TicTacToeGame*	g_tttGame;
GLFWwindow*		g_window;
FModManager*	g_fmod_manager;
Localization*	g_localization;
bool			g_lastMoveResult = true;
std::vector<MeshObject> g_vMeshObjects;

//// Vertex Struct that we are going to send to GPU to be drawn
//struct sVertex_XYZ_RGB {
//	float x, y, z;	// 0 floats from the start
//	float r, g, b;  // 2 floats from the start
//					// Each vertex is 5 floats in size
//};

// Vexter Shader
static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";

// Fragment Shader
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

// Global Variable to the Camera Eye
glm::vec3 g_cameraEye = glm::vec3(0.0, -60.0, 1.0f);
// Global Variable to the Camera Target
glm::vec3 g_cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

#define behindCamera glm::vec3(  0.0f,-80.0f,  1.0f)
#define posQ		 glm::vec3( -8.0f,  0.0f,  8.0f)
#define posW		 glm::vec3(  0.0f,  0.0f,  8.0f)
#define posE		 glm::vec3(  8.0f,  0.0f,  8.0f)
#define posA		 glm::vec3( -8.0f,  0.0f,  0.0f)
#define posS		 glm::vec3(  0.0f,  0.0f,  0.0f)
#define posD		 glm::vec3(  8.0f,  0.0f,  0.0f)
#define posZ		 glm::vec3( -8.0f,  0.0f, -8.0f)
#define posX		 glm::vec3(  0.0f,  0.0f, -8.0f)
#define posC		 glm::vec3(  8.0f,  0.0f, -8.0f)

void newGameSetMeshes() {
	for (int i = 1; i < g_vMeshObjects.size(); i++) {
		g_vMeshObjects[i].m_position = behindCamera;
	}
}

void loadGameSetMeshes() {
	int countX = 1;		// Index of X pieces existing in the game. Starts at 1 - its the first model
	int countO = 2;		// Index of O pieces existing in the game. Starts at 2 - its the first model
	int posIndex = 0;	// Global Index for us to keep a track of which slot are we checking now
	// Iterate through the game board
	for (int iLine = 0; iLine < 3; iLine++) {
		for (int iCol = 0; iCol < 3; iCol++) {
			// Checks if this piece is X or Y
			if (g_tttGame->m_board[iLine][iCol] == 'X') {
				// Uses the posIndex to know where to place the Mesh Object
				switch (posIndex) {
				case 0: g_vMeshObjects[countX].m_position = posQ; break;
				case 1: g_vMeshObjects[countX].m_position = posW; break;
				case 2: g_vMeshObjects[countX].m_position = posE; break;
				case 3: g_vMeshObjects[countX].m_position = posA; break;
				case 4: g_vMeshObjects[countX].m_position = posS; break;
				case 5: g_vMeshObjects[countX].m_position = posD; break;
				case 6: g_vMeshObjects[countX].m_position = posZ; break;
				case 7: g_vMeshObjects[countX].m_position = posX; break;
				case 8: g_vMeshObjects[countX].m_position = posC; break;
				}
				countX += 2;
			} else if (g_tttGame->m_board[iLine][iCol] == 'O') {
				switch (posIndex) {
				case 0: g_vMeshObjects[countO].m_position = posQ; break;
				case 1: g_vMeshObjects[countO].m_position = posW; break;
				case 2: g_vMeshObjects[countO].m_position = posE; break;
				case 3: g_vMeshObjects[countO].m_position = posA; break;
				case 4: g_vMeshObjects[countO].m_position = posS; break;
				case 5: g_vMeshObjects[countO].m_position = posD; break;
				case 6: g_vMeshObjects[countO].m_position = posZ; break;
				case 7: g_vMeshObjects[countO].m_position = posX; break;
				case 8: g_vMeshObjects[countO].m_position = posC; break;
				}
				countO += 2;
			}
			// Increments the posIndex to let us know that will be checking the next slot of the board
			posIndex++;
		}
	}
}

void key_callback(GLFWwindow* window, const int key, int scancode, const int action, const int mods) {
	// Move the camera
	// AWSD   AD - left and right
	//        WS - forward and back
	const float CAMERA_MOVE_SPEED = 0.5f;
	if (key == GLFW_KEY_LEFT)     // Left
	{
		::g_cameraEye.x -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_RIGHT)     // Right
	{
		::g_cameraEye.x += CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_PAGE_UP)     // Forward
	{
		::g_cameraEye.z += CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_PAGE_DOWN)     // Backwards
	{
		::g_cameraEye.z -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_UP)     // Down
	{
		::g_cameraEye.y -= CAMERA_MOVE_SPEED;
	}
	if (key == GLFW_KEY_DOWN)     // Up
	{
		::g_cameraEye.y += CAMERA_MOVE_SPEED;
	}

	if (action == GLFW_PRESS) {
		if (!g_tttGame->m_isGameOver) {
			switch (key) {
			case GLFW_KEY_Q:
				g_lastMoveResult = g_tttGame->makeAMove('q');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position Q. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posQ;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_W:
				g_lastMoveResult = g_tttGame->makeAMove('w');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position W. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posW;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_E:
				g_lastMoveResult = g_tttGame->makeAMove('e');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position E. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posE;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_A:
				g_lastMoveResult = g_tttGame->makeAMove('a');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position A. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posA;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_S:
				g_lastMoveResult = g_tttGame->makeAMove('s');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position S. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posS;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_D:
				g_lastMoveResult = g_tttGame->makeAMove('d');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position D. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posD;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_Z:
				g_lastMoveResult = g_tttGame->makeAMove('z');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position Z. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posZ;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_X:
				g_lastMoveResult = g_tttGame->makeAMove('x');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position X. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posX;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_C:
				g_lastMoveResult = g_tttGame->makeAMove('c');
				if (!g_lastMoveResult)
					std::cout << "Your move attempt was invalid. There's already a piece on the position C. Try again!" << std::endl;
				else if (!g_tttGame->m_isGameOver) {
					g_vMeshObjects[g_tttGame->m_currentRound-1].m_position = posC;
					g_tttGame->nextTurn();
				}
				break;
			case GLFW_KEY_1:
				g_localization->setLanguage(g_localization->m_vLanguagesAvailable[0]);
				g_fmod_manager->playSound("New Lang", "ch2 fx");
				break;
			case GLFW_KEY_2:
				g_localization->setLanguage(g_localization->m_vLanguagesAvailable[1]);
				g_fmod_manager->playSound("New Lang", "ch2 fx");
				break;
			case GLFW_KEY_3:
				g_localization->setLanguage(g_localization->m_vLanguagesAvailable[2]);
				g_fmod_manager->playSound("New Lang", "ch2 fx");
				break;
			case GLFW_KEY_4:
				g_localization->setLanguage(g_localization->m_vLanguagesAvailable[3]);
				g_fmod_manager->playSound("New Lang", "ch2 fx");
				break;
			}
		}
		if (key == GLFW_KEY_N) {
			g_tttGame->newGame();
			newGameSetMeshes();
		}		
		if (key == GLFW_KEY_K)
			if(mods == GLFW_MOD_CONTROL)
				g_tttGame->saveGame();
		if (key == GLFW_KEY_L)
			if (mods == GLFW_MOD_CONTROL) {
				g_tttGame->loadGame();
				newGameSetMeshes();
				loadGameSetMeshes();
			}
	}
}

int main(int argc, char* argv[]) {
	// Initialize the Localization
	g_localization = new Localization();
	// Initialize the FModManager Class
	g_fmod_manager = new FModManager(FMOD_INIT_NORMAL);
	// Initialize the TicTacToe Game
	g_tttGame = new TicTacToeGame(g_fmod_manager, g_localization);

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	// Creates Channel Groups
	g_fmod_manager->createChannelGroup("ch0 master");
	g_fmod_manager->createChannelGroup("ch1 music");
	g_fmod_manager->createChannelGroup("ch2 fx");

	// Creates Parent Child relation between channels
	g_fmod_manager->setChannelGroupParent("ch0 master", "ch1 music");
	g_fmod_manager->setChannelGroupParent("ch0 master", "ch2 fx");

	// Sets initial volume for master channel to 20%
	// WARNING: DO NOT PASS VOLUME VALUE ABOVE 1.0f
	g_fmod_manager->setChannelGroupVolume("ch0 master", 0.7f);
	g_fmod_manager->setChannelGroupVolume("ch1 music", 0.15f);
	g_fmod_manager->setChannelGroupVolume("ch2 fx", 0.7f);

	// Load all sounds from the XML File
	g_fmod_manager->loadSoundsFromFile();

	// Load all DSP
	g_fmod_manager->loadDSPs();

	// Create Sound UI
	SoundUI sound_ui(g_fmod_manager, g_localization);

	// Loading Ply Models From the File
	PlyFileReader* pfr;
	std::vector<PlyFileReader*> v_PlyModels;
	pfr = new PlyFileReader("./assets/models/tictactoeNewBoard.ply");
	v_PlyModels.push_back(pfr);
	short numberOf_X_ToDrawn = 5;
	pfr = new PlyFileReader("./assets/models/tictactoeX.ply");
	v_PlyModels.push_back(pfr);
	short numberOf_O_ToDrawn = 4;
	pfr = new PlyFileReader("./assets/models/tictactoeO.ply");
	v_PlyModels.push_back(pfr);
	
	// Game Board
	MeshObject   board(meshID::board, "board", posS);
	board.loadVerticesToBeDrawn(v_PlyModels[0]->pTheModelArray, v_PlyModels[0]->pTheModelTriangleArray, v_PlyModels[0]->m_numberOfTriangles);
	g_vMeshObjects.push_back(board);
	// Game Pieces by Turn Order
	MeshObject pieceX1(meshID::pieceX, "X1", behindCamera);
	pieceX1.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceX1);
	MeshObject pieceO1(meshID::pieceO, "O1", behindCamera);
	pieceO1.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceO1);
	MeshObject pieceX2(meshID::pieceX, "X2", behindCamera);
	pieceX2.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceX2);
	MeshObject pieceO2(meshID::pieceO, "O2", behindCamera);
	pieceO2.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceO2);
	MeshObject pieceX3(meshID::pieceX, "X3", behindCamera);
	pieceX3.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceX3);
	MeshObject pieceO3(meshID::pieceO, "O3", behindCamera);
	pieceO3.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceO3);
	MeshObject pieceX4(meshID::pieceX, "X4", behindCamera);
	pieceX4.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceX4);
	MeshObject pieceO4(meshID::pieceO, "O4", behindCamera);
	pieceO4.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceO4);
	MeshObject pieceX5(meshID::pieceX, "X5", behindCamera);
	pieceX5.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	g_vMeshObjects.push_back(pieceX5);
	
	//initialize glfw/glad
	if (!glfwInit())
		exit(EXIT_FAILURE);
	g_window = glfwCreateWindow(1922, 1030, "Sound Engine - Project #1", nullptr, nullptr);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	if (!g_window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(g_window, key_callback);

	glfwMakeContextCurrent(g_window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		return 2;
	}
	glfwSwapInterval(1);

	// Background Collor
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
	if (!ImGui_ImplGlfw_InitForOpenGL(g_window, true) || !ImGui_ImplOpenGL3_Init("#version 460")) {
		return 3;
	}

	//imgui style (dark mode for the win)
	ImGui::StyleColorsDark();

	// Let's Setup the Buffers for each of my mesh types (BOARD, X, O)
	// Asks OpenGL for an ID
	unsigned int boardVAOid;
	unsigned int xVAOid;
	unsigned int oVAOid;
	for (int iMeshType = 0; iMeshType < 3; iMeshType++) {
		switch (iMeshType) {
		case meshID::board:
			glGenVertexArrays(1, &(boardVAOid));
			// Binds this buffer
			glBindVertexArray(boardVAOid);
			// Sets the Buffer
			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex_XYZ_RGB) * board.m_numVerticesToDraw, board.m_vertices, GL_STATIC_DRAW);
			break;
		case meshID::pieceX:
			glGenVertexArrays(1, &(xVAOid));
			// Binds this buffer
			glBindVertexArray(xVAOid);
			// Sets the Buffer
			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex_XYZ_RGB) * pieceX1.m_numVerticesToDraw, pieceX1.m_vertices, GL_STATIC_DRAW);
			break;
		case meshID::pieceO:
			glGenVertexArrays(1, &(oVAOid));
			// Binds this buffer
			glBindVertexArray(oVAOid);
			// Sets the Buffer
			glGenBuffers(1, &vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(sVertex_XYZ_RGB) * pieceO1.m_numVerticesToDraw, pieceO1.m_vertices, GL_STATIC_DRAW);
			break;
		}
		// Compiles the vertex shader
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
		glCompileShader(vertex_shader);
		// Compiles the fragment shader
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
		glCompileShader(fragment_shader);
		// Runs the shaders
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);
		// Grabs the location id of shaders variables
		mvp_location = glGetUniformLocation(program, "MVP");
		vpos_location = glGetAttribLocation(program, "vPos");
		vcol_location = glGetAttribLocation(program, "vCol");
		// Offset used to link my program variables struct(sVertex_XYZ_RGB) to the shaders variable
		GLintptr vertex_position_offset = 0 * sizeof(float);
		GLintptr vertex_color_offset = 3 * sizeof(float);
		// Pass the Vertex Positions to the Shader
		glEnableVertexAttribArray(vpos_location);
		glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(sVertex_XYZ_RGB), (GLvoid*)vertex_position_offset);
		// Pass the Vertex Colors to the Shader
		glEnableVertexAttribArray(vcol_location);
		glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
			sizeof(sVertex_XYZ_RGB), (GLvoid*)vertex_color_offset);

		// Sets VOA to 0
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vpos_location);
		glDisableVertexAttribArray(vcol_location);
	}

	// Starts new TicTacToe Game
	g_tttGame->newGame();

	//game loop
	while (!glfwWindowShouldClose(g_window)) {

		float ratio;
		int width, height;
		glm::mat4x4 m, p, mvp;

		glfwGetFramebufferSize(g_window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		p = glm::perspective(0.6f,
			ratio,
			0.1f,
			1000.0f);
		glm::mat4x4 v = glm::mat4(1.0f);
		glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
		v = glm::lookAt(g_cameraEye,
			g_cameraTarget,
			upVector);
		// mat4x4_mul(mvp, p, m);

		std::vector<MeshObject>::iterator itMesh;
		for (itMesh = g_vMeshObjects.begin(); itMesh != g_vMeshObjects.end(); itMesh++) {
			if (!itMesh->m_isVisible) {
				// Skip this object because its not visible
				continue;
			}

			glCullFace(GL_BACK);
			glEnable(GL_DEPTH_TEST);

			// Make an "identity matrix"
			m = glm::mat4x4(1.0f);

			// Move the object 
			glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
				itMesh->m_position);
			// Apply the move transformation
			m = m * matTranslation;

			mvp = p * v * m;

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glUseProgram(program);
			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));
			if (itMesh->m_name.find("board") != -1) {
				glBindVertexArray(boardVAOid);
			} else if (itMesh->m_name.find("X") != -1) {
				glBindVertexArray(xVAOid);
			} else if (itMesh->m_name.find("O") != -1) {
				glBindVertexArray(oVAOid);
			}
			
			//glDrawArrays(GL_TRIANGLES, 0, itMesh->m_numVerticesToDraw);
			glDrawArrays(GL_TRIANGLES,
				0,
				itMesh->m_numVerticesToDraw);
			glBindVertexArray(0);
		}
		

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (sound_ui.is_my_tool_active)
			sound_ui.render();
		// ImGui Menu Treatment
		switch (sound_ui.menuState) {
		case SoundUI::SAVE :
			g_tttGame->saveGame();
			sound_ui.menuState = SoundUI::IDLE;
			break;
		case SoundUI::LOAD :
			g_tttGame->loadGame();
			newGameSetMeshes();
			loadGameSetMeshes();
			sound_ui.menuState = SoundUI::IDLE;
			break;
		case SoundUI::LANG :
			g_localization->setLanguage(sound_ui.newLang);
			g_fmod_manager->playSound("New Lang", "ch2 fx");
			sound_ui.newLang = "";
			sound_ui.menuState = SoundUI::IDLE;
			break;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(g_window);
		glfwPollEvents();

		/*std::stringstream ssTitle;
		ssTitle << "Camera (x,y,z): "
			<< ::g_cameraEye.x << ", "
			<< ::g_cameraEye.y << ", "
			<< ::g_cameraEye.z;

		std::string theText = ssTitle.str();

		glfwSetWindowTitle(window, ssTitle.str().c_str());*/
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(g_window);
	glfwTerminate();

	return 0;
}
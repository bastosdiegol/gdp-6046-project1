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

TicTacToeGame*	ttt;
GLFWwindow*		window;
FModManager*	fmod_manager;
bool			lastMoveResult = true;

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

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	// Creates Channel Groups
	fmod_manager->createChannelGroup("ch0 master");
	fmod_manager->createChannelGroup("ch1 music");
	fmod_manager->createChannelGroup("ch2 fx");

	// Creates Parent Child relation between channels
	fmod_manager->setChannelGroupParent("ch0 master", "ch1 music");
	fmod_manager->setChannelGroupParent("ch0 master", "ch2 fx");

	// Sets initial volume for master channel to 20%
	// WARNING: DO NOT PASS VOLUME VALUE ABOVE 1.0f
	fmod_manager->setChannelGroupVolume("ch0 master", 0.7f);
	fmod_manager->setChannelGroupVolume("ch1 music", 0.15f);
	fmod_manager->setChannelGroupVolume("ch2 fx", 0.7f);

	// Load all sounds from the XML File
	fmod_manager->loadSoundsFromFile();

	// Load all DSP
	fmod_manager->loadDSPs();

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

	std::vector<MeshObject> v_MeshObjects;
	// Game Board
	MeshObject   board(meshID::board, "board", glm::vec3(0.0f));
	board.loadVerticesToBeDrawn(v_PlyModels[0]->pTheModelArray, v_PlyModels[0]->pTheModelTriangleArray, v_PlyModels[0]->m_numberOfTriangles);
	v_MeshObjects.push_back(board);
	// Game Pieces X
	MeshObject pieceX1(meshID::pieceX, "X1", glm::vec3(-10.0f, 0.0f, 0.0f));
	pieceX1.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceX1);
	MeshObject pieceX2(meshID::pieceX, "X2", glm::vec3(-10.0f, 0.0f, 10.0f));
	pieceX2.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceX2);
	MeshObject pieceX3(meshID::pieceX, "X3", glm::vec3(-10.0f, 0.0f, -10.0f));
	pieceX3.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceX3);
	MeshObject pieceX4(meshID::pieceX, "X4", glm::vec3(10.0f, 0.0f, 10.0f));
	pieceX4.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceX4);
	MeshObject pieceX5(meshID::pieceX, "X5", glm::vec3(10.0f, 0.0f, -10.0f));
	pieceX5.loadVerticesToBeDrawn(v_PlyModels[1]->pTheModelArray, v_PlyModels[1]->pTheModelTriangleArray, v_PlyModels[1]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceX5);
	// Game Pieces O
	MeshObject pieceO1(meshID::pieceO, "O1", glm::vec3(0.0f));
	pieceO1.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceO1);
	MeshObject pieceO2(meshID::pieceO, "O2", glm::vec3(0.0f));
	pieceO2.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceO2);
	MeshObject pieceO3(meshID::pieceO, "O3", glm::vec3(0.0f));
	pieceO3.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceO3);
	MeshObject pieceO4(meshID::pieceO, "O4", glm::vec3(0.0f));
	pieceO4.loadVerticesToBeDrawn(v_PlyModels[2]->pTheModelArray, v_PlyModels[2]->pTheModelTriangleArray, v_PlyModels[2]->m_numberOfTriangles);
	v_MeshObjects.push_back(pieceO4);
	
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
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true) || !ImGui_ImplOpenGL3_Init("#version 460")) {
		return 3;
	}

	//imgui style (dark mode for the win)
	ImGui::StyleColorsDark();

	//create sound ui
	SoundUI sound_ui(fmod_manager);

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glDisableVertexAttribArray(vpos_location);
		glDisableVertexAttribArray(vcol_location);
	}

	// Starts new TicTacToe Game
	ttt->newGame();

	//game loop
	while (!glfwWindowShouldClose(window)) {

		float ratio;
		int width, height;
		glm::mat4x4 m, p, mvp;

		glfwGetFramebufferSize(window, &width, &height);
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
		for (itMesh = v_MeshObjects.begin(); itMesh != v_MeshObjects.end(); itMesh++) {
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
			glDrawElements(GL_TRIANGLES,
				itMesh->m_numVerticesToDraw,
				GL_UNSIGNED_INT,
				(void*)0);
			glBindVertexArray(0);
		}
		

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (sound_ui.is_my_tool_active)
			sound_ui.render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
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

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
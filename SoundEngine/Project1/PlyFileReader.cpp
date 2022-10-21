#include "PlyFileReader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif

// Default Constructor
//PlyFileReader::PlyFileReader() {
//	DEBUG_PRINT("PlyFileReader::PlyFileReader()");
//	m_numberOfVertices = -1;
//	m_numberOfTriangles = -1;
//	pTheModelArray = nullptr;
//	pTheModelTriangleArray = nullptr;
//}

// Constructor
PlyFileReader::PlyFileReader(std::string file_location) {
	DEBUG_PRINT("PlyFileReader::PlyFileReader(%s)\n", m_fileLocation.c_str());
	m_fileLocation = file_location;
	m_numberOfVertices = -1;
	m_numberOfTriangles = -1;
	pTheModelArray = nullptr;
	pTheModelTriangleArray = nullptr;
	loadMeshFromFile();
}

// Destructor
PlyFileReader::~PlyFileReader() {
	DEBUG_PRINT("PlyFileReader::~PlyFileReader()\n");
	delete[] pTheModelArray;
	delete[] pTheModelTriangleArray;
}

// Method that reads a Mesh File and store its Header, Vertices and Faces on memory
void PlyFileReader::loadMeshFromFile() {
	DEBUG_PRINT("PlyFileReader::loadMeshFromFile(%s)\n", m_fileLocation.c_str());
	// Loads the file
	std::ifstream theFile(m_fileLocation);
	// Checks if the file is open
	if (!theFile.is_open()) {
		std::cout << "File " << m_fileLocation << " could not be opened!" << std::endl;
		return;
	}

	// Iteration to Find the Total of Vertices
	std::string theNextToken;
	do {
		theFile >> theNextToken;
	} while (theNextToken != "vertex");
	theFile >> theNextToken;
	// Sets the total of Vertices
	m_numberOfVertices = std::stoul(theNextToken);
	// Create new instance of Vertex array for the model
	pTheModelArray = new sVertex_XYZ_N_RGBA_UV[m_numberOfVertices];

	// Iteration to Find the Total of Triangles
	do {
		theFile >> theNextToken;
	} while (theNextToken != "face");
	theFile >> theNextToken;
	// Sets the total of Faces
	m_numberOfTriangles = std::stoul(theNextToken);
	// Create a new instance of Triangles Array for the model
	pTheModelTriangleArray = new sTrianglePLY[m_numberOfTriangles];

	// Runs the file till reach "end_header" and be ready to read vertices
	do {
		theFile >> theNextToken;
	} while (theNextToken != "end_header");

	DEBUG_PRINT("Loading %s vertices ", m_fileLocation.c_str());
	// Iteration to read and store all vertices
	for (unsigned int i = 0; i < m_numberOfVertices; i++) {
		// Stores each property value per Vertice
		theFile >> pTheModelArray[i].x;
		theFile >> pTheModelArray[i].y;
		theFile >> pTheModelArray[i].z;
								  
		theFile >> pTheModelArray[i].nx;
		theFile >> pTheModelArray[i].ny;
		theFile >> pTheModelArray[i].nz;
								  
		theFile >> pTheModelArray[i].red;
		theFile >> pTheModelArray[i].green;
		theFile >> pTheModelArray[i].blue;
		theFile >> pTheModelArray[i].alpha;
								  
		//theFile >> pTheModelArray[i].texture_u;
		//theFile >> pTheModelArray[i].texture_v;

		if (i % 10000 == 0) {
			DEBUG_PRINT(".");
		}
	}
	std::cout << "done!" << std::endl;

	// IF we need to transform models we could do something here...
	/*for (unsigned int i = 0; i < m_numberOfVertices; i++) {
		pTheModelArray->x *= 0.1f;
		pTheModelArray->x += 20.0f;
	}*/

	DEBUG_PRINT("Loading %s triangle faces ", m_fileLocation.c_str());
	// Iteration to read and store all vertices
	for (unsigned int i = 0; i < m_numberOfTriangles; i++) {
		// First value of triangles is its ID that has no use
		unsigned int discard = 0;
		theFile >> discard;
		// Stores all vertices for each triangle
		theFile >> pTheModelTriangleArray[i].vertexIndices[0];
		theFile >> pTheModelTriangleArray[i].vertexIndices[1];
		theFile >> pTheModelTriangleArray[i].vertexIndices[2];
	}
	std::cout << "done!" << std::endl;

	theFile.close();
}

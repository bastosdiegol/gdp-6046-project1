#pragma once
#include <string>
#include <glm/ext/vector_float3.hpp>

#include "PlyFileReader.h"

// Vertex Struct that we are going to send to GPU to be drawn
struct sVertex_XYZ_RGB {
	float x, y, z;	// 0 floats from the start
	float r, g, b;  // 2 floats from the start
					// Each vertex is 5 floats in size
};

enum meshID {
	board, pieceX, pieceO
};

class MeshObject {
public:
	meshID m_id;						// Number for ID
	std::string m_name;					// Friendly name
	glm::vec3 m_position;				// Position to be drawn
	unsigned int m_numVerticesToDraw;	// Total number of vertices of the model
	sVertex_XYZ_RGB* m_vertices;		// Struct containing all the vertices
	bool m_isVisible;					// Bool to make it appear or not on the screen

	// Constructor
	MeshObject(meshID id, std::string name, glm::vec3 position)
		: m_id(id), m_name(name), m_position(position), m_numVerticesToDraw(-1), m_vertices(nullptr), m_isVisible(true) {}

	void loadVerticesToBeDrawn(sVertex_XYZ_N_RGBA_UV* modelArray, sTrianglePLY* facesArray, unsigned int numberOfTriangles);
};


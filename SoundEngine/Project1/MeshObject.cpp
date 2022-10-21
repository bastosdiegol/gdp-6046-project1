#include "MeshObject.h"

void MeshObject::loadVerticesToBeDrawn(sVertex_XYZ_N_RGBA_UV* modelArray, sTrianglePLY* facesArray, unsigned int numberOfTriangles) {
	// Preparing the struct to send to the GPU
	// Number of Vertices to be drawn = Faces * 3 (Vertices)
	m_numVerticesToDraw = numberOfTriangles * 3;
	// Creates news instance of Vertices to be drawn array
	m_vertices = new sVertex_XYZ_RGB[m_numVerticesToDraw];
	// For each Triangle Face LOOP
	// Grabs each Vertice of triangle
	// And stores in the struct array to be passed to the gpu
	unsigned int vertexIndex = 0;
	for (unsigned int triangleIndex = 0; triangleIndex != numberOfTriangles; triangleIndex++) {
		// Face i Vertice 1 Index
		unsigned int vert0Index = facesArray[triangleIndex].vertexIndices[0];
		// Stores vertice 1 variables
		m_vertices[vertexIndex + 0].x = modelArray[vert0Index].x;
		m_vertices[vertexIndex + 0].y = modelArray[vert0Index].y;
		m_vertices[vertexIndex + 0].z = modelArray[vert0Index].z;
		m_vertices[vertexIndex + 0].r = modelArray[vert0Index].red / 255;
		m_vertices[vertexIndex + 0].g = modelArray[vert0Index].green / 255;
		m_vertices[vertexIndex + 0].b = modelArray[vert0Index].blue / 255;
		// Face i Vertice 2 Index
		unsigned int vert1Index = facesArray[triangleIndex].vertexIndices[1];
		// Stores vertice 2 variables
		m_vertices[vertexIndex + 1].x = modelArray[vert1Index].x;
		m_vertices[vertexIndex + 1].y = modelArray[vert1Index].y;
		m_vertices[vertexIndex + 1].z = modelArray[vert1Index].z;
		m_vertices[vertexIndex + 1].r = modelArray[vert1Index].red / 255;
		m_vertices[vertexIndex + 1].g = modelArray[vert1Index].green / 255;
		m_vertices[vertexIndex + 1].b = modelArray[vert1Index].blue / 255;
		// Face i Vertice 3 Index
		unsigned int vert2Index = facesArray[triangleIndex].vertexIndices[2];
		// Stores vertice 3 variables
		m_vertices[vertexIndex + 2].x = modelArray[vert2Index].x;
		m_vertices[vertexIndex + 2].y = modelArray[vert2Index].y;
		m_vertices[vertexIndex + 2].z = modelArray[vert2Index].z;
		m_vertices[vertexIndex + 2].r = modelArray[vert2Index].red / 255;
		m_vertices[vertexIndex + 2].g = modelArray[vert2Index].green / 255;
		m_vertices[vertexIndex + 2].b = modelArray[vert2Index].blue / 255;

		// increment the vertex values by 3
		vertexIndex += 3;
	}
}

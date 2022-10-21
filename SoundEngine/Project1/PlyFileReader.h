#pragma once
#include <string>

// We read the Ply File and Store Vertices on this struct
struct sVertex_XYZ_N_RGBA_UV {
    float x, y, z;
    float nx, ny, nz;
    float red, green, blue, alpha;
    float texture_u, texture_v;
};
// We also store each face on this struct
struct sTrianglePLY {
    // The 3 vertex index values from the ply file
    unsigned int vertexIndices[3];
};

class PlyFileReader {
public:
    // Path to the Ply File
    std::string m_fileLocation;

    // Array we will load into
    sVertex_XYZ_N_RGBA_UV* pTheModelArray;
    sTrianglePLY* pTheModelTriangleArray;

    // Total of Vertices and Faces
    unsigned int m_numberOfVertices;
    unsigned int m_numberOfTriangles;

    // Constructor
    PlyFileReader(std::string file_location);
    // Destructor
    ~PlyFileReader();

private:
    // Method that reads a Mesh File and store its Header, Vertices and Faces on memory
    void loadMeshFromFile();
};


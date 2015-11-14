#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>


/// Representation of a 3D vertex.
struct Vertex
{
    float x;
    float y;
    float z;
};

/// Representation of a polygonal face.
/// The indexes in a face represents indexes into the Vertex
/// array for which vertices make up a face.
/// Faces with more than three vertices are supported by the obj
/// format, but we only support loading triangles.
struct Face
{
    int indexX;
    int indexY;
    int indexZ;
};

class ObjFile
{
private:
	std::string m_filename;
	std::vector<Vertex> m_vertices;
	std::vector<Face> m_faces;

public:
    ObjFile();
    ~ObjFile();

	/// Loads a Wavefrom Obj file.
	///
	/// Returns whether the file was loaded successfully.
    bool Load(const std::string& filename);

	/// Returns the loaded list of vertices, or none if nothing has been loaded.
    const std::vector<Vertex>& GetVertices() const;

	/// Returns the loaded list of faces, or none if nothing has been loaded.
	const std::vector<Face>& GetFaces() const;

	void RetrieveVerticiesFromFile();
	void RetrieveFacesFromFile();
};

#endif // OBJFILE_H
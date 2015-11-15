//FileLoader.h

#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>

struct Vertex
{
    float x;
    float y;
    float z;
};

struct Face
{
    int indexX;
    int indexY;
    int indexZ;
};

class ObjFile
{
private:
    std::string m_fileName;
    std::vector<Vertex> m_vertices;
    std::vector<Face> m_faces;

    //[???] This is my solution to passing local variables
    //      I like this because the user doesn't have to free the memory
    //      but you also are taking up double the memory to do it.
    //      Is there a better solution?
    //      I guess I could just avoid the structs but then there's the issue of
    //      any person managing the verts individually being more of a pain
    std::vector<float> m_verticesAsFloats;
    std::vector<unsigned int> m_facesAsIndices;

public:
    bool Load(const char* fileName);
    void RetrieveVertsFromFile();
    void RetrieveFacesFromFile();

    //[???] I know there's no overloading for return values, but is there a better way to do this?
    //      templates seem like they could do it but would be unsafe
    const std::vector<Vertex>& GetVerticesAsStructs() const;
    std::vector<float>& GetVerticesAsFloats();
    const std::vector<Face>& GetFacesAsStructs() const;
    std::vector<unsigned int>& ObjFile::GetFacesAsIndices();
};

#endif // !FILELOADER_H

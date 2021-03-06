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

typedef unsigned int GLuint;

class ObjFile
{

private:
    std::string m_fileName;
    std::vector<Vertex> m_vertices;
    std::vector<Face> m_faces;

    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    GLuint m_elementBufferObject;

    //[???] This is my solution to passing local variables
    //      I like this because the user doesn't have to free the memory
    //      but you also are taking up double the memory to do it.
    //      Is there a better solution?
    //      I guess I could just avoid the structs but then there's the issue of
    //      any person managing the verts individually being more of a pain
    std::vector<float> m_verticesAsFloats;
    std::vector<unsigned int> m_facesAsIndices;

public:
    ~ObjFile();

    bool Load(const char* fileName);
    void RetrieveVertsFromFile();
    void RetrieveFacesFromFile();

    void CreateVertexArrayObject();
    void CreateVertexBufferObject();
    void CreateElementBufferObject();

    //[???] I know there's no overloading for return values, but is there a better way to do this?
    //      templates seem like they could do it but would be unsafe
    const std::vector<Vertex>& GetVerticesAsStructs() const;
    std::vector<float>& GetVerticesAsFloats();
    const std::vector<Face>& GetFacesAsStructs() const;
    std::vector<unsigned int>& ObjFile::GetFacesAsIndices();

    unsigned int GetNumberOfIndices() const { return m_facesAsIndices.size(); }
    void* GetStartingIndexAddress() { return &m_facesAsIndices[0]; }
};

class ShaderFile
{
private:
    std::string m_fileName;
    std::string m_fileSource;

public:
    bool Load(const char* fileName);
    const char* GetSource() const 
    { 
        return m_fileSource.c_str(); 
    }
};

#endif // !FILELOADER_H

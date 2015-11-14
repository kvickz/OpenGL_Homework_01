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

public:
    bool Load(const char* fileName);
    void RetrieveVertsFromFile();
    void RetrieveFacesFromFile();

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<Face>& GetFaces() const;
};

#endif // !FILELOADER_H

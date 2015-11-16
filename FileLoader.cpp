//FileLoader.cpp

#include "FileLoader.h"

#include "Debug.h"
#include "SDLWrapper.h"

#include <fstream>



bool ObjFile::Load(const char* fileName)
{
    std::ifstream tempStream;

    tempStream.open(fileName);

    //Error Checking
    if (!tempStream.is_open())
    {
        Debug::Log("File in ObjFile::Load failed to open!");
        return false;
    }

    m_fileName = fileName;

    RetrieveVertsFromFile();
    RetrieveFacesFromFile();

    return true;
}

void ObjFile::RetrieveVertsFromFile()
{
    std::vector<Vertex> tempVertCollection;
    std::ifstream tempStream;
    tempStream.open(m_fileName);
    unsigned int currentLineNumber = 0;

    while (!tempStream.eof())
    {
        std::string currentLine;

        std::getline(tempStream, currentLine);

        //Continue until we hit vertices
        if (currentLine[0] != 'v')
            continue;

        //Once we've hit faces, start loading those
        if (currentLine[0] == 'f')
            return;

        std::string tempNumberString;
        unsigned int currentIndex = 0;
        Vertex tempVertex;

        //For every character in the current line
        for (int i = 2; i <= (int)currentLine.size(); ++i)
        {
            char currentChar = currentLine[i];

            //Keep pushing back characters until we've hit a space
            if (currentChar != ' ' && currentChar != '\0')
            {
                tempNumberString.push_back(currentChar);
                continue;
            } 
            
            float tempFloat = (float)::atof(tempNumberString.c_str());

            switch (currentIndex)
            {
            case 0:
                tempVertex.x = tempFloat;
                break;
            case 1:
                tempVertex.y = tempFloat;
                break;
            case 2:
                tempVertex.z = tempFloat;
                break;
            default:
                //End of line or error
                break;
            }

            tempNumberString.clear();
            ++currentIndex;
        }

        //Store tempVertex into the member collection
        m_vertices.push_back(tempVertex);

    }   //End of file

    //Allocate verts into sequential collection
    GetVerticesAsFloats();
}

void ObjFile::RetrieveFacesFromFile()
{
    std::vector<Face> tempFaceCollection;
    std::ifstream tempStream;
    tempStream.open(m_fileName);
    unsigned int currentLineNumber = 0;

    while (!tempStream.eof())
    {
        std::string currentLine;
        std::getline(tempStream, currentLine);

        //Skip to the current line
        if (currentLine[0] != 'f')
            continue;
            
        Face tempFace;
        std::string tempIndexString;
        unsigned int currentIndex = 0;

        for (unsigned int i = 2; i <= currentLine.size(); ++i)
        {
            char currentChar = currentLine[i];

            //Keep pushing back characters until we've hit a space
            if (currentChar != ' ' && currentChar != '\0')
            {
                tempIndexString.push_back(currentChar);
                continue;
            }

            //All of the indexes are 1-based, so need to subtract 1 from the end
            unsigned int tempInt = ((unsigned int)::atoi(tempIndexString.c_str()) - 1);

            switch (currentIndex)
            {
            case 0:
                tempFace.indexX = tempInt;
                break;
            case 1:
                tempFace.indexY = tempInt;
                break;
            case 2:
                tempFace.indexZ = tempInt;
                break;
            default:
                //End of line or error
                break;
            }

            tempIndexString.clear();
            ++currentIndex;
        }

        //push back index
        m_faces.push_back(tempFace);

    }   //End of file

    //Allocate faces into sequential collection
    GetFacesAsIndices();
}

void ObjFile::CreateVertexArrayObject()
{
    //Creating Vertex Array Object
    glGenVertexArrays(1, &m_vertexArrayObject); //Create
    glBindVertexArray(m_vertexArrayObject);     //Make active
}

void ObjFile::CreateVertexBufferObject()
{
    //Create Vertex Buffer Object
    glGenBuffers(1, &m_vertexBufferObject);              //Creating Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject); //Making the vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, m_verticesAsFloats.size(), &m_verticesAsFloats[0], GL_STATIC_DRAW);
}

void ObjFile::CreateElementBufferObject()
{
    //Create Element Buffer
    glGenBuffers(1, &m_elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_facesAsIndices.size(), &m_facesAsIndices[0], GL_STATIC_DRAW);
}

ObjFile::~ObjFile()
{
    glDeleteBuffers(1, &m_elementBufferObject);
    glDeleteBuffers(1, &m_vertexBufferObject);
    glDeleteVertexArrays(1, &m_vertexArrayObject);
}

//---------------------------------------------------------------------------- -
//  Get Vertices Function
//      -Returns the vertices in the form of structs.
//---------------------------------------------------------------------------- -
const std::vector<Vertex>& ObjFile::GetVerticesAsStructs() const
{
    return m_vertices;   
}

//---------------------------------------------------------------------------- -
//  Get Vertices Function
//      -Returns the vertices in the form of floats.
//---------------------------------------------------------------------------- -
std::vector<float>& ObjFile::GetVerticesAsFloats()
{
    //Get the size and create an array from it
    m_verticesAsFloats.resize(m_vertices.size() * 3);

    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        unsigned int vertIndex = i * 3;
        m_verticesAsFloats[vertIndex] = m_vertices[i].x;
        m_verticesAsFloats[vertIndex + 1] = m_vertices[i].y;
        m_verticesAsFloats[vertIndex + 2] = m_vertices[i].z;
    }

    return m_verticesAsFloats;
}

const std::vector<Face>& ObjFile::GetFacesAsStructs() const
{
    return m_faces;
}

//---------------------------------------------------------------------------- -
//  Get Faces Function
//      -Returns the vertices in the form of floats.
//---------------------------------------------------------------------------- -
std::vector<unsigned int>& ObjFile::GetFacesAsIndices()
{
    //Get the size and create an array from it
    m_facesAsIndices.resize(m_faces.size() * 3);

    for (unsigned int i = 0; i < m_faces.size(); ++i)
    {
        unsigned int faceIndex = i * 3;
        m_facesAsIndices[faceIndex] = m_faces[i].indexX;
        m_facesAsIndices[faceIndex + 1] = m_faces[i].indexY;
        m_facesAsIndices[faceIndex + 2] = m_faces[i].indexZ;
    }

    return m_facesAsIndices;
}

bool ShaderFile::Load(const char* fileName)
{
    std::ifstream tempStream;
    m_fileSource.clear();

    tempStream.open(fileName);

    //Error checking
    if (!tempStream.is_open())
    {
        Debug::Log("File in ShaderFile::Load failed to open!");
        return false;
    }

    std::string currentLine;

    while (std::getline(tempStream, currentLine))
    {
        m_fileSource += currentLine + "\n";
    }

    m_fileName = fileName;

    return true;
}

//const std::string ShaderFile::GetSource() const
//{
//    return m_fileSource;
//}
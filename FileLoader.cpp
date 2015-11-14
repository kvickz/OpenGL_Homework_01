//FileLoader.cpp

#include "FileLoader.h"
#include "Debug.h"
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

            unsigned int tempInt = (unsigned int)::atoi(tempIndexString.c_str());

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

    int debug = 0;
}

const std::vector<Vertex>& ObjFile::GetVertices() const
{
    return m_vertices;
}

const std::vector<Face>& ObjFile::GetFaces() const
{
    return m_faces;
}
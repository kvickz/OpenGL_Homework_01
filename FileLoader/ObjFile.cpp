//Nevins Michael
//GAP 275
//Module 8 Homework assignment
//10-30-2015

#include "ObjFile.h"
#include <iostream>
#include <fstream>

ObjFile::ObjFile()
{
}

ObjFile::~ObjFile()
{
}

bool ObjFile::Load(const std::string& filename)
{
	//Temporary variables-----------------
	//
	std::ifstream tempStream;
	
	//
	//------------------------------------

	tempStream.open(filename);
	if (!tempStream.is_open())
	{
		std::cout << "Failed to open file!" << std::endl;
		return false;
	}
	
	m_filename = filename;

	RetrieveVerticiesFromFile();
	RetrieveFacesFromFile();

	return true;
}

void ObjFile::RetrieveVerticiesFromFile()
{
	std::vector<Vertex> tempVertexCollection;
	std::ifstream tempStream;
	tempStream.open(m_filename);

	while (!tempStream.eof())
	{
		//Temporary variables-----------------
		//
		bool validEntry = false;
		std::string tempLine;
		std::string currentLine;
		std::string tempVerts[3];
		int currentCoordinate = 0;
		//
		//------------------------------------

		//Assign current line to the string
		std::getline(tempStream, currentLine);

		//If we aren't on a 'v' skip until we find one
		if (currentLine[0] != 'v')
			continue;

		//For current line, check every character
		for (int i = 0; i < (int)currentLine.size(); ++i)
		{
			char ch = currentLine[i];

			//once we've hit a 'v', start recording numbers
			if (ch == 'v')
			{
				validEntry = true;

				int offset = 2;
				//start at 1 ahead of 'v'

				while (currentCoordinate < 3)
				{
					//clear current line
					tempLine.clear();

					//Skip the space
					ch = currentLine[i + offset];

					//Until we've hit the next space, record all the chars
					while (ch != ' ' && (offset + i) < (int)currentLine.size())
					{
						tempLine += ch;

						//Move ahead
						offset++;

						//record next character
						ch = currentLine[i + offset];
					}

					//Store the string as a coordinate, get the rest
					tempVerts[currentCoordinate] = tempLine;
					++currentCoordinate;

					if (currentCoordinate == 3)
						break;

					//skip the space
					++offset;
				}
				break;
			}
		}

		//Anything beyond this point we can assume contains a 'v' at the beginning of the line
		//Also assuming no improperly formatted files are being loaded

		//Temporary vertex
		Vertex tempVertex;

		//For each coordinate
		for (int i = 0; i < 3; ++i)
		{
			float tempFloat;
			bool isNegative = false;

			if (tempVerts[i][0] == '-')
				isNegative = true;

			//Convert string to float
			tempFloat = (float)(::atof(tempVerts[i].c_str()));

			//Assign float to corresponding coordinate
			switch (i)
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
				std::cout << "ERROR assigning tempVertex.";
				break;
			}
		}	//End of assigning a single XY or Z component

		//Assign coordinate to the collection
		tempVertexCollection.push_back(tempVertex);

	}	// after end of file

	m_vertices = std::vector<Vertex>(tempVertexCollection);
}

void ObjFile::RetrieveFacesFromFile()
{
	//Temporary Variables
	std::ifstream tempStream;
	std::vector<Face> tempVector;

	//Open the file
	tempStream.open(m_filename);

	//Continue until the end of the file
	while (!tempStream.eof())
	{
		//Temporary Variables
		std::string currentLine;
		Face tempFace;

		//Get the current line and assign it to currentLine
		std::getline(tempStream, currentLine);

		//if the line doesn't start with f, then skip to next line
		if (currentLine[0] != 'f')
			continue;

		//Here we can assume that we're reached a face, also assuming we're opening a 
		//properly formatted and valid file

		//Create variable noting which component index we're storing XY or Z
		unsigned int currentIndex = 0;
		int offset = 0;

		//Start on the third character
		for (int i = 2; (i + offset) < (int)currentLine.size(); ++i)
		{
			std::string tempValueString;

			//clear string
			//tempValueString.clear();

			//collect the whole number until reaching a space
			while ((i + offset) < (int)currentLine.size() && currentLine[i + offset] != ' ')
			{
				tempValueString += currentLine[i + offset];
				++offset;
			}	//End of recording current vertex index

			//convert the string to an int
			unsigned int tempValueInt = ::atoi(tempValueString.c_str());

			//push it into a temporary array
			//tempVerticies[currentIndex] = tempValueInt;

			switch (currentIndex)
			{
			case 0:
				tempFace.indexX = tempValueInt;
				break;
			case 1:
				tempFace.indexY = tempValueInt;
				break;
			case 2:
				tempFace.indexZ = tempValueInt;
				break;
			default:
				std::cout << "Error in Face vertex assignments." << std::endl;
				break;
			}

			//move onto the next index in this current face
			++currentIndex;
		}	//End of the current line

		//Push the Face into the vector
		tempVector.push_back(tempFace);

		m_faces = std::vector<Face>(tempVector);

	}	//End of file
}

const std::vector<Vertex>& ObjFile::GetVertices() const
{
	return m_vertices;
	//return (std::vector<Vertex>(tempVertexCollection)); // TODO:
}

const std::vector<Face>& ObjFile::GetFaces() const
{
	return m_faces;
	//return std::vector<Face>(tempVector); // TODO
}

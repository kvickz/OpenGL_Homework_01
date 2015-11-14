//Main.cpp

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>

#include "FileLoader.h"

//Shader sources
const GLchar* vertexSource =
"#version 400\n"
"in vec3 position;"
//"in vec3 color;"
//"out vec3 Color;"
"void main() {"
//"    Color = color;"
"    gl_Position = vec4(position, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 400\n"
//"in vec3 Color;"
"out vec4 outColor;"
"void main() {"
"    outColor = vec4(0.6, 0.6, 0.6, 0.6);"
"}";

int main(int argc, char* argv[])
{

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_Window* pWindow = SDL_CreateWindow("OpenGL Homework"
                                           , -1200, 300
                                           , 800, 600
                                           , SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    //INIT GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    //Load suzanne
    ObjFile objFile;
    objFile.Load("hello.obj");

    //TODO: Refactor into the ObjFile class
    //This is janky, but it loads all of the coordinates into an array
    auto vertCollection = objFile.GetVertices();

    unsigned int vertCollectionSize = (vertCollection.size() * 3);
    GLfloat* pVertices;
    pVertices = new GLfloat[vertCollectionSize];

    for (unsigned int i = 0; i < vertCollection.size(); ++i)
    {
        unsigned int index = (i * 3);
        pVertices[index] = vertCollection[i].x;
        pVertices[index + 1] = vertCollection[i].y;
        pVertices[index + 2] = vertCollection[i].z;
    }

    //Creating Vertex Array Object
    GLuint vao;                 //Declare
    glGenVertexArrays(1, &vao); //Create
    glBindVertexArray(vao);     //Make active

    //Create Vertex Buffer Object
    GLuint vbo;                         //Memory managed by openGL so this works in place of a pointer
    glGenBuffers(1, &vbo);              //Creating Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //Making the vbo the active array buffer
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //This copies the vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, vertCollectionSize, pVertices, GL_STATIC_DRAW);  //This copies the vertex data into the buffer
    //  NOTE: This function doesn't refer to the ID of the VBO, but to the active array buffer
    //  The last parameter can be 
    //      -GL_STATIC_DRAW: Vertex data uploaded once and drawn many times (ex. the world)
    //      -GL_DYNAMIC_DRAW: Vertex data will be changes from time to time, but drawn many times more than changed
    //      -GL_STREAM_DRAW: Vertex data will change almost every time it's drawn (ex. user interface)
    //  These determine where in memory the data will be stored at, stream will allow faster writing for slower drawing
    //

    //Element Buffer
    GLuint* pElements;

    auto faceCollection = objFile.GetFaces();

    unsigned int faceCollectionSize = (faceCollection.size() * 3);
    pElements = new GLuint[faceCollectionSize];

    for (unsigned int i = 0; i < faceCollection.size(); ++i)
    {
        unsigned int index = (i * 3);
        pElements[index] = faceCollection[i].indexX;
        pElements[index + 1] = faceCollection[i].indexY;
        pElements[index + 2] = faceCollection[i].indexZ;
    }

    const char* vertexShaderCode[] =
    {
        "#version 400\n", // This is the only line that requires a newline, all others do not need it!
        "in vec3 vertex;",
        "void main() {",
        "  gl_Position = vec4(vertex, 1.0);",
        "}",
    };

    const char* fragmentShaderCode[] =
    {
        "#version 400\n", // This is the only line that requires a newline, all others do not need it!
        "out vec4 colorRGBA;",
        "void main() {",
        "  colorRGBA = vec4(1.0, 0.0, 0.0, 1.0);",
        "}",
    };

    //Same as creating a vertex buffer object
    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCollectionSize, pElements, GL_STATIC_DRAW);

    //explain to the graphics card how to handle these attributes.

    //Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    //Error checking
    //GLint status;
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    //True = good

    //Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
    glCompileShader(fragmentShader);

    //Create & attach programs(shaders)
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    //  Specify layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    bool running = true;
    SDL_Event windowEvent;
    while (running)
    {
        while (SDL_PollEvent(&windowEvent))
        {
            if (windowEvent.type == SDL_QUIT || windowEvent.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
                break;
            }
        }

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Draw!
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        //glDrawElements(GL_TRIANGLES, faceCollectionSize, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(pWindow);
    }


    //Cleanup

    delete pElements;
    pElements = nullptr;
    delete pVertices;
    pVertices = nullptr;

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
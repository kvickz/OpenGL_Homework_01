//Main.cpp

#include "SDLWrapper.h"
#include "FileLoader.h"

//Shader sources
const GLchar* vertexSource =
"#version 400\n"
"layout (location = 0) in vec3 position;"
"void main() {"
"    gl_Position = vec4(position, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 400\n"
"out vec4 outColor;"
"void main() {"
"    outColor = vec4(0.6, 0.6, 0.6, 0.6);"
"}";

int main(int argc, char* argv[])
{
    //Init SDL & GLEW
    SDLWrapper sdlWrapper;
    sdlWrapper.Init();

    //Load file
    ObjFile objFile;
    objFile.Load("hello.obj");

    //Get the address to the first element of the vertex collection
    auto vertCollection = objFile.GetVerticesAsFloats();
    GLfloat* pVertices = &vertCollection[0];

    //Creating Vertex Array Object
    GLuint vao;                 //Declare
    glGenVertexArrays(1, &vao); //Create
    glBindVertexArray(vao);     //Make active

    //Create Vertex Buffer Object
    GLuint vbo;                         //Memory managed by openGL so this works in place of a pointer
    glGenBuffers(1, &vbo);              //Creating Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //Making the vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, vertCollection.size(), pVertices, GL_STREAM_DRAW);  //This copies the vertex data into the buffer
    //  NOTE: This function doesn't refer to the ID of the VBO, but to the active array buffer
    //  The last parameter can be 
    //      -GL_STATIC_DRAW: Vertex data uploaded once and drawn many times (ex. the world)
    //      -GL_DYNAMIC_DRAW: Vertex data will be changes from time to time, but drawn many times more than changed
    //      -GL_STREAM_DRAW: Vertex data will change almost every time it's drawn (ex. user interface)
    //  These determine where in memory the data will be stored at, stream will allow faster writing for slower drawing
    //

    //Element Buffer
    auto faceCollection = objFile.GetFacesAsIndices();
    GLuint* pElements = &faceCollection[0];

    //Same as creating a vertex buffer object
    GLuint elementBufferObject;
    glGenBuffers(1, &elementBufferObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faceCollection.size(), pElements, GL_STATIC_DRAW);

    //explain to the graphics card how to handle these attributes.

    //Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);

    //Error checking
    GLint status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
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
        
        //Draw
        glDrawElements(GL_TRIANGLES, faceCollection.size(), GL_UNSIGNED_INT, 0);

        sdlWrapper.SwapWindow();
    }

    //Cleanup
    pElements = nullptr;
    pVertices = nullptr;

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    sdlWrapper.Shutdown();

    return 0;
}
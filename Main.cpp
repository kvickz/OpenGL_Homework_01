//Main.cpp

#include "SDLWrapper.h"
#include "FileLoader.h"

bool PollEvents(SDL_Event& windowEvent);
GLuint CompileShader(const GLchar* file, GLenum shaderType);
GLuint CreateAndLinkProgram(GLuint shaders[], unsigned int length);
void ToggleObj();

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

    //Explain to the graphics card how to handle these attributes.

    ShaderFile vertexSourceFile;
    vertexSourceFile.Load("VertexShader.glsl");
    ShaderFile fragmentSourceFile;
    fragmentSourceFile.Load("FragmentShader.glsl");

    //Create & Compile Vertex & Fragment Shader
    GLuint vertexShader = CompileShader(vertexSourceFile.GetSource(), GL_VERTEX_SHADER);
    //GLuint vertexShader = CompileShader(string.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentSourceFile.GetSource(), GL_FRAGMENT_SHADER);

    //Create & attach programs(shaders)
    GLuint shaders[] = { vertexShader, fragmentShader };
    GLuint shaderProgram = CreateAndLinkProgram(shaders, 2);

    //  Specify layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    bool running = true;
    SDL_Event windowEvent;
    while (running)
    {
        running = PollEvents(windowEvent);

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

//Returns the GLuint identifier
GLuint CreateAndLinkProgram(GLuint shaders[], unsigned int length)
{
    GLuint shaderProgram = glCreateProgram();

    //Attach all shaders
    for (unsigned int i = 0; i < length; ++i)
    {
        glAttachShader(shaderProgram, shaders[i]);
    }

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram); 

    return shaderProgram;
}

//Returns the GLuint identifier
GLuint CompileShader(const GLchar* file, GLenum shaderType)
{
    //Create shader
    GLuint id = glCreateShader(shaderType);
    //Compile Shader
    glShaderSource(id, 1, &file, nullptr);
    glCompileShader(id);

    //Error checking
    GLint status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    //True = good

    return id;
}

bool PollEvents(SDL_Event& windowEvent)
{
    while (SDL_PollEvent(&windowEvent))
    {
        if (windowEvent.type == SDL_QUIT || windowEvent.key.keysym.sym == SDLK_ESCAPE)
        {
            return false;
        }

        if (windowEvent.key.keysym.sym == SDLK_SPACE)
        {
            ToggleObj();
        }
    }

    return true;
}

void ToggleObj()
{

}
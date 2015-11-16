//Main.cpp

#include "SDLWrapper.h"
#include "FileLoader.h"

#include <vld.h>

#include <assert.h>

bool PollEvents(SDL_Event& windowEvent);
GLuint CompileShader(const GLchar* file, GLenum shaderType);
GLuint CreateAndLinkProgram(GLuint shaders[], unsigned int length);
void ToggleObj();

const int k_numOfObjects = 1;
unsigned int g_currentIndex = 0;

void CheckGLError()
{
    GLenum error = glGetError();
    if (error != 0)
    {
        SDL_Log("GL Error: %d %s", error, glewGetErrorString(error));
        __debugbreak();
    }
}

int main(int argc, char* argv[])
{
    //Init SDL & GLEW
    SDLWrapper sdlWrapper;
    sdlWrapper.Init();

    GLenum enumer = glGetError();

    //Load file
    ObjFile objFiles[k_numOfObjects];
    std::string objectsToLoad[k_numOfObjects] =
    { 
        //"test.obj",
        //"hello.obj",
        "suzanne.obj",
        //"triangle.obj",
        //"quad.obj",
    };

    //Load all files in the array above
    for (int i = 0; i < k_numOfObjects; ++i)
    {
        objFiles[i].Load(objectsToLoad[i].c_str());

        //Create VertexArray, VertexBuffer, and IndexBuffer Objects
        objFiles[i].CreateVertexArrayObject();
        objFiles[i].CreateVertexBufferObject();
        objFiles[i].CreateElementBufferObject();
    }
    
    //Explain to the graphics card how to handle these attributes.

    //Load Shaders
    ShaderFile vertexSourceFile;
    vertexSourceFile.Load("VertexShader.glsl");
    ShaderFile fragmentSourceFile;
    fragmentSourceFile.Load("FragmentShader.glsl");

    //Create & Compile Vertex & Fragment Shader
    GLuint vertexShader = CompileShader(vertexSourceFile.GetSource(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentSourceFile.GetSource(), GL_FRAGMENT_SHADER);

    //Create & attach programs(shaders)
    GLuint shaders[] = { vertexShader, fragmentShader };
    GLuint shaderProgram = CreateAndLinkProgram(shaders, 2);

    //  Specify layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    CheckGLError();

    bool running = true;
    SDL_Event windowEvent;
    while (running)
    {
        running = PollEvents(windowEvent);

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Draw
        unsigned int byteOffset = 0;

        if (g_currentIndex == 1)
            byteOffset = objFiles[g_currentIndex].GetNumberOfIndices() * sizeof(float);

        glDrawElements(GL_TRIANGLES, objFiles[g_currentIndex].GetNumberOfIndices(), GL_UNSIGNED_INT, 0);

        sdlWrapper.SwapWindow();
    }

    //Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

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

    assert(status);

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
    ++g_currentIndex;

    if (g_currentIndex >= k_numOfObjects)
        g_currentIndex = 0;
}
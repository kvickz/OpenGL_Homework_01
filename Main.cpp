//Main.cpp

#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>

#include <math.h>

//Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec2 position;"
"in vec3 color;"
"out vec3 Color;"
"void main() {"
"    Color = color;"
"    gl_Position = vec4(position, 0.0, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"out vec4 outColor;"
"void main() {"
"    outColor = vec4(Color, 1.0);"
"}";

int main(int argc, char* argv[])
{

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_Window* pWindow = SDL_CreateWindow("OpenGL Tut"
                                           , -1200, 300
                                           , 800, 600
                                           , SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(pWindow);

    //INIT GLEW
    glewExperimental = GL_TRUE;
    glewInit();
    
    /*
    GLfloat vertices[] = {
        -1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,
    };
    */
    
    //With more colors  -X, Y, R, G, B
    GLfloat vertices[] = {
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,       //topleft
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,        //topright
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f,       //bottomright
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f,      //bottomleft
        -0.8f, 0.0f, 1.0f, 0.0f, 1.0f,       //mid left

    };

    //Creating Vertex Array Object
    GLuint vao;                 //Declare
    glGenVertexArrays(1, &vao); //Create    
    glBindVertexArray(vao);     //Make active

    //Create Vertex Buffer Object
    GLuint vbo;                         //Memory managed by openGL so this works in place of a pointer
    glGenBuffers(1, &vbo);              //Creating Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //Making the vbo the active array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //This copies the vertex data into the buffer
    //  NOTE: This function doesn't refer to the ID of the VBO, but to the active array buffer
    //  The last parameter can be 
    //      -GL_STATIC_DRAW: Vertex data uploaded once and drawn many times (ex. the world)
    //      -GL_DYNAMIC_DRAW: Vertex data will be changes from time to time, but drawn many times more than changed
    //      -GL_STREAM_DRAW: Vertex data will change almost every time it's drawn (ex. user interface)
    //  These determine where in memory the data will be stored at, stream will allow faster writing for slower drawing
    //  

    //Element Buffer
    GLuint elements[] =
    {
        0, 1, 2,
        2, 3, 4,
        4, 0, 2,
        //2, 3, 4,
        //2, 3, 0,
    };

    //Same as creating a vertex buffer object
    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    //Now we have to explain to the graphics card how to handle these attributes.

    //---------------------------------------------------------------------------------- -
    //  Shaders
    //---------------------------------------------------------------------------------- -
    //  Vertex Shader
    //  (optional)Geometry Shader
    //  Fragment Shader

    /*
    const char* vertexSource[] = 
    {
        "#version 400\n", // This is the only line that requires a newline, all others do not need it!

        "layout(location = 0) in vec2 position;",

        "layout(location = 1) in vec3 inColor;",
        "out vec3 xColor;"

        "void main() {",
        "  xColor = inColor;",
        "  gl_Position = vec4(position, 0.0, 1.0);",
        "}",
    };

    const char* fragmentSource[] =
    {
        "#version 400\n", // This is the only line that requires a newline, all others do not need it!

        "uniform vec3 triangleColor;",

        "in vec3 xColor;",
        "out vec4 outColor;",

        "void main() {",
        //"  outColor = vec4(1.0, 1.0, 1.0, 1.0);",
        //"  outColor = vec4(triangleColor, 1.0);",
        "  outColor = vec4(xColor, 1.0);",
        "}",
    };
    */

    //Compile Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    //Error checking
    //GLint status;
    //glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    //True = good

    //Compile Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    //Create & attach programs(shaders)
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    //  Specify layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
    
    //specify how input is retrieved from the array
    
    //  -'2' in the function above is the number of values for that input
    //    2 in this case because of the vector 2
    //  -The GL_FALSE says, that the input values should not be normalized -1 to 1
    //  -The last two parameters specify how the attribute is laid out
    //   inside of the vertex array, the first number is 'stride'
    //   how many bytes are between each position attribute
    //   -Last parameter isthe offset, or how many bytes from
    //   The start of the array the attribute occurs
    //    
    //  Because we only have 1 attribute, both of these are zero
    //
    //  This function will store in addition to the stride and offset, the VBO
    //  that is currently bound to GL_ARRAY_BUFFER. This means that you don't
    //  have to explicitly bind the correct VBO when the actual drawing calls occur.
    //  It also implies you can use a different VBO for each attribute <-??? still confusing

    //Get location of the color attribute
    //GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    
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
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, ARRAYSIZE(elements), GL_UNSIGNED_INT, 0);

        //Changing color attribute
        //GLfloat newFloat = sinf((float)uniColor + (SDL_GetTicks() * 0.005f)) * 0.5f + 0.5f;
        //glUniform3f(uniColor, newFloat, 0.0f, 0.0f);


        SDL_GL_SwapWindow(pWindow);
    }


    //Cleanup
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    SDL_GL_DeleteContext(context);
    SDL_Quit();

    return 0;
}
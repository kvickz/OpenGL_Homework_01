//Debug.h

#ifndef DEBUG_H
#define DEBUG_H

#include <string>

//-------------------------------------------------------------------------------------- -
// Debug Class
//  -This acts as a wrapper for SDL debug functions
//-------------------------------------------------------------------------------------- -

class Debug
{
public:
    static void Log(std::string message);
};

#endif // !DEBUG_H

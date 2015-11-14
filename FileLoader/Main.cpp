#include <vector>
#include <iostream>

bool TestObjFileWithCube();
bool TestObjFileWithSuzanne();
bool TestObjFileWithNonexistentFile();

void main()
{
    typedef bool(*TestFunc)();

    std::vector<TestFunc> testFuncs =
    {
        &TestObjFileWithCube,
        &TestObjFileWithSuzanne,
        &TestObjFileWithNonexistentFile
    };

    int succeeded = 0;
    for (auto runTest : testFuncs)
    {
        succeeded += runTest();
    }

    std::cout << "RESULT: " << succeeded << " of " << testFuncs.size() << " tests passed." << std::endl;
}
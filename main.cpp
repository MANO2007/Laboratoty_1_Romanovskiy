#include "main.hpp"
int main()
{
    try
    {
        Manager manager;
        manager.Run();
    }
    catch (const exception& error)
    {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

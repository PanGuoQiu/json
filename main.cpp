#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/timeb.h>
#include "Json.h"

using namespace pan::json;

int main()
{
    std::ifstream fin("../test.json");
    std::stringstream ss;
    ss << fin.rdbuf();
    const string &data = ss.str();
    
    Json v;
    v.parse(data);
    std::cout << v.str() << std::endl;

    return 0;
}
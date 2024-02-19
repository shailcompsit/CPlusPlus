#include "FileSystem.h"

#include <iostream>

int main()
{
    FileSystem fs;
    fs.mkdir("/abc");
    fs.touch("/abc/shailesh");
    fs.touch("/abc/kapse");
    auto list = fs.ls("/abc");
    for (auto &itr : list)
        std::cout<<itr<<std::endl;
    std::string input = "I am Shailesh";
    std::string input1 = "I am Kapse";
    fs.echo("/abc/shailesh", {input.begin(), input.end()});
    fs.echo("/abc/kapse", {input1.begin(), input1.end()});
    auto content = fs.cat("/abc/shailesh");
    std::cout<<std::string(content.begin(), content.end())<<std::endl;
    auto content1 = fs.cat("/abc/kapse");
    std::cout<<std::string(content1.begin(), content1.end())<<std::endl;
    return 0; 
}
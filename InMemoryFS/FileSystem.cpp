#include "FileSystem.h"
#include <string>
#include <sstream>

namespace
{
std::vector<std::string> GetTokens(const std::string& path, const char delim)
{
    std::vector<std::string> tokens;
    std::stringstream ss(path);
    std::string token;
    while(std::getline(ss, token, delim))
    {
        if (!token.empty())
            tokens.emplace_back(token);
    }
    return tokens;
}
}

/* PRIVATE FUNCIONS*/

FileNode* FileSystem::GetNode(const std::string &path)
{
    auto paths = GetTokens(path, '/');
    auto curr = root_;
    for (const auto &str : paths)
    {
        if (curr)
            curr = curr->NextToken(str);
    }
    return curr;
}

FileNode* FileSystem::PutNode(const std::string &path, bool isFile)
{
    auto paths = GetTokens(path, '/');
    auto curr = root_;
    for (const auto &str : paths)
    {
        if (curr)
        {
            FileNode* prev = curr;
            curr = curr->NextToken(str);
            if (!curr)
            {
                if (isFile)
                {
                    auto loc = memory_.insert(memory_.end(), std::move(File()));
                    prev->AddPath(str, loc, true);
                }
                else
                {
                    prev->AddPath(str, memory_.end(), false);
                }
            }
        }
    }
    return curr;
}

/*PUBLIC FUNCTIONS*/

std::vector<std::string> FileSystem::ls(const std::string path)
{
    auto node = GetNode(path);
    if (node->IsFile())
        return { GetTokens(path, '/').back() };
    
    std::vector<std::string> output;
    for (const auto& child : node->GetChildren())
    {
        output.emplace_back(child.first);
    }
    sort(output.begin(), output.end());
    return output;
}

void FileSystem::mkdir(const std::string& path)
{
    PutNode(path, false);
}

void FileSystem::touch(const std::string& path)
{
    PutNode(path, true);
}

void FileSystem::echo(const std::string &path, std::vector<unsigned char> content)
{
    auto node = GetNode(path);
    node->GetFile()->AddContent(std::move(content));
}

const std::vector<unsigned char>& FileSystem::cat(const std::string &path)
{
    auto node = GetNode(path);
    return node->GetFile()->GetContent();
}


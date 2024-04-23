#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

class File
{
private:
    std::vector<unsigned char> content_;
    size_t size_;
public:
    File(std::vector<unsigned char> input = std::vector<unsigned char>()) : content_(std::move(input)) 
    { 
        size_ = content_.size();
    }
    ~File() = default;

    //File(const File &copy) = delete;
    //File(const File &&copy) = delete;
    //File& operator=(const File &copy) = delete;
    //File& operator=(const File &&copy) = delete;

    void AddContent(std::vector<unsigned char> input)
    {
        size_ += input.size();
        std::copy(input.begin(),input.end(), std::back_inserter(content_));
    }

    size_t GetSize() const
    {
        return size_;
    }

    const std::vector<unsigned char>& GetContent() const
    {
        return content_;
    }
};

class FileNode
{
private:
    std::unordered_map<std::string, FileNode*> children_;
    bool isFile_;
    std::list<File>::iterator loc_;
public:
    FileNode(std::list<File>::iterator loc, bool isFile = false) : isFile_(isFile), loc_(loc) { }
    ~FileNode() = default;

    FileNode* NextToken(const std::string& path) const 
    {
        const auto &itr = children_.find(path);
        if (itr != children_.end())
            return itr->second;
        return nullptr;
    }

    void AddPath(const std::string& path, std::list<File>::iterator loc, bool isFile=false)
    {
        const auto &itr = children_.find(path);
        if (itr == children_.end())
        {
            children_[path] = new FileNode(loc, isFile);
        }
    }

    const std::unordered_map<std::string, FileNode*>& GetChildren()
    {
        return children_;
    }

    inline bool IsFile()
    {
        return isFile_;
    }

    std::list<File>::iterator GetFile() const
    {
        if (isFile_)
            return loc_;
        throw std::runtime_error("Not a file");
    }
};

class FileSystem
{
private:
    FileNode *root_;
    std::list<File> memory_;

    FileNode* GetNode(const std::string &path);
    FileNode* PutNode(const std::string &path, bool isFile=false);

public:
    FileSystem() : root_(new FileNode(std::list<File>::iterator {})) { }

    std::vector<std::string> ls(const std::string path="");
    void mkdir(const std::string& path);
    const std::vector<unsigned char>& cat(const std::string &path);
    void touch(const std::string &path);
    void echo(const std::string &path, std::vector<unsigned char> content);

};

#endif  //FILESYSTEM_H

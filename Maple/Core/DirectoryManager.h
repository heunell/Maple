#pragma once
#include "Common/Singleton.h"
#include <fstream>
#include <filesystem>
#include <unordered_map>

class DirectoryManager : public Singleton<DirectoryManager>
{
public:
    DirectoryManager()  = default;
    ~DirectoryManager() = default;
    DirectoryManager(const DirectoryManager&) = delete;
    DirectoryManager(DirectoryManager&&) = delete;
    DirectoryManager& operator=(const DirectoryManager&) = delete;
    DirectoryManager& operator=(DirectoryManager&&) = delete;
    
private:
    std::filesystem::path _rootPath;
    std::filesystem::path _rootParentPath;
    std::unordered_map<std::string, std::filesystem::path> _paths;
    
private:
    void RegisterPath(const std::string& path);
    
public:
    void Init();
    virtual void Destroy() override;
    
    const std::filesystem::path& GetRootPath() const { return _rootPath; }
    std::optional<std::filesystem::path> GetCachePath(const std::string pathName) const;
    bool GetDirectoryFromRoot(const std::string& dirName, OUT std::filesystem::path& outVal);
    bool GetDirectoryFromRoot(const std::filesystem::path& dir, OUT std::filesystem::path& outVal);
    bool GetDirectory(const std::filesystem::path& basePath, const std::filesystem::path& dirName, OUT std::filesystem::path& outVal);
    bool GetFile(const std::filesystem::path& basePath, const std::filesystem::path& fileName, OUT std::filesystem::path& outVal);
    bool IsFile(const std::filesystem::path& path);
    bool IsDirectory(const std::filesystem::path& path);
    bool IsExtension(const std::filesystem::path& path, const std::string& extension);
};

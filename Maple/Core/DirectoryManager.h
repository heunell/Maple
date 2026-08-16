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
    std::filesystem::path _RootPath;
    
    std::filesystem::path _RootParentPath;
    
    std::unordered_map<std::string, std::filesystem::path> _Paths;
    
private:
    void RegisterPath(const std::string& Path);
    
public:
    void Init();
   
    virtual void Destroy() override;
    
    const std::filesystem::path& GetRootPath() const { return _RootPath; }
    
    std::optional<std::filesystem::path> GetCachePath(const std::string PathName) const;
    
    bool GetDirectoryFromRoot(const std::string& DirectoryName, OUT std::filesystem::path& OutVal);
    
    bool GetDirectoryFromRoot(const std::filesystem::path& Directory, OUT std::filesystem::path& OutVal);
    
    bool GetDirectory(const std::filesystem::path& BasePath, const std::filesystem::path& DirectoryName, OUT std::filesystem::path& OutVal);
    
    bool GetFile(const std::filesystem::path& BasePath, const std::filesystem::path& FileName, OUT std::filesystem::path& OutVal);
    
    bool IsFile(const std::filesystem::path& Path);
    
    bool IsDirectory(const std::filesystem::path& Path);
    
    bool IsExtension(const std::filesystem::path& Path, const std::string& Extension);
};

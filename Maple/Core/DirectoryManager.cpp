#include "pch.h"
#include "DirectoryManager.h"

void DirectoryManager::RegisterPath(const std::string& path)
{
    std::filesystem::path registeredPath = _rootPath / path;
    if (!std::filesystem::exists(registeredPath))
        std::filesystem::create_directories(registeredPath);
    
    if (!std::filesystem::is_directory(registeredPath))
        return;
    
    _paths[path] = registeredPath;
}

void DirectoryManager::Init()
{
    _rootPath = std::filesystem::current_path();
    if (!std::filesystem::exists(_rootPath))
        return;
    
    if (!std::filesystem::is_directory(_rootPath))
        return;
    
    _rootParentPath = _rootPath.parent_path();
    if (!std::filesystem::exists(_rootParentPath))
        return;
    if (!std::filesystem::is_directory(_rootParentPath))
        return;
    
    RegisterPath("Logs");
    RegisterPath("HLSL");
    RegisterPath("Resources");
    RegisterPath("Resources\\Level");
}

void DirectoryManager::Destroy()
{
    _paths.clear();
}

std::optional<std::filesystem::path> DirectoryManager::GetCachePath(const std::string pathName) const
{
    auto it = _paths.find(pathName);
    if (it == _paths.end())
        return std::nullopt;
    return it->second;
}

bool DirectoryManager::GetDirectoryFromRoot(const std::string& dirName, std::filesystem::path& outVal)
{
    std::filesystem::path foundPath = _rootPath / dirName;
    if (!std::filesystem::exists(foundPath))
        return false;
    if (!std::filesystem::is_directory(foundPath))
        return false;
    outVal = foundPath;
    return true;
}

bool DirectoryManager::GetDirectoryFromRoot(const std::filesystem::path& dir, std::filesystem::path& outVal)
{
    std::filesystem::path foundPath = _rootPath / dir;
    if (!std::filesystem::exists(foundPath))
        return false;
    if (!std::filesystem::is_directory(foundPath))
        return false;
    outVal = foundPath;
    return true;
}

bool DirectoryManager::GetDirectory(const std::filesystem::path& basePath, const std::filesystem::path& dirName, std::filesystem::path& outVal)
{
    std::filesystem::path foundPath = basePath / dirName;
    if (!std::filesystem::exists(foundPath))
        return false;
    if (!std::filesystem::is_directory(foundPath))
        return false;
    outVal = foundPath;
    return true;
}

bool DirectoryManager::GetFile(const std::filesystem::path& basePath, const std::filesystem::path& fileName, std::filesystem::path& outVal)
{
    std::filesystem::path foundPath = basePath / fileName;
    outVal = foundPath;
    if (!std::filesystem::exists(foundPath))
        return false;
    if (!std::filesystem::is_regular_file(foundPath))
        return false;
    return true;
}

bool DirectoryManager::IsFile(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return false;
    if (!std::filesystem::is_regular_file(path))
        return false;
    return true;
}

bool DirectoryManager::IsDirectory(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path))
        return false;
    if (!std::filesystem::is_directory(path))
        return false;
    return true;
}

bool DirectoryManager::IsExtension(const std::filesystem::path& path, const std::string& extension)
{
    std::filesystem::path fileExtension = path.extension();
    std::string strExtension = fileExtension.string();
    std::string compareExtension = extension;
    std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), toupper);
    std::transform(compareExtension.begin(), compareExtension.end(), compareExtension.begin(), toupper);
    if (compareExtension == strExtension)
        return true;
    return false;
}

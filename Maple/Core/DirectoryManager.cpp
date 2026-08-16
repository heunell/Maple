#include "pch.h"
#include "DirectoryManager.h"

void DirectoryManager::RegisterPath(const std::string& Path)
{
    std::filesystem::path registeredPath = _RootParentPath / Path;
    if (!std::filesystem::exists(registeredPath))
        std::filesystem::create_directories(registeredPath);
    
    if (!std::filesystem::is_directory(registeredPath))
        return;
    
    _Paths[Path] = registeredPath;
}

void DirectoryManager::Init()
{
    _RootPath = std::filesystem::current_path();
    if (!std::filesystem::exists(_RootPath))
        return;
    
    if (!std::filesystem::is_directory(_RootPath))
        return;
    
    _RootParentPath = _RootPath.parent_path();
    if (!std::filesystem::exists(_RootParentPath))
        return;
    if (!std::filesystem::is_directory(_RootParentPath))
        return;
    
    RegisterPath("Logs");
    RegisterPath("HLSL");
    RegisterPath("Resources");
    RegisterPath("Resources\\Level");
}

void DirectoryManager::Destroy()
{
    _Paths.clear();
}

std::optional<std::filesystem::path> DirectoryManager::GetCachePath(const std::string PathName) const
{
    auto it = _Paths.find(PathName);
    if (it == _Paths.end())
        return std::nullopt;
    return it->second;
}

bool DirectoryManager::GetDirectoryFromRoot(const std::string& DirectoryName, std::filesystem::path& OutVal)
{
    std::filesystem::path FoundPath = _RootPath / DirectoryName;
    if (!std::filesystem::exists(FoundPath))
        return false;
    if (!std::filesystem::is_directory(FoundPath))
        return false;
    OutVal = FoundPath;
    return true;
}

bool DirectoryManager::GetDirectoryFromRoot(const std::filesystem::path& Directory, std::filesystem::path& OutVal)
{
    std::filesystem::path FoundPath = _RootPath / Directory;
    if (!std::filesystem::exists(FoundPath))
        return false;
    if (!std::filesystem::is_directory(FoundPath))
        return false;
    OutVal = FoundPath;
    return true;
}

bool DirectoryManager::GetDirectory(const std::filesystem::path& BasePath, const std::filesystem::path& DirectoryName, std::filesystem::path& OutVal)
{
    std::filesystem::path FoundPath = BasePath / DirectoryName;
    if (!std::filesystem::exists(FoundPath))
        return false;
    if (!std::filesystem::is_directory(FoundPath))
        return false;
    OutVal = FoundPath;
    return true;
}

bool DirectoryManager::GetFile(const std::filesystem::path& BasePath, const std::filesystem::path& FileName, std::filesystem::path& OutVal)
{
    std::filesystem::path FoundPath = BasePath / FileName;
    OutVal = FoundPath;
    if (!std::filesystem::exists(FoundPath))
        return false;
    if (!std::filesystem::is_regular_file(FoundPath))
        return false;
    return true;
}

bool DirectoryManager::IsFile(const std::filesystem::path& Path)
{
    if (!std::filesystem::exists(Path))
        return false;
    if (!std::filesystem::is_regular_file(Path))
        return false;
    return true;
}

bool DirectoryManager::IsDirectory(const std::filesystem::path& Path)
{
    if (!std::filesystem::exists(Path))
        return false;
    if (!std::filesystem::is_directory(Path))
        return false;
    return true;
}

bool DirectoryManager::IsExtension(const std::filesystem::path& Path, const std::string& Extension)
{
    std::filesystem::path FileExtension = Path.extension();
    std::string StringExtension = FileExtension.string();
    std::string CompareExtension = Extension;
    std::transform(StringExtension.begin(), StringExtension.end(), StringExtension.begin(), toupper);
    std::transform(CompareExtension.begin(), CompareExtension.end(), CompareExtension.begin(), toupper);
    if (CompareExtension == StringExtension)
        return true;
    return false;
}

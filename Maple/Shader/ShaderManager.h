#pragma once
#include "Common/Singleton.h"
#include "Shader.h"
#include "SBuffer.h"
#include "CBuffer.h"
#include <unordered_map>

class ShaderManager : public Singleton<ShaderManager>
{
public:
    ShaderManager() = default;
    ~ShaderManager() = default;
    ShaderManager(const ShaderManager&) = delete;
    ShaderManager(ShaderManager&&) = delete;
    ShaderManager& operator=(const ShaderManager&) = delete;
    ShaderManager& operator=(ShaderManager&&) = delete;
    
protected:
    std::unordered_map<std::string, Ptr<Shader>>  _Shaders;

    std::unordered_map<std::string, Ptr<CBuffer>> _cBuffers;

    std::unordered_map<std::string, Ptr<SBuffer>> _sBuffers;

    ComPtr<ID3D11SamplerState> _Samplers[TEXTURE_SAMPLE_END];
    
private:
    void CreateSampler();
    
public:
    bool Init();
   
    virtual void Destroy() override;
    
    Ptr<Shader> FindShader(const std::string& Name);
    
    void SetSample(eTextureSampleType Type);
    
    template<typename T>
    Ptr<T> FindCBuffer(const std::string& Name)
    {
        auto it = _cBuffers.find(Name);
        if (_cBuffers.end() == it)
        {
            return nullptr;
        }
        
        return Cast<CBuffer, T>(it->second);
    }
    
    template<typename T>
    Ptr<T> FindSBuffer(const std::string& Name)
    {
        auto it = _sBuffers.find(Name);
       
        if(_sBuffers.end() == it)
        {
            return nullptr;
        }
        return Cast<SBuffer, T>(it->second);
    }
    
private:
    template<typename T>
    bool CreateShader(const std::string& Name)
    {
        Ptr<Shader> _Shader = FindShader(Name);
       
        if (_Shader)
        {
            return false;
        }
        
        _Shader = New<T>();
       
        if (false == _Shader->Init())
        {
            Delete(_Shader);
            
            return false;
        }
        _Shaders[Name] = _Shader;
        
        return true;
    }
    
    template<typename T>
    bool CreateCBuffer(const std::string& Name, int32 Size, int32 Register, int32 Type)
    {
        Ptr<T> cBuffer = FindCBuffer<T>(Name);
       
        if (cBuffer)
        {
            return false;
        }
        
        cBuffer = New<T>();

        if (false == cBuffer->Create(Size, Register, Type))
        {
            Delete(cBuffer);

            return false;
        }
        
        _cBuffers[Name] = cBuffer;
        
        return true;
    }
    
    template<typename T>
    bool CreateSBuffer(const std::string& Name, int32 Size, int32 ElementCount, int32 Register, int32 Type)
    {
        Ptr<T> sBuffer = FindSBuffer<T>(Name);
       
        if (sBuffer)
        {
            return false;
        }
        
        sBuffer = New<T>();
        
        if (false == sBuffer->Create(Size, ElementCount, Register, Type))
        {
            DESTROY(sBuffer);
           
            return false;
        }
       
        _sBuffers[Name] = sBuffer;
       
        return true;
    }
};

#define SHADER_MANAGER ShaderManager::Instance()

#define FIND_SHADER(x) ShaderManager::Instance().FindShader(x)

#define FIND_CBUFFER(x, T) ShaderManager::Instance().FindCBuffer<T>(x)

#define FIND_SBUFFER(x, T) ShaderManager::Instance().FindSBuffer<T>(x)
#pragma once
#include "SubManager.h"
#include <unordered_map>

class MeshManager : public SubManager
{
public:
	MeshManager();
	virtual ~MeshManager();
	MeshManager(const MeshManager&) = delete;
	MeshManager(MeshManager&&) = delete;
	MeshManager& operator=(const MeshManager&) = delete;
	MeshManager& operator=(MeshManager&&) = delete;

private:
	std::unordered_map<std::string, Ptr<class Mesh>> _Mesh;

public:
	Ptr<class Mesh> FindMesh(const std::string& Name);

	bool CreateMesh(const std::string& Name, void* VertexData, int32 Size, int32 Count, D3D11_USAGE VertexUsage,
		D3D11_PRIMITIVE_TOPOLOGY Primitive, void* IndexData = nullptr, int32 IndexSize = 0, int32 IndexCount = 0,
		DXGI_FORMAT Fmt = DXGI_FORMAT_UNKNOWN, D3D11_USAGE IndexUsage = D3D11_USAGE_DEFAULT);

	virtual bool Init() override;

	virtual void Destroy() override;
};


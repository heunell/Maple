#pragma once
#include "GraphicShader.h"

class FrameMeshShader : public GraphicShader
{
public:
	FrameMeshShader();
	~FrameMeshShader();

public:
	virtual bool Init() override;
};


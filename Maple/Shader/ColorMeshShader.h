#pragma once
#include "Graphicshader.h"

class ColorMeshShader : public GraphicShader
{
public:
	ColorMeshShader();
	~ColorMeshShader();

public:
	virtual bool Init() override;
};


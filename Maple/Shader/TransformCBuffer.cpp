#include "pch.h"
#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer()
{}

TransformCBuffer::~TransformCBuffer()
{}

void TransformCBuffer::Update()
{
    _Data._wv  = _Data._World * _Data._View;

    _Data._wvp = _Data._wv * _Data._Proj;

    _Data._World.Transpose();

    _Data._View.Transpose();
    
    _Data._Proj.Transpose();
    
    _Data._wv.Transpose();
    
    _Data._wvp.Transpose();

    SetData(&_Data);
}

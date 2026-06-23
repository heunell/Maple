#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include <memory>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>
#include <string>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <dwrite_3.h>
#include <d2d1.h>

using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;

using int8   = char;
using int16  = short;
using int32  = int;
using int64  = long long;

#include "wrl.h"
using namespace Microsoft::WRL;

#include <Common/Vector2D.h>
#include <Common/Vector3D.h>
#include <Common/Vector2D.h>
#include <Common/Matrix.h>
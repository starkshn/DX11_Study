#pragma once

#include "Types.h"
#include "Values.h"
#include "Struct.h"

// STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
using namespace std;

#include <memory>

// WIN API
#include <windows.h>
#include <assert.h>

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

// direct 텍스쳐 로딩 관련해서 헤더 (외부 라이브러리 에서 골라준 것이다)
#include <DirectXTex/DirectXTex.h>
using namespace DirectX;
using namespace Microsoft::WRL;

#include <chrono>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex\\DirectXTex.lib")
#endif


// Mecros
#define _XM_NO_INTRINSICS_

#define C(p)	assert(SUCCEEDED(p))
#define Z(p, s) ZeroMemory(p, s)
#define H		HRESULT


// Class Wrapping

// Engine
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "Geometry.h"
#include "GeometryHelper.h"
#include "VertexData.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "Texture.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "BlendState.h"
#include "Pipeline.h"
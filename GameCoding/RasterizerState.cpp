#include "pch.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::Create()
{
	D3D11_RASTERIZER_DESC desc;
	Z(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;		// 기본 모드 (중요)
	// D3D11_FILL_WIREFRAME 와이어 프레임 모드
	desc.CullMode = D3D11_CULL_BACK;		// 기본 모드 (중요)
	desc.FrontCounterClockwise = false;		// 기본 모드 (중요)

	H hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	C(hr);
}

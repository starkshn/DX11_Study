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
	desc.FillMode = D3D11_FILL_SOLID;		// �⺻ ��� (�߿�)
	// D3D11_FILL_WIREFRAME ���̾� ������ ���
	desc.CullMode = D3D11_CULL_BACK;		// �⺻ ��� (�߿�)
	desc.FrontCounterClockwise = false;		// �⺻ ��� (�߿�)

	H hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	C(hr);
}

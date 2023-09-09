#include "pch.h"
#include "Game.h"

Game::Game()
{

}

Game::~Game()
{

}

void Game::Init(HWND hWnd)
{
	_hWnd			= hWnd;
	_graphcis		= make_shared<Graphics>(hWnd);
	_vertexBuffer	= make_shared<VertexBuffer>(_graphcis->GetDevice());
	_indexBuffer	= make_shared<IndexBuffer>(_graphcis->GetDevice());
	_inputLayout	= make_shared<InputLayout>(_graphcis->GetDevice());
	_geometry		= make_shared<Geometry<VertexTextureData>>();
	_vertexShader	= make_shared<VertexShader>(_graphcis->GetDevice());
	_pixelShader	= make_shared<PixelShader>(_graphcis->GetDevice());
	_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_graphcis->GetDevice(), _graphcis->GetDeviceContext());
	_texture1		= make_shared<Texture>(_graphcis->GetDevice());

	// Geometry
	{
		// Vertex Data
		GeometryHelper::CreateRectangle(_geometry);

		// VertexBuffer
		_vertexBuffer->CreateVertexBuffer(_geometry->GetVertices());

		// IndexBuffer
		_indexBuffer->CreateIndexBuffer(_geometry->GetIndices());
	}
	
	// Shader
	{
		_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");
		_inputLayout->CraeteInputLayout(VertexTextureData::descs, _vertexShader->GetBlob());
		_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");
	}

	CreateRasterizerState(); // Rasterizer State
	CreateSamplerState();
	CreateBlendState();

	// SRV
	{
		_texture1->Create(L"BFS.jpg");
	}
	_constantBuffer->Create(); // ConstantBuffer Create
}

void Game::Update()
{
	_localPosition.x += 0.001f;

	// SRT
	Matrix matScale = Matrix::CreateScale(_localScale / 3);
	Matrix matRotation = Matrix::CreateRotationX(_localRotation.x);
	matRotation *= Matrix::CreateRotationY(_localRotation.y);
	matRotation *= Matrix::CreateRotationZ(_localRotation.z);
	Matrix matTranslation = Matrix::CreateTranslation(_localPosition);
	Matrix matWorld = matScale * matRotation * matTranslation; // SRT 행렬
	_transformData.matWorld = matWorld;   

	// CPU -> GPU로의 데이터 복사
	_constantBuffer->CopyData(_transformData);
}

void Game::Render()
{
	_graphcis->RenderBegin();

	// TODO
	// IA(Input Assembler) - VS(Vertex Shader) - RS - PS(Pixel Shader) - OM(Output)
	{
		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		auto DC = _graphcis->GetDeviceContext();

		// IA
		DC->IASetVertexBuffers(0, 1, _vertexBuffer->GetComPtr().GetAddressOf(), &stride, &offset); // 1. vertex buffer
		DC->IASetIndexBuffer(_indexBuffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0); // 2. index buffer
		DC->IASetInputLayout(_inputLayout->GetComPtr().Get()); 		// 3. Input Layout
		DC->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		DC->VSSetShader(_vertexShader->GetComPtr().Get(), nullptr, 0);
		DC->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());

		// RS
		DC->RSSetState(_rasterizerState.Get());

		// PS
		DC->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
		DC->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());
		DC->PSSetSamplers(0, 1, _samplerState.GetAddressOf()); // sampler state
		
		// OM
		DC->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);

		DC->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphcis->RenderEnd();
}

void Game::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC desc;
	Z(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;		// 기본 모드 (중요)
	// D3D11_FILL_WIREFRAME 와이어 프레임 모드
	desc.CullMode = D3D11_CULL_BACK;		// 기본 모드 (중요)
	desc.FrontCounterClockwise = false;		// 기본 모드 (중요)

	H hr = _graphcis->GetDevice()->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
	C(hr);
}

void Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC desc;
	Z(&desc, sizeof(desc));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	desc.BorderColor[0] = 1;
	desc.BorderColor[1] = 0;
	desc.BorderColor[2] = 0;
	desc.BorderColor[3] = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.MaxAnisotropy = 16;
	desc.MaxLOD = FLT_MAX;
	desc.MinLOD = FLT_MIN;
	desc.MipLODBias = 0.0f;

	_graphcis->GetDevice()->CreateSamplerState(&desc, _samplerState.GetAddressOf());
}


// 많이 중요하지 않다.
// 블렌딩과 관련된 것이다.
void Game::CreateBlendState()
{
	D3D11_BLEND_DESC desc;
	Z(&desc, sizeof(desc));
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC1_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	H hr = _graphcis->GetDevice()->CreateBlendState(&desc, _blendState.GetAddressOf());
	C(hr);
}

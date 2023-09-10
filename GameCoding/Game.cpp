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
	_rasterizerState = make_shared<RasterizerState>(_graphcis->GetDevice());
	_samplerState	= make_shared<SamplerState>(_graphcis->GetDevice());
	_blendState		= make_shared<BlendState>(_graphcis->GetDevice());

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

	// 삼총사
	{
		_rasterizerState->Create();
		_samplerState->Create();
		_blendState->Create();
	}

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
		DC->RSSetState(_rasterizerState->GetComPtr().Get());

		// PS
		DC->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
		DC->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());
		DC->PSSetSamplers(0, 1, _samplerState->GetComPtr().GetAddressOf()); // sampler state
		
		// OM
		DC->OMSetBlendState(_blendState->GetComPtr().Get(), _blendState->GetBlendFactor(), _blendState->GetSampleMask());

		DC->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphcis->RenderEnd();
}

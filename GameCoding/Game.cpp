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
	_pipeline		= make_shared<Pipeline>(_graphcis->GetDeviceContext());

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

	{
		PipelineInfo info;
		info.inputLayout = _inputLayout;
		info.vertexShader = _vertexShader;
		info.pixelShader = _pixelShader;
		info.rasterizerState = _rasterizerState;
		info.blendState = _blendState;
		info.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		_pipeline->UpdatePipeline(info);

		uint32 stride = sizeof(VertexTextureData);
		uint32 offset = 0;

		auto DC = _graphcis->GetDeviceContext();

		// IA
		_pipeline->SetVertexBuffer(_vertexBuffer);
		_pipeline->SetIndexBuffer(_indexBuffer);

		// VS
		_pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);
		
		// RS
		
		// PS
		_pipeline->SetTexture(0, SS_PixelShader, _texture1);
		_pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);
		
		// OM
		_pipeline->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphcis->RenderEnd();
}

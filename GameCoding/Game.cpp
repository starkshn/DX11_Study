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
	
	CreateGeometry();		// VertextBuffer GPU에게 건내줌
	CreateVS();				// Vertex Shader 생성
	CreateInputLayout();	// InputLayout 생성
	CreatePS();				// Pixel Shader 생성

	CreateRasterizerState(); // Rasterizer State
	CreateSamplerState();
	CreateBlendState();

	CreateSRV();			// Shader Resouce View로드 및 생성
	CreateConstantBuffer(); // Constant Buffer
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

	D3D11_MAPPED_SUBRESOURCE subResouce;
	Z(&subResouce, sizeof(subResouce));

	_graphcis->GetDeviceContext()->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce); // open
	::memcpy(subResouce.pData, &_transformData, sizeof(_transformData)); // CPU -> GPU로 데이터가 복사
	_graphcis->GetDeviceContext()->Unmap(_constantBuffer.Get(), 0); // close
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
		DC->VSSetShader(_vertexShader.Get(), nullptr, 0);
		DC->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

		// RS
		DC->RSSetState(_rasterizerState.Get());

		// PS
		DC->PSSetShader(_pixelShader.Get(), nullptr, 0);
		DC->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		DC->PSSetSamplers(0, 1, _samplerState.GetAddressOf()); // sampler state
		
		// OM
		DC->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);

		DC->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
	}

	_graphcis->RenderEnd();
}

void Game::CreateGeometry()
{
	// Vertex Data
	GeometryHelper::CreateRectangle(_geometry);
	
	// VertexBuffer
	_vertexBuffer->CreateVertexBuffer(_geometry->GetVertices());
	
	// IndexBuffer
	_indexBuffer->CreateIndexBuffer(_geometry->GetIndices());
}

// 데이터를 어떻게 읽어야 할지 알려달라. 그래서 GPU에게 알려주는 함수
void Game::CreateInputLayout()
{
	// Struct.h의 Vertex가 어떻게 되어 있는지 묘사하는 부분
	_inputLayout->CraeteInputLayout(VertexTextureData::descs, _vsBlob);
}

void Game::CreateVS()
{
	// VS만들어 버림.
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	H hr = _graphcis->GetDevice()->CreateVertexShader
	(
		_vsBlob->GetBufferPointer(),
		_vsBlob->GetBufferSize(), 
		nullptr,
		_vertexShader.GetAddressOf()
	);
	C(hr);
}

void Game::CreatePS()
{
	// VS만들어 버려엇
	LoadShaderFromFile(L"Default.hlsl", "PS", "ps_5_0", _psBlob);

	H hr = _graphcis->GetDevice()->CreatePixelShader
	(
		_psBlob->GetBufferPointer(),
		_psBlob->GetBufferSize(),
		nullptr,
		_pixelShader.GetAddressOf()
	);
	C(hr);
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

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	H hr = ::LoadFromWICFile(L"BFS.jpg", WIC_FLAGS_NONE, &md, img);
	C(hr);

	hr = ::CreateShaderResourceView
	(
		_graphcis->GetDevice().Get(), img.GetImages(), img.GetImageCount(),
		md, _shaderResourceView.GetAddressOf()
	);
	C(hr);
}

void Game::CreateConstantBuffer()
{
	D3D11_BUFFER_DESC desc;
	Z(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write+ GPU_Read
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(TransformData);
	desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	
	H hr = _graphcis->GetDevice()->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	C(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// d3dcompiler.h에서
	H hr = ::D3DCompileFromFile
	(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr
	);
	C(hr);
}

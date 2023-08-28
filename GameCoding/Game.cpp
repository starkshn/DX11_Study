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
	_hWnd = hWnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	// TODO
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
	
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
	// 아래 두줄은 이동 테스트
	// _transformData.offset.x += 0.003f;
	// _transformData.offset.y += 0.003f;

	D3D11_MAPPED_SUBRESOURCE subResouce;
	Z(&subResouce, sizeof(subResouce));

	// 뚜껑 열기
	_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);

	::memcpy(subResouce.pData, &_transformData, sizeof(_transformData)); // CPU -> GPU로 데이터가 복사가 될 것이다.

	// 뚜껑 닫기
	_deviceContext->Unmap(_constantBuffer.Get(), 0);
}


void Game::Render()
{
	RenderBegin();

	// TODO
	// IA(Input Assembler) - VS(Vertex Shader) - RS - PS(Pixel Shader) - OM(Output)
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// ==========================================
		// IA단계

		// Vertex Buffer
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);

		// Index Buffer
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// ==========================================
		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
		_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

		// ==========================================
		// RS
		_deviceContext->RSSetState(_rasterizerState.Get());

		// ==========================================
		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
		_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		_deviceContext->PSSetSamplers(0, 1, _samplerState.GetAddressOf()); // sampler state
		
		// ==========================================
		// OM
		// _deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->OMSetBlendState(_blendState.Get(), nullptr, 0xFFFFFFFF);

		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	// "그림 그리는게 완료 되었다면 _renderTargetView에다가 그려줘~" 라고(후면 버퍼에게) 요청한다.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);	// OM

	// 초기값으로 밀어버리는 부분
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);

	// 화면의 크기 정보 기입
	_deviceContext->RSSetViewports(1, &_viewport); // RS
}

void Game::RenderEnd()
{
	// 아래의 함수가 굉장히 중요하다.
	// 후면, 전면 두개 스왑
	HRESULT hr = _swapChain->Present(1, 0); // 제출한다.
	C(hr);
}

// 800 x 600
void Game::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width; // 800
		desc.BufferDesc.Height = _height; // 600
		desc.BufferDesc.RefreshRate.Numerator = 60; // 화면 주사율
		desc.BufferDesc.RefreshRate.Denominator = 1; // 분모
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 각각 8비트
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 아래 두부분은 조금 중요하다.
		// Count : 멀티 셈플링과 관련된 문제이다.
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		// 아래 3개 중요하다. 
		// 버퍼 어떻게 사용할 것인가?
		// 아래의 매크로는 최종 결과물을 그리는데 사용하겠다.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1; // 후면 버퍼 개수
		desc.OutputWindow = _hWnd; // 어떤 윈도우 핸들 설정?
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr, 0,
		nullptr, 0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr, 
		_deviceContext.GetAddressOf()
	);

	C(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// 스왑 체인에서 후면 버퍼에 해당하는 버퍼를
	// ID3D11Texture2D라는 타입으로 반환해서 backBuffer에 할당
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	C(hr);

	// 스왑체인에서 가져온 backBuffer를 아래의 함수를 통해서 
	// _renderTargetView에 묘사하는 녀석을 넣어준 느낌.
	// 후면버퍼를 render target view로 발급 받음.

	// GPU는 _renderTargetView여기다가 그림을 그린다.
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	C(hr);

}

void Game::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

void Game::CreateGeometry()
{
	// Vertex Data_1
	{
		_vertices.resize(4);

		// 13
		// 02
		/*_vertices[0].position = Vec3(-0.5f, -0.5f, 0.0f);
		_vertices[0].color = Color(1.f, 0.f, 0.f, 1.0f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].color = Color(1.f, 0.f, 0.f, 1.0f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].color = Color(1.f, 0.f, 0.f, 1.0f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].color = Color(1.f, 0.f, 0.f, 1.0f);*/
	}

	// Vertex Data_2
	{
		// 13
		// 02
		_vertices[0].position = Vec3(-0.5f, -0.5f, 0.0f);
		_vertices[0].uv = Vec2(0.f, 5.f);

		_vertices[1].position = Vec3(-0.5f, 0.5f, 0.f);
		_vertices[1].uv = Vec2(0.f, 0.f);

		_vertices[2].position = Vec3(0.5f, -0.5f, 0.f);
		_vertices[2].uv = Vec2(5.f, 5.f);

		_vertices[3].position = Vec3(0.5f, 0.5f, 0.f);
		_vertices[3].uv = Vec2(5.f, 0.f);
	}

	// VertexBuffer
	{
		// 아래의 desc, data는 GPU쪽에 Buffer를 생성하는 작업이다.
		// CPU에 있던 데이터들 GPU로 복사한다.
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc)); // 자신이 없다면 먼저 0으로 셋팅
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());
		
		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		
		// GPU에게 건내준다.
		HRESULT hr = _device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
		C(hr);
	}

	// Index
	{
		_indices = {0, 1, 2, 2, 1, 3};
	}

	// IndexBuffer
	{
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc)); // 자신이 없다면 먼저 0으로 셋팅
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _indices.data();

		// GPU에게 건내준다.
		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		C(hr);
	};
}

// 데이터를 어떻게 읽어야 할지 알려달라. 그래서 GPU에게 알려주는 함수
void Game::CreateInputLayout()
{
	// Struct.h의 Vertex가 어떻게 되어 있는지 묘사하는 부분
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	_device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Game::CreateVS()
{
	// VS만들어 버림.
	LoadShaderFromFile(L"Default.hlsl", "VS", "vs_5_0", _vsBlob);

	HRESULT hr = _device->CreateVertexShader
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

	HRESULT hr = _device->CreatePixelShader
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

	H hr = _device->CreateRasterizerState(&desc, _rasterizerState.GetAddressOf());
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

	_device->CreateSamplerState(&desc, _samplerState.GetAddressOf());
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

	H hr = _device->CreateBlendState(&desc, _blendState.GetAddressOf());
	C(hr);
}

void Game::CreateSRV()
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	HRESULT hr = ::LoadFromWICFile(L"BFS.jpg", WIC_FLAGS_NONE, &md, img);
	C(hr);

	hr = ::CreateShaderResourceView
	(
		_device.Get(), img.GetImages(), img.GetImageCount(),
		md, _shaderResourceView.GetAddressOf()
	);
	C(hr);

	DirectX::TexMetadata md2;
	DirectX::ScratchImage img2;
	hr = ::LoadFromWICFile(L"Ral.jpg", WIC_FLAGS_NONE, &md2, img2);
	C(hr);

	hr = ::CreateShaderResourceView
	(
		_device.Get(), img2.GetImages(), img2.GetImageCount(),
		md2, _shaderResourceView2.GetAddressOf()
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
	
	HRESULT hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
	C(hr);
}

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// d3dcompiler.h에서
	HRESULT hr = ::D3DCompileFromFile
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

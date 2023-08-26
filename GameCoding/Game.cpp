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
	
	CreateGeometry();		// VertextBuffer GPU���� �ǳ���
	CreateVS();				// Vertex Shader ����
	CreateInputLayout();	// InputLayout ����
	CreatePS();				// Pixel Shader ����
	CreateSRV();			// Shader Resouce View�ε� �� ����
}

void Game::Update()
{

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
		// IA�ܰ�

		// Vertex Buffer
		_deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);

		// Index Buffer
		_deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		_deviceContext->IASetInputLayout(_inputLayout.Get());
		_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// ==========================================
		// VS
		_deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);

		// ==========================================
		// RS


		// ==========================================
		// PS
		_deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

		flag += 1;
		if (flag >= 100)
		{
			if (flag >= 200) flag = 0;

			_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView.GetAddressOf());
		}
		else if (flag < 100)
		{
			_deviceContext->PSSetShaderResources(0, 1, _shaderResourceView2.GetAddressOf());
		}

		// ==========================================
		// OM
		// _deviceContext->Draw(_vertices.size(), 0);
		_deviceContext->DrawIndexed(_indices.size(), 0, 0);
	}

	RenderEnd();
}

void Game::RenderBegin()
{
	// "�׸� �׸��°� �Ϸ� �Ǿ��ٸ� _renderTargetView���ٰ� �׷���~" ���(�ĸ� ���ۿ���) ��û�Ѵ�.
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);	// OM

	// �ʱⰪ���� �о������ �κ�
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);

	// ȭ���� ũ�� ���� ����
	_deviceContext->RSSetViewports(1, &_viewport); // RS
}

void Game::RenderEnd()
{
	// �Ʒ��� �Լ��� ������ �߿��ϴ�.
	// �ĸ�, ���� �ΰ� ����
	HRESULT hr = _swapChain->Present(1, 0); // �����Ѵ�.
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
		desc.BufferDesc.RefreshRate.Numerator = 60; // ȭ�� �ֻ���
		desc.BufferDesc.RefreshRate.Denominator = 1; // �и�
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ���� 8��Ʈ
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// �Ʒ� �κκ��� ���� �߿��ϴ�.
		// Count : ��Ƽ ���ø��� ���õ� �����̴�.
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;

		// �Ʒ� 3�� �߿��ϴ�. 
		// ���� ��� ����� ���ΰ�?
		// �Ʒ��� ��ũ�δ� ���� ������� �׸��µ� ����ϰڴ�.
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1; // �ĸ� ���� ����
		desc.OutputWindow = _hWnd; // � ������ �ڵ� ����?
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

	// ���� ü�ο��� �ĸ� ���ۿ� �ش��ϴ� ���۸�
	// ID3D11Texture2D��� Ÿ������ ��ȯ�ؼ� backBuffer�� �Ҵ�
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	C(hr);

	// ����ü�ο��� ������ backBuffer�� �Ʒ��� �Լ��� ���ؼ� 
	// _renderTargetView�� �����ϴ� �༮�� �־��� ����.
	// �ĸ���۸� render target view�� �߱� ����.

	// GPU�� _renderTargetView����ٰ� �׸��� �׸���.
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
		_vertices[0].position = Vec3(-1.f, -1.f, 0.0f);
		_vertices[0].uv = Vec2(0.f, 1.f);

		_vertices[1].position = Vec3(-1.f, 1.f, 0.f);
		_vertices[1].uv = Vec2(0.f, 0.f);

		_vertices[2].position = Vec3(1.f, -1.f, 0.f);
		_vertices[2].uv = Vec2(1.f, 1.f);

		_vertices[3].position = Vec3(1.f, 1.f, 0.f);
		_vertices[3].uv = Vec2(1.f, 0.f);
	}

	// VertexBuffer
	{
		// �Ʒ��� desc, data�� GPU�ʿ� Buffer�� �����ϴ� �۾��̴�.
		// CPU�� �ִ� �����͵� GPU�� �����Ѵ�.
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc)); // �ڽ��� ���ٸ� ���� 0���� ����
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());
		
		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _vertices.data();
		
		// GPU���� �ǳ��ش�.
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
		Z(&desc, sizeof(desc)); // �ڽ��� ���ٸ� ���� 0���� ����
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (uint32)(sizeof(uint32) * _indices.size());

		D3D11_SUBRESOURCE_DATA data;
		Z(&data, sizeof(data));
		data.pSysMem = _indices.data();

		// GPU���� �ǳ��ش�.
		HRESULT hr = _device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
		C(hr);
	};
}

// �����͸� ��� �о�� ���� �˷��޶�. �׷��� GPU���� �˷��ִ� �Լ�
void Game::CreateInputLayout()
{
	// Struct.h�� Vertex�� ��� �Ǿ� �ִ��� �����ϴ� �κ�
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
	// VS����� ����.
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
	// VS����� ������
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

void Game::LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	// d3dcompiler.h����
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

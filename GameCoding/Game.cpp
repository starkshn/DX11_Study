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
	
}

void Game::Update()
{

}

void Game::Render()
{
	RenderBegin();

	// TODO IA(Input Assembler) - VS(Vertex Shader) - RS - PS(Pixel Shader) - OM(Output)
	{

	}

	RenderEnd();
}

void Game::RenderBegin()
{
	// �ĸ� ���ۿ� �׷��޶�� ��û
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);	// OM
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);										// RS
}

void Game::RenderEnd()
{
	// �Ʒ��� �Լ��� ������ �߿��ϴ�.
	// �ĸ�, ���� �ΰ� ����
	HRESULT hr = _swapChain->Present(1, 0); // �����Ѵ�.
	CHECK(hr);
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

	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// ���� ü�ο��� �ĸ� ���ۿ� �ش��ϴ� ���۸�
	// ID3D11Texture2D��� Ÿ������ ��ȯ�ؼ� backBuffer�� �Ҵ�
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// �ĸ���۸� render target view�� �߱� ����.
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	CHECK(hr);

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

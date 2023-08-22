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
	// 후면 버퍼에 그려달라고 요청
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);	// OM
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);										// RS
}

void Game::RenderEnd()
{
	// 아래의 함수가 굉장히 중요하다.
	// 후면, 전면 두개 스왑
	HRESULT hr = _swapChain->Present(1, 0); // 제출한다.
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

	CHECK(hr);
}

void Game::CreateRenderTargetView()
{
	HRESULT hr;
	
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;

	// 스왑 체인에서 후면 버퍼에 해당하는 버퍼를
	// ID3D11Texture2D라는 타입으로 반환해서 backBuffer에 할당
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	// 후면버퍼를 render target view로 발급 받음.
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

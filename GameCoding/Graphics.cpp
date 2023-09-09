#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hWnd)
{
	_hWnd = hWnd;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

Graphics::~Graphics()
{

}

void Graphics::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void Graphics::RenderEnd()
{
	H hr = _swapChain->Present(1, 0);
	C(hr);
}

void Graphics::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	Z(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = GWinSizeX;						// 800
		desc.BufferDesc.Height = GWinSizeY;						// 600
		desc.BufferDesc.RefreshRate.Numerator = 60;				// 화면 주사율
		desc.BufferDesc.RefreshRate.Denominator = 1;			// 분모
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 각각 8비트
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
		desc.BufferCount = 1;				// 후면 버퍼 개수
		desc.OutputWindow = _hWnd;			// 어떤 윈도우 핸들 설정?
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	H hr = ::D3D11CreateDeviceAndSwapChain
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

// SwapChain에다가 버퍼를 두개를 요청을 해서 두개가 만들어 졌는데
// 만들어진 그 버퍼를 지칭하기 위해서 RenderTargetView라는 것을 만들어 놔야 했다.
// 이제 이 RenderTargetView를 제출해서 이곳에다가 그려달라고 할 것이다.
void Graphics::CreateRenderTargetView()
{
	H hr;
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	C(hr);
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	C(hr);
}

// Rasterizer단계에서 
void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(GWinSizeX);
	_viewport.Height = static_cast<float>(GWinSizeY);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

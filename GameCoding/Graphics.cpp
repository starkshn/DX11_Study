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
		desc.BufferDesc.RefreshRate.Numerator = 60;				// ȭ�� �ֻ���
		desc.BufferDesc.RefreshRate.Denominator = 1;			// �и�
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���� 8��Ʈ
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
		desc.BufferCount = 1;				// �ĸ� ���� ����
		desc.OutputWindow = _hWnd;			// � ������ �ڵ� ����?
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

// SwapChain���ٰ� ���۸� �ΰ��� ��û�� �ؼ� �ΰ��� ����� ���µ�
// ������� �� ���۸� ��Ī�ϱ� ���ؼ� RenderTargetView��� ���� ����� ���� �ߴ�.
// ���� �� RenderTargetView�� �����ؼ� �̰����ٰ� �׷��޶�� �� ���̴�.
void Graphics::CreateRenderTargetView()
{
	H hr;
	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	C(hr);
	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	C(hr);
}

// Rasterizer�ܰ迡�� 
void Graphics::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(GWinSizeX);
	_viewport.Height = static_cast<float>(GWinSizeY);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}

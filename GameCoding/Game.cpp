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
	
}

void Game::Update()
{

}

void Game::Render()
{

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

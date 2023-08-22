#pragma once
class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hWnd);
	void Update();
	void Render();

private:
	// ������ ������ ���ο��� �̷�� ���� �۾��� �� ���Լ����ٰ� ���ҽ����� ���� �־���Ѵ�.
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();

	// swap chain��(�ĸ� ���ۿ� �׸��� �۾� ��û)
	void CreateRenderTargetView();

	void SetViewport();

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	// DX
private:
	// ID3D �� COM ��ü�̴�. (�������� X) 
	// wrl.h �� ����(����)
	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// Render Traget View
	// �ĸ� ���۸� �����ϴ� �༮
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // �ĸ� ���ۿ� �׷��޶�� ��û�� ��ü

	// Misc (view port)
	// ȭ�� ����(����)�ϴ� ����ü
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // �ĸ� ���� �ʱ�ȭ ��

};

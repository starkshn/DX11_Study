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
	void CreateDeviceAndSwapChain();

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	// DX
private:
	// ID3D 는 COM 객체이다. (쌩포인터 X) 
	// wrl.h 에 정의(선언)
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
};

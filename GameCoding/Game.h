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
	// 렌더링 파이프 라인에서 이루어 지는 작업을 이 두함수에다가 리소스들을 묶어 주어야한다.
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();

	// swap chain에(후면 버퍼에 그리는 작업 요청)
	void CreateRenderTargetView();

	void SetViewport();

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	// DX
private:
	// ID3D 는 COM 객체이다. (쌩포인터 X) 
	// wrl.h 에 정의(선언)
	// Device & SwapChain
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

	// Render Traget View
	// 후면 버퍼를 묘사하는 녀석
	ComPtr<ID3D11RenderTargetView> _renderTargetView; // 후면 버퍼에 그려달라고 요청할 객체

	// Misc (view port)
	// 화면 구성(묘사)하는 구조체
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // 후면 버퍼 초기화 색

};

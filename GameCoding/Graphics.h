#pragma once
class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

public:
	ComPtr<ID3D11Device>		GetDevice() const { return _device; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return _deviceContext; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND	_hWnd;

private:
	// ID3D 는 COM 객체이다. (쌩포인터 X), wrl.h 에 정의(선언)
	ComPtr<ID3D11Device>			_device = nullptr;
	ComPtr<ID3D11DeviceContext>		_deviceContext = nullptr;
	ComPtr<IDXGISwapChain>			_swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView>	_renderTargetView = nullptr; // Render Traget View(후면 버퍼 묘사)

	// Misc (view port)
private:
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f }; // 후면 버퍼 초기화 색
};


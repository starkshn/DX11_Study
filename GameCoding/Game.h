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
	// View는 GPU에게 설명하기 위한 Tag느낌의 부연설명
	void CreateRenderTargetView(); // RTV
	void SetViewport();

private:
	void CreateGeometry();		// 기하 도형 생성
	void CreateInputLayout();	// 기하 도형의 설명서
	// InputLayout은 Vertex가 어떻게 생겼는지 묘사하는 녀석이다

	// GPU에게 동작해달라고 요청하는 부분
	void CreateVS(); // Create Vertex Shader
	void CreatePS(); // Create Pixel Shader

	// Shader Resouce View
	void CreateSRV();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND	_hWnd;
	uint32	_width = 0;
	uint32	_height = 0;

	uint32 flag = 0;

	// DX
private:
	// ID3D 는 COM 객체이다. (쌩포인터 X), wrl.h 에 정의(선언)
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr; // Render Traget View(후면 버퍼 묘사)

	// Misc (view port)
private:
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // 후면 버퍼 초기화 색

	// Geometry
private:
	// [CPU <-> RAM], [GPU <-> VRAM]
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	ComPtr<ID3D11InputLayout> _inputLayout = nullptr; // Vertex구조 묘사

	// Shader Load
	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr; // vertex shader
	ComPtr<ID3DBlob> _vsBlob = nullptr;					// blob

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;	// pixel shader
	ComPtr<ID3DBlob> _psBlob = nullptr;					// blob

	// SRV
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView = nullptr;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView2 = nullptr;
};

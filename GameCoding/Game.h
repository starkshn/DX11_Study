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
	// View�� GPU���� �����ϱ� ���� Tag������ �ο�����
	void CreateRenderTargetView(); // RTV
	void SetViewport();

private:
	void CreateGeometry();		// ���� ���� ����
	void CreateInputLayout();	// ���� ������ ����
	// InputLayout�� Vertex�� ��� ������� �����ϴ� �༮�̴�

	// GPU���� �����ش޶�� ��û�ϴ� �κ�
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
	// ID3D �� COM ��ü�̴�. (�������� X), wrl.h �� ����(����)
	ComPtr<ID3D11Device> _device = nullptr;
	ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> _renderTargetView = nullptr; // Render Traget View(�ĸ� ���� ����)

	// Misc (view port)
private:
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // �ĸ� ���� �ʱ�ȭ ��

	// Geometry
private:
	// [CPU <-> RAM], [GPU <-> VRAM]
	vector<Vertex> _vertices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;

	ComPtr<ID3D11InputLayout> _inputLayout = nullptr; // Vertex���� ����

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

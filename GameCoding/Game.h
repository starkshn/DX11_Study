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
	void CreateGeometry();		// ���� ���� ����
	void CreateInputLayout();	// ���� ������ ����
	// InputLayout�� Vertex�� ��� ������� �����ϴ� �༮�̴�

	// GPU���� �����ش޶�� ��û�ϴ� �κ�
	void CreateVS(); // Create Vertex Shader
	void CreatePS(); // Create Pixel Shader


	// ��������� �� �߿��� ���ѻ�
	void CreateRasterizerState();
	void CreateSamplerState();		// ���ø��� �Ѵ�.
	void CreateBlendState();

	// Shader Resouce View
	void CreateSRV();

	void CreateConstantBuffer();

	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);

private:
	HWND								_hWnd;

private:
	shared_ptr<Graphics>				_graphcis;

private:
	// Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;
	shared_ptr<InputLayout>					_inputLayout;

private:
	// VS
	ComPtr<ID3D11VertexShader>			_vertexShader = nullptr; 
	ComPtr<ID3DBlob>					_vsBlob = nullptr;				
	// RS
	ComPtr<ID3D11RasterizerState>		_rasterizerState = nullptr;

	// PS
	ComPtr<ID3D11PixelShader>			_pixelShader = nullptr;
	ComPtr<ID3DBlob>					_psBlob = nullptr;

	// SRV
	ComPtr<ID3D11ShaderResourceView>	_shaderResourceView = nullptr;
	ComPtr<ID3D11ShaderResourceView>	_shaderResourceView2 = nullptr;

private:
	// SRT�� transformData�� �־��ְ� �ȴ�.
	TransformData						_transformData;
	ComPtr<ID3D11Buffer>				_constantBuffer = nullptr;

private:
	ComPtr<ID3D11SamplerState>			_samplerState = nullptr;
	ComPtr<ID3D11BlendState>			_blendState = nullptr;

private:
	Vec3 _localPosition = {0.f, 0.f, 0.f};
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = {1.f, 1.f, 1.f};
};

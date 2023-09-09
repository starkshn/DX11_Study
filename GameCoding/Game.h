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
	void CreateGeometry();		// 기하 도형 생성
	void CreateInputLayout();	// 기하 도형의 설명서
	// InputLayout은 Vertex가 어떻게 생겼는지 묘사하는 녀석이다

	// GPU에게 동작해달라고 요청하는 부분
	void CreateVS(); // Create Vertex Shader
	void CreatePS(); // Create Pixel Shader


	// 상대적으로 덜 중요한 삼총사
	void CreateRasterizerState();
	void CreateSamplerState();		// 샘플링을 한다.
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
	// SRT를 transformData에 넣어주게 된다.
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

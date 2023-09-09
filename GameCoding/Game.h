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
	// »ó´ëÀûÀ¸·Î ´ú Áß¿äÇÑ »ïÃÑ»ç
	void CreateRasterizerState();
	void CreateSamplerState();		// »ùÇÃ¸µÀ» ÇÑ´Ù.
	void CreateBlendState();

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

	shared_ptr<VertexShader>				_vertexShader;
	ComPtr<ID3D11RasterizerState>			_rasterizerState;
	shared_ptr<PixelShader>					_pixelShader;

	shared_ptr<Texture>						_texture1;

private:
	TransformData								_transformData;
	shared_ptr<ConstantBuffer<TransformData>>	_constantBuffer;

private:
	ComPtr<ID3D11SamplerState>			_samplerState;
	ComPtr<ID3D11BlendState>			_blendState;

private:
	Vec3 _localPosition = {0.f, 0.f, 0.f};
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = {1.f, 1.f, 1.f};
};

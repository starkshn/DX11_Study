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
	HWND								_hWnd;

private:
	shared_ptr<Graphics>				_graphcis;
	shared_ptr<Pipeline>				_pipeline;

private:
	// Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry;
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;
	shared_ptr<InputLayout>					_inputLayout;

	shared_ptr<VertexShader>				_vertexShader;
	shared_ptr<RasterizerState>				_rasterizerState;
	shared_ptr<PixelShader>					_pixelShader;

	shared_ptr<Texture>						_texture1;

private:
	TransformData								_transformData;
	shared_ptr<ConstantBuffer<TransformData>>	_constantBuffer;

private:
	shared_ptr<SamplerState>				_samplerState;
	shared_ptr<BlendState>					_blendState;

private:
	Vec3 _localPosition = {0.f, 0.f, 0.f};
	Vec3 _localRotation = { 0.f, 0.f, 0.f };
	Vec3 _localScale = {1.f, 1.f, 1.f};
};

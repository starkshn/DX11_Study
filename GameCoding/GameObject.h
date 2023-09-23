#pragma once

class MonoBehavior;

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, bool option = false);
	~GameObject();

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FixedUpdate();

public:
	shared_ptr<Component> GetFixedComponent(ComponentType type);
	shared_ptr<Transform> GetTransform();

public:
	void AddComponent(shared_ptr<Component> comp);

	void Render(shared_ptr<Pipeline> pipeline);

private:
	ComPtr<ID3D11Device>		_device;
	ComPtr<ID3D11DeviceContext> _deviceContext;

private:
	bool _option;

	// Resouce
private:
	// Geometry
	shared_ptr<Geometry<VertexTextureData>> _geometry_V;
	shared_ptr<Geometry<VertexColorData>>	_geometry_C;
	shared_ptr<VertexBuffer>				_vertexBuffer;
	shared_ptr<IndexBuffer>					_indexBuffer;
	shared_ptr<InputLayout>					_inputLayout;

private:
	// Shader
	shared_ptr<VertexShader>				_vertexShader;
	shared_ptr<PixelShader>					_pixelShader;

private:
	// Rasterizer
	shared_ptr<RasterizerState>				_rasterizerState;

private:
	// SRT
	TransformData								_transformData;
	shared_ptr<ConstantBuffer<TransformData>>	_constantBuffer;

private:
	shared_ptr<Texture>						_texture1;
	shared_ptr<SamplerState>				_samplerState;
	shared_ptr<BlendState>					_blendState;

protected:
	array<shared_ptr<Component>, FIXED_COMPONENT_COUNT> _components;
	vector<shared_ptr<MonoBehavior>> _scripts;
};
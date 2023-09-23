#include "pch.h"
#include "GameObject.h"
#include "MonoBehavior.h"
#include "Transform.h"


GameObject::GameObject(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, bool option)
	:
	_device(device), _deviceContext(deviceContext)
{
	if (option)
	{
		_geometry_V = make_shared<Geometry<VertexTextureData>>();
		GeometryHelper::CreateRectangle(_geometry_V);

		_vertexBuffer = make_shared<VertexBuffer>(_device);
		_vertexBuffer->Create(_geometry_V->GetVertices());

		_indexBuffer = make_shared<IndexBuffer>(_device);
		_indexBuffer->Create(_geometry_V->GetIndices());

		_vertexShader = make_shared<VertexShader>(_device);
		_vertexShader->Create(L"Default.hlsl", "VS", "vs_5_0");

		_inputLayout = make_shared<InputLayout>(_device);
		_inputLayout->Create(VertexTextureData::descs, _vertexShader->GetBlob());

		_pixelShader = make_shared<PixelShader>(_device);
		_pixelShader->Create(L"Default.hlsl", "PS", "ps_5_0");

		_rasterizerState = make_shared<RasterizerState>(_device);
		_rasterizerState->Create();

		_blendState = make_shared<BlendState>(_device);
		_blendState->Create();

		_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
		_constantBuffer->Create();

		_texture1 = make_shared<Texture>(_device);
		_texture1->Create(L"BFS.jpg");

		_samplerState = make_shared<SamplerState>(_device);
		_samplerState->Create();
	}
	else
	{
		_geometry_C = make_shared<Geometry<VertexColorData>>();
		GeometryHelper::CreateRectangle(_geometry_C, Color(1.f, 0.0f, 0.0f, 1.f));

		_vertexBuffer = make_shared<VertexBuffer>(_device);
		_vertexBuffer->Create(_geometry_C->GetVertices());

		_indexBuffer = make_shared<IndexBuffer>(_device);
		_indexBuffer->Create(_geometry_C->GetIndices());

		_vertexShader = make_shared<VertexShader>(_device);
		_vertexShader->Create(L"Color.hlsl", "VS", "vs_5_0");

		_inputLayout = make_shared<InputLayout>(_device);
		_inputLayout->Create(VertexColorData::descs, _vertexShader->GetBlob());

		_pixelShader = make_shared<PixelShader>(_device);
		_pixelShader->Create(L"Color.hlsl", "PS", "ps_5_0");

		_rasterizerState = make_shared<RasterizerState>(_device);
		_rasterizerState->Create();

		_blendState = make_shared<BlendState>(_device);
		_blendState->Create();

		_constantBuffer = make_shared<ConstantBuffer<TransformData>>(_device, _deviceContext);
		_constantBuffer->Create();

		 _texture1 = make_shared<Texture>(_device);
		 _texture1->Create(L"BFS.jpg");

		_samplerState = make_shared<SamplerState>(_device);
		_samplerState->Create();
	}

	{
		
	}
	_option = option;
}

GameObject::~GameObject()
{
}

void GameObject::Awake()
{
	for (shared_ptr<Component>& component : _components)
	{
		component->Awake();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Awake();
	}
}

void GameObject::Start()
{
	for (shared_ptr<Component>& component : _components)
	{
		component->Start();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Start();
	}
}

void GameObject::Update()
{
	for (shared_ptr<Component>& component : _components)
	{
		component->Update();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->Update();
	}

	_transformData.matWorld = _transform->GetWorldMatrix();

	// CPU -> GPU로의 데이터 복사
	_constantBuffer->CopyData(_transformData);
}

void GameObject::LateUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		component->LateUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->LateUpdate();
	}
}

void GameObject::FixedUpdate()
{
	for (shared_ptr<Component>& component : _components)
	{
		component->FixedUpdate();
	}

	for (shared_ptr<MonoBehavior>& script : _scripts)
	{
		script->FixedUpdate();
	}
}

shared_ptr<Component> GameObject::GetFixedComponent(ComponentType type)
{
	uint8 index = static_cast<uint8>(type);
	assert(index < FIXED_COMPONENT_COUNT);
	return _components[index];
}

shared_ptr<Transform> GameObject::GetTransform()
{
	shared_ptr<Component> comp = GetFixedComponent(ComponentType::Transform);
	return static_pointer_cast<Transform>(comp);
}

void GameObject::AddComponent(shared_ptr<Component> comp)
{
	comp->SetGameObject(shared_from_this());
}

void GameObject::Render(shared_ptr<Pipeline> pipeline)
{
	PipelineInfo info;
	info.inputLayout		= _inputLayout;
	info.vertexShader		= _vertexShader;
	info.pixelShader		= _pixelShader;
	info.rasterizerState	= _rasterizerState;
	info.blendState			= _blendState;
	info.topology			= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	pipeline->UpdatePipeline(info);

	// IA
	pipeline->SetVertexBuffer(_vertexBuffer);
	pipeline->SetIndexBuffer(_indexBuffer);

	// VS
	pipeline->SetConstantBuffer(0, SS_VertexShader, _constantBuffer);

	// RS

	// PS
	pipeline->SetTexture(0, SS_PixelShader, _texture1);
	pipeline->SetSamplerState(0, SS_PixelShader, _samplerState);

	// OM
	if (_option)
	{
		pipeline->DrawIndexed(_geometry_V->GetIndexCount(), 0, 0);
	}
	else
	{
		pipeline->DrawIndexed(_geometry_C->GetIndexCount(), 0, 0);
	}
}

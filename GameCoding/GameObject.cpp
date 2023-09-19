#include "pch.h"
#include "GameObject.h"

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
		// Test
		//_parent->AddChild(_transform);
		//_transform->SetParent(_parent);
	}
	_option = option;
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	/*Vec3 pos = _parent->GetPosition();
	pos.x += 0.001f;
	_parent->SetPosition(pos);*/

	/*Vec3 pos = _transform->GetPosition();
	pos.x += 0.001f;
	_transform->SetPosition(pos);*/
	
	Vec3 rot = _transform->GetRotation();
	rot.z += 0.01f;
	_transform->SetRotation(rot);

	_transformData.matWorld = _transform->GetWorldMatrix();

	// CPU -> GPU로의 데이터 복사
	_constantBuffer->CopyData(_transformData);
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

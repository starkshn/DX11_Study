#pragma once

struct PipelineInfo
{
	shared_ptr<InputLayout>		inputLayout;
	shared_ptr<VertexShader>	vertexShader;
	shared_ptr<PixelShader>		pixelShader;
	shared_ptr<RasterizerState> rasterizerState;
	shared_ptr<BlendState>		blendState;
	D3D11_PRIMITIVE_TOPOLOGY	topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class Pipeline
{
public:
	Pipeline(ComPtr<ID3D11DeviceContext> deviceContext);
	~Pipeline();

public:
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return _deviceContext; }
	
public:
	void UpdatePipeline(PipelineInfo info);

public:
	void SetVertexBuffer(shared_ptr<VertexBuffer> buffer);
	void SetIndexBuffer(shared_ptr<IndexBuffer> buffer);

	template <typename T>
	void SetConstantBuffer(uint32 slot, ShaderScope scope, shared_ptr<ConstantBuffer<T>> buffer)
	{
		if (scope & SS_VertexShader)
		{
			_deviceContext->VSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
		}
		else if (scope & SS_PixelShader)
		{
			_deviceContext->PSSetConstantBuffers(slot, 1, buffer->GetComPtr().GetAddressOf());
		}
		else if (scope & SS_Both)
		{

		}
	}

	void SetTexture(uint32 slot, ShaderScope scope, shared_ptr<Texture> texture);
	void SetSamplerState(uint32 slot, ShaderScope scope, shared_ptr<SamplerState> samplerState);

public:
	void Draw(uint32 vertexCount, uint32 startVertexLoation);
	void DrawIndexed(uint32 indexCount, uint32 startIndexLoation, uint32 baseVertexLocation);

private:
	ComPtr<ID3D11DeviceContext>	_deviceContext;
};


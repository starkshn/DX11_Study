#pragma once

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
		:
		_device(device), _deviceContext(deviceContext)
	{

	}
	~ConstantBuffer() {}

public:
	void Create()
	{
		D3D11_BUFFER_DESC desc;
		Z(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU_Write+ GPU_Read
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(T);
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		H hr = _device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
		C(hr);
	}

public:
	void CopyData(const T& data)
	{
		D3D11_MAPPED_SUBRESOURCE subResouce;
		Z(&subResouce, sizeof(subResouce));

		_deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce); // open
		::memcpy(subResouce.pData, &data, sizeof(data)); // CPU -> GPU로 데이터가 복사
		_deviceContext->Unmap(_constantBuffer.Get(), 0); // close
	}

public:
	ComPtr<ID3D11Buffer> GetComPtr() { return _constantBuffer; }

private:
	ComPtr<ID3D11Device>		_device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<ID3D11Buffer>		_constantBuffer;
};	


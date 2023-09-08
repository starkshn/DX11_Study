#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout(ComPtr<ID3D11Device> device)
	: _device(device)
{

}

InputLayout::~InputLayout()
{

}

void InputLayout::CraeteInputLayout(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
{
	const int32 count = static_cast<uint32>(descs.size());
	_device->CreateInputLayout(descs.data(), descs.size(), blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
}

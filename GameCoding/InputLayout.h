#pragma once
class InputLayout
{
public:
	InputLayout(ComPtr<ID3D11Device> device);
	~InputLayout();

public:
	ComPtr<ID3D11InputLayout> GetComPtr() const { return _inputLayout; }

public:
	void Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob);

private:
	ComPtr<ID3D11Device>				_device			= nullptr;
	ComPtr<ID3D11InputLayout>			_inputLayout	= nullptr; // Vertex±¸Á¶ ¹¦»ç
};


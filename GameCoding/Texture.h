#pragma once
class Texture
{
public:
	Texture(ComPtr<ID3D11Device> device);
	~Texture();

public:
	void Create(const wstring& path);

public:
	ComPtr<ID3D11ShaderResourceView> GetComPtr() { return _shaderResourceView; }

private:
	ComPtr<ID3D11Device>				_device;
	ComPtr<ID3D11ShaderResourceView>	_shaderResourceView;
};


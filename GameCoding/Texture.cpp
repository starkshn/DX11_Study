#include "pch.h"
#include "Texture.h"

Texture::Texture(ComPtr<ID3D11Device> device)
	: _device(device)
{
}

Texture::~Texture()
{
}

void Texture::Create(const wstring& path)
{
	DirectX::TexMetadata md;
	DirectX::ScratchImage img;
	H hr = ::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, &md, img);
	C(hr);

	hr = ::CreateShaderResourceView
	(
		_device.Get(), img.GetImages(), img.GetImageCount(),
		md, _shaderResourceView.GetAddressOf()
	);
	C(hr);
}

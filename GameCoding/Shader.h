#pragma once

enum ShaderScope : uint32
{
	SS_None = 0,
	SS_VertexShader = (1 << 0),
	SS_PixelShader = (1 << 1),
	SS_Both = SS_VertexShader | SS_PixelShader,
};


class Shader
{
public:
	Shader(ComPtr<ID3D11Device> device);
	virtual ~Shader();

public:
	ComPtr<ID3DBlob> GetBlob() { return _blob; }
	virtual void Create(const wstring& path, const string& name, const string& version) abstract;

protected:
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version);

protected:
	ComPtr<ID3D11Device>	_device;
	ComPtr<ID3DBlob>		_blob;

	wstring		_path;
	string		_name;
};

class VertexShader : public Shader
{
	using Super = Shader;
public:
	VertexShader(ComPtr<ID3D11Device> device);
	virtual ~VertexShader() override;

public:
	ComPtr<ID3D11VertexShader> GetComPtr() { return _vertexShader; }

public:
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11VertexShader>		_vertexShader = nullptr;
};


class PixelShader : public Shader
{
	using Super = Shader;
public:
	PixelShader(ComPtr<ID3D11Device> device);
	virtual ~PixelShader() override;

public:
	ComPtr<ID3D11PixelShader> GetComPtr() { return _pixelShader; }

public:
	virtual void Create(const wstring& path, const string& name, const string& version) override;

protected:
	ComPtr<ID3D11PixelShader>		_pixelShader = nullptr;
};


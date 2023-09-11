#pragma once
class RasterizerState
{
public:
	RasterizerState(ComPtr<ID3D11Device> device);
	~RasterizerState();

public:
	void Create();

public:
	ComPtr<ID3D11RasterizerState> GetComPtr() { return _rasterizerState; }

private:
	ComPtr<ID3D11Device>			_device;
	ComPtr<ID3D11RasterizerState>	_rasterizerState;
};


#pragma once

struct VertexTextureData
{
	Vec3 position	= { 0.f, 0.f, 0.f };
	Vec2 uv			= { 0.f, 0.f };

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};

struct VertexColorData
{
	Vec3 position	= { 0.f, 0.f, 0.f };
	Color color		= {0.f, 0.f, 0.f, 0.f};

	static vector<D3D11_INPUT_ELEMENT_DESC> descs;
};


#include "pch.h"
#include "GeometryHelper.h"

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexTextureData>> geometry)
{
	// Vertex
	// 13
	// 02
	vector<VertexTextureData> verticise;
	verticise.resize(4);

	verticise[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	verticise[0].uv = {0.f, 1.f};
	verticise[1].position = Vec3(-0.5f, 0.5f, 0.f);
	verticise[1].uv = {0.f, 0.f};
	verticise[2].position = Vec3(0.5f, -0.5f, 0.f);
	verticise[2].uv = {1.f, 1.f};
	verticise[3].position = Vec3(0.5f, 0.5f, 0.f);
	verticise[3].uv = {1.f, 0.f};
	geometry->SetVertices(verticise);

	// Index
	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}

void GeometryHelper::CreateRectangle(shared_ptr<Geometry<VertexColorData>> geometry, Color color)
{
	// Vertex
	vector<VertexColorData> verticise;
	verticise.resize(4);

	verticise[0].position = Vec3(-0.5f, -0.5f, 0.0f);
	verticise[0].color = color;
	verticise[1].position = Vec3(-0.5f, 0.5f, 0.f);
	verticise[1].color = color;
	verticise[2].position = Vec3(0.5f, -0.5f, 0.f);
	verticise[2].color = color;
	verticise[3].position = Vec3(0.5f, 0.5f, 0.f);
	verticise[3].color = color;
	geometry->SetVertices(verticise);

	// Index
	vector<uint32> indices = { 0, 1, 2, 2, 1, 3 };
	geometry->SetIndices(indices);
}

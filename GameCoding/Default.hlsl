
struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv       : TEXCOORD;
};;

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv       : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
}

// IA - VS - RS - PS - OM
// 이 단계가 Vertex Shader 단계 진입점이다.
// 아래의 함수는 '정점' 단위로 실행된다.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // WVP
    float4 position = mul(input.position, matWorld);
    position = mul(position, matView);
    position = mul(position, matProjection);
    
    output.position = position;
    output.uv = input.uv;
    
    return output;
}



Texture2D texture0 : register(t0);
// Texture2D texture1 : register(t1);

SamplerState sampler0 : register(s0); // 어떻게 가져올지에 대한 규약
// SamplerState sampler1 : register(s1); // 어떻게 가져올지에 대한 규약

// 좀졍과 과련된 색상 처리하는 부분
float4 PS(VS_OUTPUT input) : SV_Target
{   
    float4 color = texture0.Sample(sampler0, input.uv);
   //  float4 color2 = texture1.Sample(sampler1, input.uv);
    
    return color;
}
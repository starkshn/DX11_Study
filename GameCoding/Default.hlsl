
struct VS_INPUT
{
    float4 position : POSITION;
    // float4 color : COLOR;
    float2 uv : TEXCOORD;
};;

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    // float4 color : COLOR;
    float2 uv : TEXCOORD;
};

// IA - VS - RS - PS - OM
// �� �ܰ谡 Vertex Shader �ܰ� �������̴�.
// �Ʒ��� �Լ��� '����' ������ ����ȴ�.
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.uv = input.uv;
    
    return output;
}

Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);

SamplerState sampler0 : register(s0); // ��� ���������� ���� �Ծ�
SamplerState sampler1 : register(s1); // ��� ���������� ���� �Ծ�

// ������ ���õ� ���� ó���ϴ� �κ�
float4 PS(VS_OUTPUT input) : SV_Target
{   
    float4 color = texture0.Sample(sampler0, input.uv);
    float4 color2 = texture1.Sample(sampler1, input.uv);
    
    return color;
}
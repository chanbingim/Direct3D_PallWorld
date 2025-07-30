cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

Texture2D BaseTex : register(t0);
sampler Sampler1 = sampler_state
{
    Texture = <BaseTex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};


struct VertexInd
{
    float3 iPosL : POSITION;
    float2 uv : TEXCOORD0;
};

struct VertexOut
{
    float4 oPosH : SV_POSITION;
    float2 uv    : TEXCOORD0;
};

VertexOut VS(VertexInd vIn)
{
    VertexOut outVtx;
    
    outVtx.oPosH = float4(vIn.iPosL, 1.0f);
    //mul(float4(vIn.iPosL, 1.0f), gWorldViewProj);
    outVtx.uv = vIn.uv;
    
    return outVtx;
}

float4 PS(VertexOut input) : SV_Target
{
    //return float4(1.f, 0.f, 0.f, 1.f);
    return BaseTex.Sample(Sampler1, input.uv);
}
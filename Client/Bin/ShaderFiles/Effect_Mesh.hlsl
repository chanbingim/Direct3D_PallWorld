#include "DefualtStates.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D       g_DiffuseTexture;
Texture2D       g_NormalTexture;
Texture2D       g_NoiseTexture;
Texture2D       g_MaskTexture;

vector          g_vColor;
bool            g_bReverse;
int             g_DistionType;
int             g_MaskType;

float           g_fLifeTime;
float           g_fLifeAccTime;
float           g_fNoiseStLength;
float2          g_fDistotionAccTime;

/* 정점 쉐이더 : */
/* 정점에 대한 셰이딩 == 정점에 필요한 연산을 수행한다 == 정점의 상태변환(월드, 뷰, 투영) + 추가변환 */
/* 정점의 구성 정보를 수정, 변경한다 */ 
struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBINormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    //float3 vTangent : TANGENT;
    //float3 vBINormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    //float4 vWorldPos : TEXCOORD1;
    //float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_Default(VS_IN In)
{
    VS_OUT Out;
    
    /* In.vPosition * 월드 * 뷰 * 투영 */    
    //float4x4 == matrix
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
        
    if (g_bReverse)
        Out.vTexcoord = -1 * In.vTexcoord;
    else
        Out.vTexcoord = In.vTexcoord;
  
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    //Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), g_WorldMatrix)).xyz;
    //Out.vBINormal = normalize(mul(vector(In.vBINormal, 0.f), g_WorldMatrix)).xyz;
    //Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    //Out.vProjPos = Out.vPosition;
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    //float3 vTangent : TANGENT;
    //float3 vBINormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    //float4 vWorldPos : TEXCOORD1;
    //float4 vProjPos : TEXCOORD2;
};

struct PS_ModelCreateIn
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float3 vLocalPosition : TEXCOORD1;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    //float4 vNormal : SV_TARGET1;
    //float4 vDepth : SV_TARGET3;
};

PS_OUT PS_Default(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    float2 vTexcoord = In.vTexcoord;
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, vTexcoord).r;
    if (vMtrlDiffuse.a * fMask < 0.3)
        discard;
    
    switch(g_MaskType)
    {
        case 0 :
            Out.vDiffuse = (vMtrlDiffuse + g_vColor) * fMask;
        break;
        case 1:
            Out.vDiffuse = (vMtrlDiffuse * g_vColor) * fMask;
        break;

    }
  
    return Out;
}


PS_OUT PS_Distotion(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord + g_fDistotionAccTime / g_fLifeTime;
    vector vNoiseTexture = g_NoiseTexture.Sample(DefaultSampler, vTexcoord);
    
    switch (g_DistionType)
    {
        // 극좌표 왜곡
        case 1:
        {
            vTexcoord = vNoiseTexture.rg - 0.5f * g_fNoiseStLength;
            vTexcoord += In.vTexcoord;
        }
            break;
        
    }
        
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexcoord);
    float fMask = g_MaskTexture.Sample(DefaultSampler, vTexcoord).r;
    
    if (vMtrlDiffuse.a * fMask < 0.3)
        discard;
    
    switch (g_MaskType)
    {
        case 0:
            Out.vDiffuse = (vMtrlDiffuse + g_vColor) * fMask;
            break;
        case 1:
            Out.vDiffuse = (vMtrlDiffuse * g_vColor) * fMask;
            break;
    }
    
    return Out;
}

technique11 Tech
{
    pass Additive
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Default();
    }

    pass Distotion
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Distotion();
    }
}
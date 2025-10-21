#include "DefualtStates.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D   g_DiffuseTexture;
Texture2D   g_NormalTexture;
Texture2D   g_NoiseTexture;
Texture2D   g_MaskTexture;

vector      g_vColor;
float       g_fLifeAccTime;
float2      g_fDistotionAccTime;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
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
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

/* 출력된 정점 위치벡터의 w값으로 모든 성분을 나눈다 -> 투영스페이스로 변환 */ 
/* 정점의 위치에 대해서 뷰포트 변환을 수행한다 */ 
/* 정점의 모든 정보를 보간하여 픽셀을 만든다. -> 래스터라이즈 */ 

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};



/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_Default(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexCoord = In.vTexcoord + g_fDistotionAccTime;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexCoord);
    vector vMaskTexture = g_MaskTexture.Sample(DefaultSampler, vTexCoord);
    
    vMtrlDiffuse.a *= vMaskTexture.r;
    vector vColor = (vMtrlDiffuse + g_vColor) * vMaskTexture;
    if (vColor.a < 0.3f)
        discard;
    
    Out.vColor = vColor;
    return Out;
}


technique11 Tech
{
    pass Default
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Default();
    }

    pass AlphaBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Default();
    }
}
#include "DefualtStates.hlsli"
#include "EffectDefualtHeader.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_NoiseTexture;
Texture2D g_MaskTexture;
Texture2D g_DissolveTexture;

vector g_vColor;
bool g_bReverse;
int g_DistionType;

bool g_bIsLerp;
bool g_bIsDissolve;
float g_fDissolveTime;

int g_AlphaLerpType;
int g_MaskType;
int g_MaskMixType;

float g_fLifeTime;
float2 g_vSlice;
float g_fLifeAccTime;
float g_fNoiseStLength;
float2 g_fDistotionAccTime;

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
        
    if (g_bReverse)
        Out.vTexcoord = -1 * In.vTexcoord;
    else
        Out.vTexcoord = In.vTexcoord;
  
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix)).xyz;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
};

PS_OUT PS_Default(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, 1);
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord).r;
    vMtrlDiffuse.a *= fMask;
        
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fDissolveTime);
        vector vDissolve = DissolveFunc(g_DissolveTexture, ClampSampler, In.vTexcoord, Ratio);
       // vMtrlDiffuse.a *= vDissolve.a;
    }
    else
    {
        if (g_bIsLerp)
            vMtrlDiffuse.a = ComputeAlpha(vMtrlDiffuse, g_fLifeAccTime / g_fLifeTime, g_AlphaLerpType, In.vTexcoord);
    }
    
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}


PS_OUT PS_Distotion(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord + g_fDistotionAccTime / g_fLifeTime;
    vector vNoiseTexture = g_NoiseTexture.Sample(DefaultSampler, vTexcoord);
    
    vTexcoord = ComputeUV(vNoiseTexture.rg, true, 0, 0);
    vTexcoord = In.vTexcoord + vTexcoord * g_fNoiseStLength + g_fDistotionAccTime / g_fLifeTime;;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, 1);
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, vTexcoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fDissolveTime);
        vector vDissolve = DissolveFunc(g_DissolveTexture, ClampSampler, In.vTexcoord, Ratio);
        //vMtrlDiffuse.a *= vDissolve.a;
    }
    else
    {
        if (g_bIsLerp)
            vMtrlDiffuse.a = ComputeAlpha(vMtrlDiffuse, g_fLifeAccTime / g_fLifeTime, g_AlphaLerpType, vTexcoord);
    }
    
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

PS_OUT PS_SpriteDefault(PS_IN In)
{
    PS_OUT Out;
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, g_MaskType);
    
    float2 vMaskTexCoord = In.vTexcoord;
    if (1 == g_MaskType)
    {
        float2 vSliceIndex = { 1 / g_vSlice.x, 1 / g_vSlice.y };
        vMaskTexCoord = SliceUV(vMaskTexCoord, g_fLifeAccTime / g_fLifeTime, vSliceIndex);
    }
   
    float fMask = g_MaskTexture.Sample(DefaultSampler, vMaskTexCoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fDissolveTime);
        vector vDissolve = DissolveFunc(g_DissolveTexture, ClampSampler, In.vTexcoord, Ratio);
        //vMtrlDiffuse.a *= vDissolve.a;
    }
    else
    {
        if (g_bIsLerp)
            vMtrlDiffuse.a = ComputeAlpha(vMtrlDiffuse, g_fLifeAccTime / g_fLifeTime, g_AlphaLerpType, In.vTexcoord);
    }
  
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

PS_OUT PS_SpriteDistotion(PS_IN In)
{
    PS_OUT Out;
    float2 vTexcoord = In.vTexcoord + g_fDistotionAccTime / g_fLifeTime;
    
    vector vNoiseTexture = g_NoiseTexture.Sample(DefaultSampler, vTexcoord);
    vTexcoord = ComputeUV(vNoiseTexture.rg, true, 0, 0) * g_fNoiseStLength;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, g_MaskType);
    
    float2 vMaskTexCoord = vTexcoord;
    if (1 == g_MaskType)
    {
        float2 vSliceIndex = { 1 / g_vSlice.x, 1 / g_vSlice.y };
        vMaskTexCoord = SliceUV(vMaskTexCoord, g_fLifeAccTime / g_fLifeTime, vSliceIndex);
    }
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, vTexcoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fDissolveTime);
        vector vDissolve = DissolveFunc(g_DissolveTexture, ClampSampler, In.vTexcoord, Ratio);
        //vMtrlDiffuse.a *= vDissolve.a;
    }
    else
    {
        if (g_bIsLerp)
            vMtrlDiffuse.a = ComputeAlpha(vMtrlDiffuse, g_fLifeAccTime / g_fLifeTime, g_AlphaLerpType, vTexcoord);
    }
   
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

technique11 Tech
{
    pass Additive
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Default();
    }

    pass Distotion
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Distotion();
    }

    pass SpriteAddtive
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SpriteDefault();
    }

    pass SpriteDistotion
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SpriteDistotion();
    }
}
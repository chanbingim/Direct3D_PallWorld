#include "DefualtStates.hlsli"
#include "EffectDefualtHeader.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D       g_DiffuseTexture;
Texture2D       g_NormalTexture;
Texture2D       g_NoiseTexture;
Texture2D       g_MaskTexture;
Texture2D       g_DissolveTexture;

vector          g_vColor;
bool            g_bReverse;
int             g_DistionType;

bool            g_bIsLerp;
bool            g_bIsDissolve;
float           g_fDissolveTime;

int             g_AlphaLerpType;
int             g_MaskType;
int             g_MaskMixType;

float           g_fLifeTime;
float2          g_vSlice;
float           g_fLifeAccTime;
float           g_fNoiseStLength;
float2          g_fDistotionAccTime;

struct VS_IN
{
    float3 vPosition : POSITION;
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
    float4 vDiffuse : SV_TARGET0;
};

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_Default(PS_IN In)
{
    PS_OUT Out;
    float4 vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, 1);
    
   float fMask = g_MaskTexture.Sample(DefaultSampler, In.vTexcoord).r;
   vMtrlDiffuse.a *= fMask;
   
    if(g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fDissolveTime);
        vector vDissolve = DissolveFunc(g_DissolveTexture, ClampSampler, In.vTexcoord, Ratio);
        
        // 효과가 다른 디졸브 연산
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

PS_OUT PS_Distotion(PS_IN In)
{
    PS_OUT Out;
    
    float2 vTexcoord = In.vTexcoord + g_fDistotionAccTime / g_fLifeTime;
    
    vector vNoiseTexture = g_NoiseTexture.Sample(DefaultSampler, vTexcoord);
    vTexcoord = ComputeUV(vNoiseTexture.rg, true, 0, 0) * g_fNoiseStLength;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, 1);
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, vTexcoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fLifeTime);
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

//=========================================
//   _____ _____  _____ _____ _ _______   |
//  / ____|  __ \|  __ \_   _| |__   __|  |
// | (___ | |__) | |__) || | | |  | |     |
//  \___ \|  ___/|  ___/ | | | |  | |     |
//  ____) | |    | |    _| |_| |  | |     |
// |_____/|_|    |_|   |_____|_|  |_|     |
//=========================================
//      S P R I T E   E F F E C T         |
//=========================================

PS_OUT PS_SpriteDefault(PS_IN In)
{
    PS_OUT Out;
    
    float2 vMaskTexCoord = In.vTexcoord;
    if(1 == g_MaskType)
    {
        vMaskTexCoord = SliceUV(vMaskTexCoord, g_fLifeAccTime / g_fLifeTime, g_vSlice);
    }
   
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(ClampSampler, vMaskTexCoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, 1);
    
    float fMask = g_MaskTexture.Sample(DefaultSampler, vMaskTexCoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fLifeTime);
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
    
    float2 vMaskTexCoord = vTexcoord;
    if (1 == g_MaskType)
    {
        float2 vSliceIndex = { 1 / g_vSlice.x, 1 / g_vSlice.y };
        vMaskTexCoord = SliceUV(vMaskTexCoord, g_fLifeAccTime / g_fLifeTime, vSliceIndex);
    }
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, vTexcoord);
    vMtrlDiffuse = ComputeColor(vMtrlDiffuse, g_vColor, g_MaskType);
    
    float fMask = g_MaskTexture.Sample(ClampSampler, vTexcoord).r;
    vMtrlDiffuse.a *= fMask;
    
    if (g_bIsDissolve)
    {
        float Ratio = saturate(g_fLifeAccTime / g_fLifeTime);
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
        SetRasterizerState(RS_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Default();
    }

    pass Distotion
    {
        SetRasterizerState(RS_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_Distotion();
    }

    pass SpriteAddtive
    {
        SetRasterizerState(RS_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SpriteDefault();
    }

    pass SpriteDistotion
    {
        SetRasterizerState(RS_None);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_Default();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SpriteDistotion();
    }
}
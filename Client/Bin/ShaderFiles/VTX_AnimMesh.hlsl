#include "DefualtStates.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix g_BoneMatrices[512];

float     g_fCamFar;
Texture2D g_DiffuseTexture;
Texture2D g_NormalTexture;
Texture2D g_SpecularTexture;

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
    
    uint4 vBlendIndex : BLENDINDEX;
    float4 vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBINormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    float fBlendWight = 1 - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
                        g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
                        g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
                        g_BoneMatrices[In.vBlendIndex.w] * fBlendWight;
   
     /* 스키닝 */
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    
    /* In.vPosition * 월드 * 뷰 * 투영 */    
    //float4x4 == matrix
    matrix matWV, matWVP;
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), BoneMatrix * g_WorldMatrix)).xyz;
    Out.vTangent = normalize(mul(vector(In.vTangent, 0.f), BoneMatrix * g_WorldMatrix)).xyz;
    Out.vBINormal = normalize(mul(vector(In.vBINormal, 0.f), BoneMatrix * g_WorldMatrix)).xyz;
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(vPosition, g_WorldMatrix);
    Out.vProjPos = Out.vPosition;
    return Out;
}

/* 출력된 정점 위치벡터의 w값으로 모든 성분을 나눈다 -> 투영스페이스로 변환 */ 
/* 정점의 위치에 대해서 뷰포트 변환을 수행한다 */ 
/* 정점의 모든 정보를 보간하여 픽셀을 만든다. -> 래스터라이즈 */ 

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float3 vNormal : NORMAL;
    float3 vTangent : TANGENT;
    float3 vBINormal : BINORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vMtrlDiffuse.a < 0.4f)
        discard;
    
    vector vNoramlTexture = g_NormalTexture.Sample(MirrorSampler, In.vTexcoord);
    row_major float3x3 TangentSpaceMat = float3x3(In.vTangent, In.vBINormal * -1.f, In.vNormal);
    float3 vNormal = mul(vNoramlTexture.xyz * 2.f -1.f, TangentSpaceMat);
    
    Out.vColor = vMtrlDiffuse;
    Out.vNormal = float4(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = float4(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_fCamFar, 0.0f, 0.0f);
    
    return Out;
}

// Shadow

struct VS_OUT_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};


VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN In)
{
    VS_OUT_SHADOW Out;
    
    float fBlendWight = 1 - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);
    matrix BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
                        g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
                        g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
                        g_BoneMatrices[In.vBlendIndex.w] * fBlendWight;
   
     /* 스키닝 */
    vector vPosition = mul(vector(In.vPosition, 1.f), BoneMatrix);
    
    /* In.vPosition * 월드 * 뷰 * 투영 */    
    //float4x4 == matrix
    matrix matWV, matWVP;
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vPosition, matWVP);
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_SHADOW
{
    float4 vPosition : SV_POSITION;
    float4 vProjPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
    float4 vShadowLightDepth : SV_TARGET0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
    PS_OUT_SHADOW Out = (PS_OUT_SHADOW) 0;
    
    Out.vShadowLightDepth.x = In.vProjPos.w / g_fCamFar;
    
    return Out;
}

technique11 Tech
{
    pass Pass0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Shadow
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_SHADOW();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_SHADOW();
    }
}
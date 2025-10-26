#include "DefualtStates.hlsli"

matrix      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D   g_DiffuseTexture;
vector      g_vCamPosition;
float       g_vMaxLifeTime;

/* 정점 쉐이더 : */
/* 정점에 대한 셰이딩 == 정점에 필요한 연산을 수행한다 == 정점의 상태변환(월드, 뷰, 투영) + 추가변환 */
/* 정점의 구성 정보를 수정, 변경한다 */ 
struct VS_IN
{
    float3 vPosition        : POSITION;
    
    float4 vRight           : TEXCOORD0;
    float4 vUp              : TEXCOORD1;
    float4 vLook            : TEXCOORD2;
    float4 vTranslation     : TEXCOORD3;
    float  fLifeTime        : TEXCOORD4;
};

struct VS_OUT
{
    float4 vPosition        : POSITION;
    float  fSize            : PSIZE;
    float  fLifeTime        : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    /* In.vPosition * 월드 * 뷰 * 투영 */    
    //float4x4 == matrix
    matrix  matWV, matWVP;
    row_major matrix InstanceMatrix = { In.vRight, In.vUp, In.vLook, In.vTranslation };
    
    vector vPosition = mul(vector(In.vPosition, 1.f), InstanceMatrix);

    Out.vPosition = vPosition;
    Out.fSize = length(In.vRight);
    Out.fLifeTime = In.fLifeTime;

    return Out;
}

struct GS_IN
{
    float4 vPosition    : POSITION;
    float  fSize        : PSIZE;
    float  vLifeTime    : TEXCOORD0;
};

struct GS_OUT
{
    float4 vPosition    : SV_POSITION;
    float2 vTexcoord    : TEXCOORD0;
    float  vLifeTime : TEXCOORD1;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    GS_OUT Out[4];
    
    float3 vLook = (g_vCamPosition - In[0].vPosition).xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * In[0].fSize * 0.5f;
    float3 vUp = normalize(cross(vLook, vRight)) * In[0].fSize * 0.5f;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[1]);
    OutStream.Append(Out[2]);
    OutStream.RestartStrip();
    
    OutStream.Append(Out[0]);
    OutStream.Append(Out[2]);
    OutStream.Append(Out[3]);
    OutStream.RestartStrip();
}



/* 출력된 정점 위치벡터의 w값으로 모든 성분을 나눈다 -> 투영스페이스로 변환 */ 
/* 정점의 위치에 대해서 뷰포트 변환을 수행한다 */ 
/* 정점의 모든 정보를 보간하여 픽셀을 만든다. -> 래스터라이즈 */ 

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float  vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord + In.vLifeTime);
    if (vMtrlDiffuse.r <= 0.3f || In.vLifeTime >= g_vMaxLifeTime)
        vMtrlDiffuse.a = 0.f;
        
    Out.vDiffuse = vMtrlDiffuse;
    return Out;
}

technique11 Tech
{
    pass Pass0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
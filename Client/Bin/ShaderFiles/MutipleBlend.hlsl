#include "DefualtStates.hlsli"
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture : register(t0);
Texture2D g_DestTexture : register(t1);

float g_Percent;

sampler sampler0 = sampler_state
{
    filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
};

/* 정점 쉐이더 : */
/* 정점에 대한 셰이딩 == 정점에 필요한 연산을 수행한다 == 정점의 상태변환(월드, 뷰, 투영) + 추가변환 */
/* 정점의 구성 정보를 수정, 변경한다 */ 
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vSrcTex : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;
    
    matrix matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    
    In.vTexcoord.x = abs(In.vTexcoord.x + g_Percent);
    Out.vSrcTex = In.vTexcoord;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vSrcTex : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    // UV는 0 ~ 1 사이의 값으로 들어옴
    // 보정을 해도 결국 정점을 보정하는거지 txture UV는 보정되는 값이 아닌듯?
    // 근데 결국 그릴떄 삼각형 단위로 그려서 UV만으로 모든걸 조절하기는 어렵다.
    
    float4 vSrcColor = g_Texture.Sample(sampler0, In.vSrcTex);
    float OrizinAlpha = vSrcColor.a;
    
    // 넘겨받은 UV 좌표 기준에서 보정치를 빼게되면 UV의 X값은 0 ~ 1.0이 되고
    // 가장 자리로올수록 보정치없는값은 0.5에 가까워 지게 된다.
    // 이때 수식 In.vSrcTex.x - g_Percent 에 의해 0 ~ 1.0f 사이로 보정되고
    // - 0.5f를 빼게되면 가운데 0.0 ~ 0.5 사이의 값으로 변한다.
    // 이때 *2 를 통해서 0 ~ 1 값으로 만든다음 알파 1값에서 변환된 값을빼면
    // 그릴지 말지 나옴
    
    // 야매방법같긴함 왜냐면 그릴지 말지는 0~ 1.0사이의 값으로하고
    // 샘플링하는값 tex좌표는 보정치를 가져오니까 이방법이 맞는거 같기도하고
    float2 BlurUV = abs(In.vSrcTex.x - g_Percent - 0.5f) * 2.f;
    
    vSrcColor.a = saturate(1.0f - BlurUV);
    if (OrizinAlpha < 0.4f || vSrcColor.a < 0.4f)
        discard;
    
    Out.vColor = vSrcColor;
    return Out;
}

technique11 Tech
{
    pass MutiplyBlend
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
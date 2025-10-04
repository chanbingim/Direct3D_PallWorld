    #include "DefualtStates.hlsli"

matrix      g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D   g_Texture : register(t0);

vector      g_vCamPosition;
float       g_Percent;

sampler sampler0 = sampler_state
{
    filter = MIN_MAG_MIP_LINEAR;
};

/* 정점 쉐이더 : */
/* 정점에 대한 셰이딩 == 정점에 필요한 연산을 수행한다 == 정점의 상태변환(월드, 뷰, 투영) + 추가변환 */
/* 정점의 구성 정보를 수정, 변경한다 */ 
struct VS_IN
{
    float3 vPosition : POSITION;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
};

VS_OUT VS_MAIN(VS_IN In)
{
    //지오 메트리 셰이더를 이용해서 한번 그려보자
    VS_OUT Out;
    Out.vPosition = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

// 지오 메트리 셰이더를 사용한다.
struct GS_IN
{
    float4 vPosition : POSITION;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

[maxvertexcount(6)]
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> OutStream)
{
    //지오 메트리 셰이더를 이용해서 한번 그려보자
    GS_OUT Out[4];
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    float3 vLook = (g_vCamPosition - In[0].vPosition).xyz;
    float3 vRight = normalize(cross(float3(0.f, 1.f, 0.f), vLook)) * length(g_WorldMatrix._11_12_13) * 0.5f;
    float3 vUp = normalize(cross(vLook, vRight)) * length(g_WorldMatrix._21_22_23) * 0.5f;
    
    Out[0].vPosition = mul(float4(In[0].vPosition.xyz + vRight + vUp, 1.f), matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    
    Out[1].vPosition = mul(float4(In[0].vPosition.xyz - vRight + vUp, 1.f), matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    
    Out[2].vPosition = mul(float4(In[0].vPosition.xyz - vRight - vUp, 1.f), matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    
    Out[3].vPosition = mul(float4(In[0].vPosition.xyz + vRight - vUp, 1.f), matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    
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
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

/* 픽셀 쉐이더 : 픽셀의 최종적인 색을 결정하낟. */
PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float4 vNewColor = g_Texture.Sample(sampler0, In.vTexcoord);
    if(vNewColor.a < 0.4f)
        discard;
   
    Out.vColor = vNewColor;
    return Out;
}

/* 셰이더를 통해 특정 UV좌표 만 탈락시킨다.*/
PS_OUT PS_PERCENT(PS_IN In)
{
    PS_OUT Out;
    
    float4 vNewColor = g_Texture.Sample(sampler0, In.vTexcoord);
    if (vNewColor.a < 0.4f || In.vTexcoord.x > g_Percent)
        discard;
   
    Out.vColor = vNewColor;
    return Out;
}

technique11 Tech
{
    pass WORLD
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass Screen
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_None, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
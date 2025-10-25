static const int EFFECT_DEFULAT = 0;
static const int EFFECT_MUTIPLY = 1;

static const int EFFECT_RADIAL = 1;

// 함수 설명
// EFFECT_TYPE
// EFFECT_DEFULAT : 0;
// EFFECT_MUTIPLY : 1;
inline float4 ComputeColor(float4 vSrcColor, float4 vDestColor, uint Type)
{
    float4 vOut = 0;
    switch (Type)
    {
        case EFFECT_MUTIPLY:
            vOut = vSrcColor * vDestColor;
            break;
        default:
            vOut = vSrcColor + vDestColor;
            break;
    }

    return vOut;
}

// 디졸브 엑션
inline float4 ComputeDissolve(float4 vSrcColor, float4 vDestColor, float fTime, bool bIsEdge, float fEdgeWidth)
{
    float4 vOut = vSrcColor;
    if(bIsEdge)
    {
        float edge = saturate(1.0 - abs(fTime - vDestColor.r) * fEdgeWidth);
        vOut = lerp(vOut, vDestColor, edge);
    }
    
    if(fTime <= vDestColor.r)
        vOut.a *= vDestColor.a;

    return vOut;
}

// 함수 설명
// EFFECT_DEFULAT : 0;
// EFFECT_Trig: 1;
// time = 시간 속도
inline float2 ComputeUV(float2 vBaseUV, bool bIsPolar, float fTime, uint Type)
{
    float2 vOut = 0;
    if (bIsPolar)
        vBaseUV = -0.5f;
    
    vBaseUV *= 2.f;
    switch (Type)
    {
        // 삼각함수 파형 형태
        case EFFECT_RADIAL:
           {
                float2 vDir = vBaseUV - float2(0, 0);
                float fAngle = atan2(vDir.y, vDir.x);
                
                vBaseUV += float2(cos(fAngle), sin(fAngle));
            }
            break;
    }
    
    vOut = vBaseUV;
    if (0 < fTime)
        vOut += fTime;
    
    return vOut;
}

inline float2 SliceUV(float2 vBaseUV, float fRatio, float2 vSliceCount)
{
    float2 vOut = vBaseUV;
    
    float2 vSliceSize = 1.0 / vSliceCount;
    
    // 전체 슬라이스 개수
    float totalSlices = vSliceCount.x * vSliceCount.y;

    // 현재 슬라이스 인덱스 (0 ~ totalSlices-1)
    float fFrame = floor(fRatio * totalSlices);
    
      // 슬라이스 위치 (x, y)
    float sliceX = fmod(fFrame, vSliceCount.x);
    float sliceY = floor(fFrame / vSliceCount.x);

    vOut.x = (vBaseUV.x * vSliceSize.x) + (sliceX * vSliceSize.x);
    vOut.y = (vBaseUV.y * vSliceSize.y) + (sliceY * vSliceSize.y);
    
    return vOut;
}

inline float ComputeAlpha(float4 vBaseColor, float fTime, int iType, float2 vTexcrood)
{
    float vOut;

    
    switch (iType)
    {
        case 0 :
            {
                float flength = length(vTexcrood - float2(0.5f, 0.5f));
                if (fTime <= flength)
                    vBaseColor.a = 0.f;
            }
            break;
        
        case 1:
            {
                vTexcrood -= 0.5f;
                float flength = length(vTexcrood);
                if (fTime >= flength)
                    vBaseColor.a = 0.f;
            }
            break;
        default :
            break;
    }
    
    vOut = vBaseColor.a;
    return vOut;
}

inline float4 DissolveFunc(Texture2D DissolveTexture, SamplerState Sampler, float2 vTexcoord, float fAmount)
{
    float4 vOut;

    vOut = DissolveTexture.Sample(Sampler, vTexcoord);
    clip(vOut.r - fAmount);
    return vOut;
}
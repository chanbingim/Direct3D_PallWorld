// 어떤 자료형을 통해서 저장할지
struct TrailData
{
    float4  vLBPosition;
    float4  vTRPosition;
    float   fLifeTime;
};

cbuffer TrailConstBuffer : register(b0)
{
    row_major matrix    SocketMatrix;
    
    uint        iHeadIndex;
    uint        iMaxIndex;

    float       fDeletaTime;
    float       fSize;
    float       fLifeTime;      // 여기서 Lifetime보다 크면 
    float       fSmoothRate;    // 얼마나 부드럽게 따라갈지 (0~1)
    
    uint        iPadding;
    uint        iPadding2;
}

// 읽어올 데이터 구조체
StructuredBuffer<TrailData>         g_Input : register(t0);

//연산 결과를 저장하는 부분 
RWStructuredBuffer<TrailData>       g_Out : register(u0);

// SV_GroupID           : 스레드가 속한 그룹의 좌표
// SV_GoupThreadID      : 그룹 내에서, 스레드의 좌표
// SV_GoupIndex         : 그룹 내에서, 스레드의 인덱스 (1차원)
// SV_DispatchThreadID  : 전체 스레드(모든 그룹 통합) 기준으로, 호출된 스레드의 좌표

[numthreads(8, 4, 4)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
void main( uint3 GlobalThreadID : SV_DispatchThreadID,
           uint3 LocalThreadID : SV_GroupThreadID,
           uint LocalIndex : SV_GroupIndex)
{
    // 여기서 트레일이 죽었는지 살았는지를 확인해준다.
    // 여기 이후로는 졸려서 좀있다가 
    uint InlearIndex = GlobalThreadID.x +
                   GlobalThreadID.y * (4 * 8) + // 32
                   GlobalThreadID.z * (4 * 8) * (4 * 4); // 512
    
    float4 vRight = { fSize, 0.f, 0.f, 0.f };
    float4 vLook = { 0.f, fSize, 0.f, 0.f };
    float4 vUp = { 0.f, 0.f, fSize, 0.f };
    float4 vPosition = { 0.f, 0.f, 0.f, 1.f };
    
    if (iHeadIndex == InlearIndex)
    {
        g_Out[InlearIndex].vLBPosition = mul(vPosition, SocketMatrix);
        g_Out[InlearIndex + 1].vTRPosition = mul(vPosition, SocketMatrix);
        g_Out[InlearIndex].fLifeTime = 0.f;
    }
    else
        g_Out[InlearIndex] = g_Input[InlearIndex];
    
    g_Out[InlearIndex].fLifeTime += fDeletaTime;

}
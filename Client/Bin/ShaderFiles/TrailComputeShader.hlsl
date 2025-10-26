// 어떤 자료형을 통해서 저장할지
struct TrailData
{
    float4        vRight;
    float4        vUp;
    float4        vLook;
    float4        vPosition;

    float         fLifeTime;
};

cbuffer TrailConstBuffer : register(b0)
{
    matrix      SocketMatrix;
    
    uint        iHeadIndex;
    uint        iMaxIndex;

    float       fDeletaTime;
    float       fLifeTime;      // 여기서 Lifetime보다 크면 
    float       fSmoothRate;    // 얼마나 부드럽게 따라갈지 (0~1)
}

// 읽어올 데이터 구조체
StructuredBuffer<TrailData>         g_Input : register(t0);

//연산 결과를 저장하는 부분 
RWStructuredBuffer<TrailData>       g_Out : register(u0);

// SV_GroupID           : 스레드가 속한 그룹의 좌표
// SV_GoupThreadID      : 그룹 내에서, 스레드의 좌표
// SV_GoupIndex         : 그룹 내에서, 스레드의 인덱스 (1차원)
// SV_DispatchThreadID  : 전체 스레드(모든 그룹 통합) 기준으로, 호출된 스레드의 좌표

[numthreads(128, 1, 1)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
void main( uint3 GlobalThreadID : SV_DispatchThreadID,
                   uint3 LocalThreadID : SV_GroupThreadID,
                   uint LocalIndex : SV_GroupIndex)
{
    // 여기서 트레일이 죽었는지 살았는지를 확인해준다.
    // 여기 이후로는 졸려서 좀있다가 
    uint InlearIndex = GlobalThreadID.x + GlobalThreadID.y * 8 + GlobalThreadID.z * 8 * 8;
    uint iIndex = (iHeadIndex + InlearIndex) % iMaxIndex;
    
    // 일단 이렇게 연산을 해보고 잘되면 보간도 넣어보자
    g_Out[iIndex].vRight = mul(g_Input[iIndex].vRight, SocketMatrix);
    g_Out[iIndex].vLook = mul(g_Input[iIndex].vLook, SocketMatrix);
    g_Out[iIndex].vUp = mul(g_Input[iIndex].vUp, SocketMatrix);
    g_Out[iIndex].vPosition = mul(g_Input[iIndex].vPosition, SocketMatrix);
    g_Out[iIndex].fLifeTime = g_Input[iIndex].fLifeTime + fDeletaTime;
}
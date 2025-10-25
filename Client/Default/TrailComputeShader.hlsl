
// 어떤 자료형을 통해서 저장할지
struct TrailData
{
    matrix                  MatWorld;
    float                   vLifeTime;
};


cbuffer TrailParam : register(b0)
{
    float                           fDeletaTime;
    float                           fLifeTime;          // 여기서 Lifetime보다 크면 
    float                           fSmoothRate;        // 얼마나 부드럽게 따라갈지 (0~1)
}

// 읽어올 데이터 구조체
StructuredBuffer<TrailData>         g_Input : register(t0);

//연산 결과를 저장하는 부분 
RWStructuredBuffer<TrailData>       g_Out : register(u0);
RWStructuredBuffer<uint>            g_iNumVertices : register(u1); // 생성된 정점 수 (atomic)


// SV_GroupID           : 스레드가 속한 그룹의 좌표
// SV_GoupThreadID      : 그룹 내에서, 스레드의 좌표
// SV_GoupIndex         : 그룹 내에서, 스레드의 인덱스 (1차원)
// SV_DispatchThreadID  : 전체 스레드(모든 그룹 통합) 기준으로, 호출된 스레드의 좌표

[numthreads(128, 1, 1)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
void CS_TrailMain( uint3 DTid : SV_DispatchThreadID )
{
    // 여기서 트레일이 죽었는지 살았는지를 확인해준다.
    // 여기 이후로는 졸려서 좀있다가 
}


// 여기서 트위치표현을 한다.
[numthreads(128, 1, 1)] // 그룹 당 스레드 개수 (최대 1024 까지 지정 가능)
void CS_GenerateTrailMain(uint3 DTid : SV_DispatchThreadID)
{
    
}

#include "WorkBench.h"

#include "GameInstance.h"
#include "GamePlayHUD.h"
#include "Level.h"

#include "WorkBenchCreateUI.h"

vector<_uInt> CWorkBench::m_MakeItemList = {};

CWorkBench::CWorkBench(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CWorkBench::CWorkBench(const CWorkBench& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CWorkBench::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_WORKBENCH;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    m_MakeItemList.push_back(1);
    m_MakeItemList.push_back(2);
    m_MakeItemList.push_back(3);
    return S_OK;
}

HRESULT CWorkBench::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CWorkBench::Priority_Update(_float fDeletaTime)
{
}

void CWorkBench::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    //여기서 플레이어랑 멀어지면 UI 제거
    //이건 일단 UI 보고
    if (m_bIsAction)
    {
        _float3 vWorkBenchPos = m_pTransformCom->GetPosition();
        _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);
        _vector vCalWorkBenchPos = XMLoadFloat3(&vWorkBenchPos);

        _float fLength = XMVectorGetX(XMVector3Length(vPlayerPos - vCalWorkBenchPos));
        if (fLength < 20.f)
            ArchitectureAction();
    }
}

void CWorkBench::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
    }
}

HRESULT CWorkBench::Render()
{
    _uInt iNumMesh = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMesh; ++i)
    {
        Apply_ConstantShaderResources(i);

        if (m_bIsCompleted)
            m_pShaderCom->Update_Shader(0);
        else
            m_pShaderCom->Update_Shader(2);

        m_pVIBufferCom->Render(i);
    }

    m_pHitBoxCollision->Render();

    return S_OK;
}

void CWorkBench::ArchitectureAction()
{
    //여기서 워크밴치 UI에 대한처리
    auto pLevelHUD = m_pGameInstance->GetCurrentLevel()->GetHUD();
    if (nullptr == pLevelHUD)
        return;

    CGamePlayHUD* pGamePlayHUD = static_cast<CGamePlayHUD*>(pLevelHUD);
    if (false == m_bIsAction)
    {
        auto pWorkPopupUI = static_cast<CWorkBenchCreateUI*>(pGamePlayHUD->GetPopUpUserInterface(2));
        CWorkBenchCreateUI::WORK_BENCH_CREATE_DESC WorkBenchCreateDesc;
        WorkBenchCreateDesc.ItemList = &m_MakeItemList;
        WorkBenchCreateDesc.szTitleName = TEXT("원시적인 작업대");

        pWorkPopupUI->SetViewItemList(&WorkBenchCreateDesc);


        pGamePlayHUD->ActivePopUpUserInterface(2);
        m_bIsAction = true;
    }
    else
    {
        pGamePlayHUD->UnActivePopUpUserInterface(2);
        m_bIsAction = false;
    }
}

void CWorkBench::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    __super::HitOverlapFunction(vDir, pHitActor);
}

HRESULT CWorkBench::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_WorkBench"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CWorkBench* CWorkBench::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWorkBench* pWorkBench = new CWorkBench(pDevice, pContext);
    if (FAILED(pWorkBench->Initalize_Prototype()))
    {
        Safe_Release(pWorkBench);
        MSG_BOX("CREATE FAIL : WORK BENCH");
    }
    return pWorkBench;
}

CGameObject* CWorkBench::Clone(void* pArg)
{
    CWorkBench* pWorkBench = new CWorkBench(*this);
    if (FAILED(pWorkBench->Initialize(pArg)))
    {
        Safe_Release(pWorkBench);
        MSG_BOX("CLONE FAIL : WORK BENCH");
    }
    return pWorkBench;
}

void CWorkBench::Free()
{
    __super::Free();
}

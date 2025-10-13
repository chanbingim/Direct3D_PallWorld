#include "PalBox.h"

#include "GameInstance.h"
#include "Level.h"

#include "PellBase.h"
#include "WorkAbleObject.h"
#include "PellBoxManager.h"

#include "PalBoxSlot.h"

CPalBox::CPalBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CArchitecture(pDevice, pContext)
{
}

CPalBox::CPalBox(const CPalBox& rhs) :
    CArchitecture(rhs)
{
}

HRESULT CPalBox::Initalize_Prototype()
{
    m_ItemID = ARCHITECTURE_PALBOX;
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalBox::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_fAreaRadius = 30.f;
    list<CGameObject*> EmptyList = {};
    for (_uInt i = 0; i < ENUM_CLASS(PELL_WORK_TYPE::END); ++i)
        m_JobList.emplace(PELL_WORK_TYPE(i), EmptyList);

    auto pPellBoxManager = CPellBoxManager::GetInstance();
    _uInt iCurLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    auto WorkAbleLayer = m_pGameInstance->GetAllObejctToLayer(iCurLevel, TEXT("Layer_GamePlay_WorkAbleObject"));
    for (auto pWorkAbleObject : (*WorkAbleLayer))
    {
        _float3 vWorkAbleObjectPos = pWorkAbleObject->GetTransform()->GetPosition();
        _float3 vPalBoxPos = m_pTransformCom->GetPosition();

        _float  fLenght = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPalBoxPos) - XMLoadFloat3(&vWorkAbleObjectPos)));
        if (fLenght <= m_fAreaRadius)
            Add_JobListObject(static_cast<CWorkAbleObject*>(pWorkAbleObject)->GetWorkAbleType(), pWorkAbleObject);
    }

    pPellBoxManager->Add_PalBox(this);
    m_pDebugSphere->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    m_pHitBoxCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CPalBox::Priority_Update(_float fDeletaTime)
{
}

void CPalBox::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPalBox::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
        m_pGameInstance->ADD_CollisionList(m_pHitBoxCollision);
    }
}

HRESULT CPalBox::Render()
{
    _uInt iNumMesh = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0;  i < iNumMesh; ++i)
    {
        Apply_ConstantShaderResources(i);

        if(m_bIsCompleted)
            m_pShaderCom->Update_Shader(0);
        else
            m_pShaderCom->Update_Shader(2);

        m_pVIBufferCom->Render(i);
    }

    m_pHitBoxCollision->Render();
    m_pDebugSphere->Render();

    return S_OK;
}

void CPalBox::ArchitectureAction()
{

}

void CPalBox::Add_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject)
{
    auto pair = m_JobList.find(eWorkType);
    if (pair == m_JobList.end())
        return;

    auto& ObjectList = pair->second;
    auto iter = find(ObjectList.begin(), ObjectList.end(), pObject);
    if (iter == ObjectList.end())
    {
        Safe_AddRef(pObject);
        ObjectList.push_back(pObject);
    }
}

void CPalBox::Remove_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject)
{
    auto pair = m_JobList.find(eWorkType);
    if (pair == m_JobList.end())
        return;

    auto ObjectList = pair->second;
    auto iter = find(ObjectList.begin(), ObjectList.end(), pObject);
    if (iter == ObjectList.end())
        return;

    Safe_Release(*iter);
    ObjectList.erase(iter);
}

void CPalBox::StorePalBox(const PELL_INFO& PellInfo, _Int iStoreID)
{
    _uInt iInfoIndex = {};
    if (-1 != iStoreID)
    {
        m_BoxInPellInfo[iStoreID].first = PellInfo;
        m_BoxInPellInfo[iStoreID].second = true;
    }
    else
    {
        auto iter = find_if(m_BoxInPellInfo.begin(), m_BoxInPellInfo.end(), [&](auto& pair)->_bool
            {
                if (false == pair.second)
                    return true;

                iInfoIndex++;
                return false;
            });

        if (iter == m_BoxInPellInfo.end())
            return;

        iter->first = PellInfo;
        iter->second = true;
    }
    return;
}

const PELL_INFO* CPalBox::GetPalBoxInfo(_uInt iStoreID)
{
    if (-1 == iStoreID)
        return nullptr;
    else
    {
        if (m_BoxInPellInfo[iStoreID].second)
        {
            return &m_BoxInPellInfo[iStoreID].first;
        }
        else
            return nullptr;
    }
    return nullptr;
}

HRESULT CPalBox::LoadPalBox(_uInt iStoreID, PELL_INFO* pOutPalInfo)
{
    if (0 > iStoreID || m_BoxInPellInfo.size() <= iStoreID)
        return E_FAIL;

    if (m_BoxInPellInfo[iStoreID].second)
    {
        memcpy(pOutPalInfo, &m_BoxInPellInfo[iStoreID].first, sizeof(PAL_NETWORK_DATA));
        pOutPalInfo->NickName = m_BoxInPellInfo[iStoreID].first.NickName;
        pOutPalInfo->pPellIconTexture = m_BoxInPellInfo[iStoreID].first.pPellIconTexture;
        pOutPalInfo->DefaultSkill = m_BoxInPellInfo[iStoreID].first.DefaultSkill;
        m_BoxInPellInfo[iStoreID].second = false;
    }
    else
        return E_FAIL;

    return S_OK;
}

void CPalBox::Add_WorkPalList(_uInt iStoreID)
{
    // 생성해서 넘기자 
    // 위치는 지금 선택된 PalBox의 위치에서
    _uInt iCurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    CPellBase::PELL_BASE_DESC PellDesc = {};
    PellDesc.vScale = { 1.f, 1.f, 1.f };

    _float3 vPalPosition = GetTransform()->GetPosition();
    vPalPosition.x += m_pGameInstance->Random(1.f, 4.f);
    vPalPosition.y += m_pGameInstance->Random(1.f, 4.f);
    PellDesc.vPosition = vPalPosition;

    PellDesc.bIsPellData = true;
    if (FAILED(LoadPalBox(iStoreID, &PellDesc.PellInfo)))
        return;

    PellDesc.PellInfo.ePellStorageState = PELL_STORAGE_STATE::WORLD;
    auto pBase = m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iCurLevelID, PellDesc.PellInfo.szPrototyeName, &PellDesc);
    if (nullptr == pBase)
        return;
    m_pGameInstance->Add_GameObject_ToLayer(iCurLevelID, TEXT("Layer_GamePlay_Pell"), static_cast<CGameObject*>(pBase));

    CPellBase* pPellBase = static_cast<CPellBase*>(pBase);
    _uInt iIndex = {};
    auto iter = find_if(m_SpawnPells.begin(), m_SpawnPells.end(), [&](CGameObject* pObject)->_bool
        {
            if (nullptr == pObject)
                return true;

            iIndex++;
            return false;
        });

    if (iter == m_SpawnPells.end() || m_iMaxSpawnPell <= iIndex)
        return;

    (*iter) = pPellBase;
}

void CPalBox::Add_WorkPalList(const PELL_INFO& PellInfo, _Int WorkSlotID)
{
    // 생성해서 넘기자 
  // 위치는 지금 선택된 PalBox의 위치에서
    _uInt iCurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    CPellBase::PELL_BASE_DESC PellDesc = {};
    PellDesc.vScale = { 1.f, 1.f, 1.f };

    _float3 vPalPosition = GetTransform()->GetPosition();
    vPalPosition.x += m_pGameInstance->Random(1.f, 4.f);
    vPalPosition.y += m_pGameInstance->Random(1.f, 4.f);
    PellDesc.vPosition = vPalPosition;

    PellDesc.bIsPellData = true;
    PellDesc.PellInfo = PellInfo;
    PellDesc.PellInfo.ePellStorageState = PELL_STORAGE_STATE::WORLD;
    auto pBase = m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iCurLevelID, PellDesc.PellInfo.szPrototyeName, &PellDesc);
    if (nullptr == pBase)
        return;
    m_pGameInstance->Add_GameObject_ToLayer(iCurLevelID, TEXT("Layer_GamePlay_Pell"), static_cast<CGameObject*>(pBase));
    m_SpawnPells[WorkSlotID] = static_cast<CPellBase *>(pBase);

}

const PELL_INFO* CPalBox::GetWorkPalInfo(_uInt iWorkIndex)
{
    return nullptr;
}

void CPalBox::Load_WorkPalList(_uInt iStoreID, PELL_INFO* pOutPalInfo)
{
    auto iter = m_SpawnPells.begin();
    for (_uInt i = 0; i < iStoreID; ++i)
        iter++;

    CPellBase* pFindPal = *iter;
    m_SpawnPells.erase(iter);

    *pOutPalInfo = pFindPal->GetPellInfo();
    pFindPal->IsDead();
}

void CPalBox::Remove_WorkPalList(CPellBase* pPellBase)
{
    auto iter = find(m_SpawnPells.begin(), m_SpawnPells.end(), pPellBase);
    if (iter == m_SpawnPells.end())
        return;

    StorePalBox(pPellBase->GetPellInfo());
    m_SpawnPells.erase(iter);
}

void CPalBox::SwapPalBox(_uInt iSlotType, _uInt iToSlotIndex, _uInt iFromSlotIndex)
{
    CPalBoxSlot::PAL_SLOT_TYPE SlotType = CPalBoxSlot::PAL_SLOT_TYPE(iSlotType);
    switch (SlotType)
    {
    case CPalBoxSlot::PAL_SLOT_TYPE::BOX:
        swap(m_BoxInPellInfo[iFromSlotIndex], m_BoxInPellInfo[iToSlotIndex]);
        break;
    case CPalBoxSlot::PAL_SLOT_TYPE::WORK:
        swap(m_SpawnPells[iFromSlotIndex], m_SpawnPells[iToSlotIndex]);
        break;
    }
}

void CPalBox::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
    __super::HitOverlapFunction(vDir, pHitActor);
}

HRESULT CPalBox::ADD_Components()
{
    // 나무 모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalBox"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // 콜리전 정보를 넣어서 한다.
   // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pHitBoxCollision, &OBBDesc)))
        return E_FAIL;
    m_pHitBoxCollision->BindOverlappingEvent([this](_float3 vDir, CGameObject* pHitActor) { HitOverlapFunction(vDir, pHitActor); });

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

#ifdef _DEBUG
    // 콜리전 정보를 넣어서 한다.
  // 근데 이거 돌마다 다른데 이거도 뭐 데이터값으로 하자
    CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = {};
    SphereDesc.pOwner = this;
    SphereDesc.Radius = 30.f; // m_fAreaRadius;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), TEXT("AreaCollision_Com"), (CComponent**)&m_pDebugSphere, &SphereDesc)))
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

CPalBox* CPalBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalBox* pPalBox = new CPalBox(pDevice, pContext);
    if (FAILED(pPalBox->Initalize_Prototype()))
    {
        Safe_Release(pPalBox);
        MSG_BOX("CREATE FAIL : PAL BOX");
    }
    return pPalBox;
}

CGameObject* CPalBox::Clone(void* pArg)
{
    CPalBox* pPalBox = new CPalBox(*this);
    if (FAILED(pPalBox->Initialize(pArg)))
    {
        Safe_Release(pPalBox);
        MSG_BOX("CLONE FAIL : PAL BOX");
    }
    return pPalBox;
}

void CPalBox::Free()
{
    __super::Free();

    for (auto pPellBase : m_SpawnPells)
        Safe_Release(pPellBase);
    m_SpawnPells.clear();

#ifdef _DEBUG
    Safe_Release(m_pDebugSphere);
#endif // _DEBUG
}

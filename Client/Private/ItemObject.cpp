#include "ItemObject.h"

#include "GameInstance.h"

#include "TerrainManager.h"
#include "Chunk.h"

#include "ItemManager.h"
#include "PlayerManager.h"
#include "ItemInfoUI.h"

CItemObject::CItemObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CWorkAbleObject(pDevice, pContext)
{
}

CItemObject::CItemObject(const CItemObject& rhs) :
    CWorkAbleObject(rhs)
{

}

HRESULT CItemObject::Initalize_Prototype()
{
    m_eWorkType = PELL_WORK_TYPE::TRANSPORT;
    return S_OK;
}

HRESULT CItemObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    ITEM_OBJECT_DESC* pDesc = static_cast<ITEM_OBJECT_DESC*>(pArg);
    m_ItemDesc = CItemManager::GetInstance()->GetItemInfo(pDesc->iItemIndex);
    m_iItemCount = pDesc->iItemCount;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    SettingNavigation();
    m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
    return S_OK;
}

void CItemObject::Priority_Update(_float fDeletaTime)
{
}

void CItemObject::Update(_float fDeletaTime)
{


}

void CItemObject::Late_Update(_float fDeletaTime)
{
    if (m_pGameInstance->DistanceCulling(m_pTransformCom->GetPosition()))
    {
        CCollision::DEFAULT_HIT_DESC HitDesc = {};

        //카메라 위치에서 중간지점으로 레이를 쏜다.
        m_bIsRayHit = m_pCollision->RayHit(m_pGameInstance->GetCameraState(WORLDSTATE::POSITION),
                                           m_pGameInstance->GetCameraState(WORLDSTATE::LOOK),
                                           HitDesc);

        if (m_bIsRayHit && 3.f >= HitDesc.vfDistance)
        {
            if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_F))
            {
                //여기서 습득 이벤트 발생
                Damage(nullptr, nullptr);
            }
            else
            {
                //여기서 월드 UI를 통해 아이템의 이름을 보여준다.
                m_pItemUI->Late_Update(fDeletaTime);
            }
        }

        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CItemObject::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Update_Shader(0);

        m_pVIBufferCom->Render(i);
    }

#ifdef _DEBUG
    true == m_bIsRayHit ? m_pCollision->Render({ 0.f, 1.f, 1.f, 1.f }) : m_pCollision->Render();
#endif // _DEBUG
    return S_OK;
}

void CItemObject::Damage(void* pArg, CActor* pDamagedActor)
{
    //여기서 인벤토리로 들어간다.
    CPlayerManager::GetInstance()->AddInventoryItem(m_ItemDesc->iItemNum, m_iItemCount);
    m_IsDead = true;
}

void CItemObject::SettingNavigation()
{
#pragma region NAVI_MESH
    auto pTerrianManager = CTerrainManager::GetInstance();
    _float3 vPosition = m_pTransformCom->GetPosition();

    pTerrianManager->ComputeHieght(m_pTransformCom, &vPosition, true);
    m_pTransformCom->SetPosition(vPosition);
#pragma endregion
}

HRESULT CItemObject::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), m_ItemDesc->szItemModelPath, TEXT("CVIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = {};
    SphereDesc.pOwner = this;
    SphereDesc.Radius = 3.f;
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &SphereDesc)))
        return E_FAIL;

   //셰이더는 그냥 모델 셰이더 써야겠다
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;
    
    CItemInfoUI::ITEM_INFO_DESC ItemDesc = {};
    ItemDesc.pParent = this;
    ItemDesc.vScale = { 120.f, 30.f, 0.f };
    ItemDesc.pItemName = m_ItemDesc->szItemName;
    ItemDesc.vPosition = { ItemDesc.vScale.x * 0.5f, 0.3f, 0.f };
    m_pItemUI = static_cast<CItemInfoUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Item_Info_UI"), &ItemDesc));

    return S_OK;
}

CItemObject* CItemObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItemObject* pItemObject = new CItemObject(pDevice, pContext);
    if (FAILED(pItemObject->Initalize_Prototype()))
    {
        Safe_Release(pItemObject);
        MSG_BOX("CREATE FAIL : ITEM OBJECT");
    }
    return pItemObject;
}

CGameObject* CItemObject::Clone(void* pArg)
{
    CItemObject* pItemObject = new CItemObject(*this);
    if (FAILED(pItemObject->Initialize(pArg)))
    {
        Safe_Release(pItemObject);
        MSG_BOX("CREATE FAIL : ITEM OBJECT");
    }
    return pItemObject;
}

void CItemObject::Free()
{
    __super::Free();

    Safe_Release(m_pItemUI);
    Safe_Release(m_pCollision);
}

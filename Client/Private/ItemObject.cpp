#include "ItemObject.h"

#include "GameInstance.h"
#include "TerrainManager.h"
#include "ItemManager.h"

#include "PlayerManager.h"

CItemObject::CItemObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CItemObject::CItemObject(const CItemObject& rhs) :
    CNoneAnimMesh(rhs)
{

}

HRESULT CItemObject::Initalize_Prototype()
{
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

    auto pNaviMesh = CTerrainManager::GetInstance()->GetNavimesh();
    if (nullptr != pNaviMesh)
        pNaviMesh->ComputeHeight(m_pTransformCom, true);

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
        m_pGameInstance->ADD_CollisionList(m_pCollision);
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
    return S_OK;
}

void CItemObject::Damage(void* pArg, CActor* pDamagedActor)
{
    //여기서 인벤토리로 들어간다.
    CPlayerManager::GetInstance()->AddInventoryItem(m_ItemDesc->iItemNum, m_iItemCount);
    m_IsDead = true;
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
}

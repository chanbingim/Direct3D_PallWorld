#include "Enviormnent.h"

#include "GameInstance.h"
#include "Level.h"

#include "ItemObject.h"
#include "ItemManager.h"
#include "DropComponent.h"

#include "PellBoxManager.h"
#include "Client_Struct.h"

CEnviormnent::CEnviormnent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CWorkAbleObject(pDevice, pContext)
{
}

CEnviormnent::CEnviormnent(const CEnviormnent& rhs) :
    CWorkAbleObject(rhs)
{
}

HRESULT CEnviormnent::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CEnviormnent::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    CPellBoxManager::GetInstance()->Add_JobListObject(m_eWorkType, this);
    return S_OK;
}

void CEnviormnent::Priority_Update(_float fDeletaTime)
{
}

void CEnviormnent::Update(_float fDeletaTime)
{
}

void CEnviormnent::Late_Update(_float fDeletaTime)
{
}

HRESULT CEnviormnent::Render()
{
    return S_OK;
}

HRESULT CEnviormnent::DeadFunction()
{
    if (m_pDropComponent)
    {
        //아이템 떨구기
        list<CDropComponent::DROP_ITEM_DESC> DropItems = {};
        m_pDropComponent->GetDropItemDesc(true, &DropItems);

        if (!DropItems.empty())
        {
            CItemManager* pItemManager = CItemManager::GetInstance();
            _uInt iLevelIndex = m_pGameInstance->GetCurrentLevel()->GetLevelID();

            for (auto ItemDesc : DropItems)
            {
                CItemObject::ITEM_OBJECT_DESC ObjectDesc = {};
                ObjectDesc.vScale = { 1.f,1.f,1.f };
                ObjectDesc.vPosition = m_pTransformCom->GetPosition();
                ObjectDesc.iItemIndex = ItemDesc.iDropItemIndex;
                ObjectDesc.iItemCount = ItemDesc.iDropItemCount;

                _uInt LoopCount = ItemDesc.iDropItemCount / 100.f + 1;
                for (_uInt i = 0; i < LoopCount; ++i)
                {
                    //여기서 월드 아이템 생성
                    m_pGameInstance->Add_GameObject_ToLayer(iLevelIndex, TEXT("Layer_GamePlay_WorkAbleObject"),
                        static_cast<CGameObject *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iLevelIndex, TEXT("Prototype_GameObject_ItemObject"), &ObjectDesc)));

                }
            }
        }
    
    }

    if (PELL_WORK_TYPE::END != m_eWorkType)
        CPellBoxManager::GetInstance()->Remove_JobListObject(m_eWorkType, this);
    m_IsDead = true;
    return S_OK;
}

HRESULT CEnviormnent::HitBeginFunction(_float3 vDir, CGameObject* pGameObject)
{
   

    return S_OK;
}

HRESULT CEnviormnent::HitOverlapFunction(_float3 vDir, CGameObject* pGameObject)
{
    _float3 vHitActorPos = pGameObject->GetTransform()->GetPosition();
    _vector vCalHitActorPos = XMLoadFloat3(&vHitActorPos);

    _float3 vColisionExtents = {};
    if (m_pCollision)
    {
        auto pOBBColision = static_cast<COBBCollision*>(m_pCollision);
        _float3 vBoundCenter = pOBBColision->GetBounding().Center;
        _vector vCalBoundCenter = XMLoadFloat3(&vBoundCenter);
        _vector vDir = XMVector3Normalize(vCalBoundCenter - vCalHitActorPos);

        _float vDist = {};
        _float fLength = XMVectorGetX(XMVector3Length(vCalBoundCenter - vCalHitActorPos));
        pOBBColision->GetBounding().Intersects(vCalHitActorPos, vDir, vDist);
        if (0 > vDist)
            pGameObject->ADDPosition(vDir * vDist);
    }

    return S_OK;
}

void CEnviormnent::Damage(void* pArg, CActor* pDamagedActor)
{
    DEFAULT_DAMAGE_DESC* pDamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
    if (pDamageDesc)
    {
        m_fHealth -= pDamageDesc->fDmaged;
        if (0 >= m_fHealth)
            DeadFunction();
        else
        {
            //여기서 애니메이션 처리
            //있다면

        }
    }
}

CGameObject* CEnviormnent::Clone(void* pArg)
{
    return nullptr;
}

void CEnviormnent::Free()
{
    __super::Free();

    Safe_Release(m_pCollision);
    Safe_Release(m_pDropComponent);
}

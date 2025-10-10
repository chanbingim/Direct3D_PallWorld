#include "DefenceBossComponent.h"

#include "GameInstance.h"
#include "Architecture.h"

CDefenceBossComponent::CDefenceBossComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CDefenceBossComponent::CDefenceBossComponent(const CDefenceBossComponent& Prototype) :
    CComponent(Prototype)
{
}

HRESULT CDefenceBossComponent::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDefenceBossComponent::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    DEFENCE_BOSS_COMPONENT_DESC* pDefenceBossDesc = static_cast<DEFENCE_BOSS_COMPONENT_DESC *>(pArg);

    m_pOwner = pDefenceBossDesc->pOwner;
    m_pTargetArchitecture = pDefenceBossDesc->pTargetArchitecture;
    m_pNavigation = pDefenceBossDesc->pNavigation;

    Safe_AddRef(m_pNavigation);
    return S_OK;
}

HRESULT CDefenceBossComponent::Update(_float fDeletatime)
{
    if (false == m_pPathList.empty())
    {
        _float3 vOwnerPos = m_pOwner->GetTransform()->GetPosition();
        _vector vCalOwnerPos = XMLoadFloat3(&vOwnerPos);
        _vector vTargetPoint = XMLoadFloat3(&m_pPathList.front());

        _vector vMoveDir = XMVector3Normalize(vTargetPoint - vCalOwnerPos);
        _vector vMovePos = vMoveDir * m_fMoveSpeed * fDeletatime;

        _float fLength = XMVectorGetX(XMVector3Length(vTargetPoint - vCalOwnerPos));

        if (2.f < fLength)
        {
            if (m_pNavigation->IsMove(vCalOwnerPos + vMovePos))
            {
                m_pOwner->GetTransform()->LerpTurn({ 0.f, 1.f, 0.f, 0.f }, vCalOwnerPos + vMovePos, XMConvertToRadians(180.f), fDeletatime);
                m_pOwner->GetTransform()->ADD_Position(vMovePos);
            }
        }
        else
            m_pPathList.pop_front();
    }

    return S_OK;
}

HRESULT CDefenceBossComponent::ComputeTargetAcrchitectureAStar(list<_float3>* PathList)
{
    if (nullptr == m_pNavigation || nullptr == m_pOwner || nullptr == m_pTargetArchitecture)
        return E_FAIL;

    _float3 vStartPos = m_pOwner->GetTransform()->GetPosition();
    _float3 vTargetPos = m_pTargetArchitecture->GetTransform()->GetPosition();

    m_pNavigation->ComputePathfindingAStar(vStartPos, vTargetPos, &m_pPathList);
    return S_OK;
}

CDefenceBossComponent* CDefenceBossComponent::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDefenceBossComponent* pDefenceBossCom = new CDefenceBossComponent(pDevice, pContext);
    if (FAILED(pDefenceBossCom->Initialize_Prototype()))
    {
        Safe_Release(pDefenceBossCom);
        MSG_BOX("CREATE FAIL : DEFENCE BOSS COMPONENT");
    }
    return pDefenceBossCom;
}

CComponent* CDefenceBossComponent::Clone(void* pArg)
{
    CDefenceBossComponent* pDefenceBossCom = new CDefenceBossComponent(*this);
    if (FAILED(pDefenceBossCom->Initialize(pArg)))
    {
        Safe_Release(pDefenceBossCom);
        MSG_BOX("CLONE FAIL : DEFENCE BOSS COMPONENT");
    }
    return pDefenceBossCom;
}

void CDefenceBossComponent::Free()
{
    __super::Free();

    Safe_Release(m_pNavigation);
}

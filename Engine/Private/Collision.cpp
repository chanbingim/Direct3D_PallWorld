#include "Collision.h"

#include "GameInstance.h"
#include "GameObject.h"

CCollision::CCollision(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CComponent(pDevice, pContext)
{
}

CCollision::CCollision(const CCollision& rhs) :
    CComponent(rhs),
    m_CollisionType(rhs.m_CollisionType)
#ifdef _DEBUG
    , m_pBatch{ rhs.m_pBatch }
    , m_pEffect{ rhs.m_pEffect }
    , m_pInputLayout{ rhs.m_pInputLayout }
#endif
{
#ifdef _DEBUG
    Safe_AddRef(m_pInputLayout);
#endif
}

HRESULT CCollision::Initialize_Prototype()
{
#ifdef _DEBUG
    m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
    m_pEffect = new BasicEffect(m_pDevice);
    m_pEffect->SetVertexColorEnabled(true);

    const void* pShaderByteCode = { nullptr };
    size_t		iShaderByteCodeLength = {};

    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

    if (m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
        pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout))
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT CCollision::Initialize(void* pArg)
{
    COLLISION_DESC* Desc = static_cast<COLLISION_DESC*>(pArg);
    if (Desc)
    {
        m_pOwner = Desc->pOwner;
    }

    return S_OK;
}

void CCollision::UpdateColiision(_matrix WorldMatrix)
{
  
}

_bool CCollision::RayHit(_vector vOrizin, _vector vDiraction, DEFAULT_HIT_DESC& OutDesc)
{
    return _bool();
}

ContainmentType CCollision::Contains(_vector Point)
{
    return ContainmentType();
}

void CCollision::Render(_vector vColor)
{
    m_pEffect->SetWorld(XMMatrixIdentity());
    m_pEffect->SetView(XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEffect->SetProjection(XMLoadFloat4x4(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    m_pContext->IASetInputLayout(m_pInputLayout);
    m_pEffect->Apply(m_pContext);
}

void CCollision::BindBeginOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> BeginEvent)
{
    m_BeginHitFunc = BeginEvent;
}

void CCollision::BindOverlappingEvent(function<void(_float3 vDir, CGameObject* pHitActor)> OverlappingEvent)
{
    m_OverlapHitFunc = OverlappingEvent;
}

void CCollision::BindEndOverlapEvent(function<void(_float3 vDir, CGameObject* pHitActor)> EndEvent)
{
    m_EndHitFunc = m_EndHitFunc;
}

void CCollision::ADD_HitObejct(CGameObject* pObject)
{
    auto iter = find(m_HitList.begin(), m_HitList.end(), pObject);
    if (iter == m_HitList.end())
    {
        m_HitList.push_back(pObject);
        m_bIsHit = true;
    }
}

void CCollision::ADD_IgnoreObejct(size_t typeID)
{
    m_IgnoreObject.insert(typeID);
}

void CCollision::ADD_OnlyHitObject(size_t typeID)
{
    m_bIsOnlyHitCollision = true;
    m_bIsOnlyHitActorHashCode = typeID;
}

void CCollision::CallFunction()
{
    list<CGameObject*> ExitObject;
    for (auto& HitObject : m_HitList)
    {
        auto iter = find(m_OldHitList.begin(), m_OldHitList.end(), HitObject);
        if (iter == m_OldHitList.end())
        {
            if (m_BeginHitFunc)
                m_BeginHitFunc({}, HitObject);
        }
        else
        {
            if (m_OverlapHitFunc)
                m_OverlapHitFunc({}, HitObject);
        }
    }

    for (auto& OldHitObject : m_OldHitList)
    {
        auto iter = find(m_HitList.begin(), m_HitList.end(), OldHitObject);
        if (iter == m_HitList.end())
        {
            if (m_EndHitFunc)
                m_EndHitFunc({}, OldHitObject);
        }
    }

    m_OldHitList = m_HitList;
    m_HitList.clear();
}

void CCollision::ResetCollision()
{
    m_OldHitList.clear();
    m_HitList.clear();
}

_bool CCollision::IntersectAble(size_t TagetTypeHashCode)
{
    auto iter = m_IgnoreObject.find(TagetTypeHashCode);
    if (iter == m_IgnoreObject.end())
        return true;

    return false;
}

CComponent* CCollision::Clone(void* pArg)
{
    return nullptr;
}

void CCollision::Free()
{
    __super::Free();

    if (false == m_isCloned)
    {
        Safe_Delete(m_pBatch);
        Safe_Delete(m_pEffect);
    }

#ifdef _DEBUG
    Safe_Release(m_pInputLayout);
#endif // _DEBUG

    m_OldHitList.clear();
    m_HitList.clear();
}
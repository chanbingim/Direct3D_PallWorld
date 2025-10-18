#include "Chunk.h"

#include "GameInstance.h"

CChunk::CChunk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext),
    m_pGameInstance (CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CChunk::Initialize(void* pArg)
{
    if (FAILED(Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Component(static_cast<CHUNK_DESC *>(pArg))))
        return E_FAIL;

    return S_OK;
}

_bool CChunk::IsIn(_float3 vPosition)
{
    ContainmentType eContainType = m_pCollision->Contains(XMLoadFloat3(&vPosition));
   if(DirectX::DISJOINT == eContainType)
        return false;
    return true;
}

CNavigation* CChunk::GetChunckNavigation()
{
    return m_pNavigation;
}

HRESULT CChunk::ADD_Component(CHUNK_DESC* pDesc)
{
    // 박스의 크기를 랜더링하기위해서 보여줄 범위
        COBBCollision::OBB_COLLISION_DESC OBBDesc = {};
    OBBDesc.pOwner = this;
    _float3 vRoation = pDesc->vRotation;
    OBBDesc.vAngles = { vRoation.x, vRoation.y, vRoation.z, 0.f } ;
    OBBDesc.vExtents = { 1.f, 1.f, 1.f };

    m_pCollision = static_cast<CCollision*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), &OBBDesc));
    if (nullptr == m_pCollision)
        return E_FAIL;

    m_pNavigation = CNavigation::Create(m_pDevice, m_pContext, pDesc->NavigationFilePath);
    if (nullptr == m_pNavigation)
        return E_FAIL;

    return S_OK;
}

CChunk* CChunk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg)
{
    CChunk* pChunk = new CChunk(pDevice, pContext);
    if (FAILED(pChunk->Initialize(pArg)))
    {
        Safe_Release(pChunk);
        MSG_BOX("CREATE FAIL : CHUNK");
    }
    return pChunk;
}

void CChunk::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}

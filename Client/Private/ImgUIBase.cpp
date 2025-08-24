#include "ImgUIBase.h"

#include "GameInstance.h"

CImgUIBase::CImgUIBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    m_pDevice(pDevice),
    m_pContext(pContext),
    m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CImgUIBase::Prototype_Initialize()
{
    return S_OK;
}

HRESULT CImgUIBase::Initialize(void* pArg)
{
    return S_OK;
}

void CImgUIBase::Update(_float fDeletaTime)
{
}

void CImgUIBase::Render()
{
}

void CImgUIBase::SetVisibility(VISIBILITY eType)
{
    m_eVisibility = eType;
}

const VISIBILITY& CImgUIBase::GetVisibility()
{
    return m_eVisibility;
}

CImgUIBase* CImgUIBase::Clone(void* pArg)
{
    return nullptr;
}

void CImgUIBase::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pGameInstance);
}

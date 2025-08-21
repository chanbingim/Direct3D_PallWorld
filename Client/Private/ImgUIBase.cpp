#include "ImgUIBase.h"

#include "GameInstance.h"

CImgUIBase::CImgUIBase() :
    m_pGameInstance(CGameInstance::GetInstance())
{
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
    Safe_Release(m_pGameInstance);
}

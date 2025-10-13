#include "PalSlotBase.h"

#include "GameInstance.h"

CPalSlotBase::CPalSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPalSlotBase::CPalSlotBase(const CPalSlotBase& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPalSlotBase::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalSlotBase::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_eType = OBJECT_TYPE::STATIC;
    m_bIsMouseEvent = true;
    return S_OK;
}

void CPalSlotBase::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CPalSlotBase::Late_Update(_float fDeletaTime)
{
}

HRESULT CPalSlotBase::Render()
{
    return S_OK;
}

void CPalSlotBase::SetPalInfo(const PELL_INFO& PellInfo)
{
}

void CPalSlotBase::SwapSlot(CPalSlotBase* From)
{
}

void CPalSlotBase::MouseHoverEnter()
{
}

void CPalSlotBase::MouseHovering()
{
}

void CPalSlotBase::MouseHoverExit()
{
}

void CPalSlotBase::MouseButtonDwon()
{
}

void CPalSlotBase::MouseButtonPressed()
{
}

void CPalSlotBase::MouseButtonUp()
{
    if (m_bIsHover)
    {
        CUserInterface* pFoucusWidget = nullptr;
        CPalSlotBase* pToSlot = nullptr;

        m_pGameInstance->GetMouseFocus((CUserInterface**)&pFoucusWidget);
        m_pGameInstance->SetDrag(false);
        auto pPalSlot = dynamic_cast<CPalSlotBase*>(pFoucusWidget);
        if (pPalSlot)
            SwapSlot(pPalSlot);
    }
}

CGameObject* CPalSlotBase::Clone(void* pArg)
{
    return nullptr;
}

void CPalSlotBase::Free()
{
    __super::Free();
}

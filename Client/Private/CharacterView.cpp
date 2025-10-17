#include "CharacterView.h"

#include "GameInstance.h"

#include "Inventory.h"
#include "Equipment.h"

CCharacterView::CCharacterView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CCharacterView::CCharacterView(const CCharacterView& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CCharacterView::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCharacterView::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    m_iZOrder = 3;
    return S_OK;
}

void CCharacterView::Update(_float fDeletaTime)
{
    for (auto& pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CCharacterView::Late_Update(_float fDeletaTime)
{
    for (auto& pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CCharacterView::Render()
{
    for (auto& pChild : m_pChildList)
        pChild->Render();

    return S_OK;
}

HRESULT CCharacterView::ADD_Childs()
{
    CUserInterface::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CUserInterface::GAMEOBJECT_DESC));
    Desc.pParent = this;

    /* 인벤토리 */
    m_pInventory = CInventory::Create(m_pGraphic_Device, m_pDeviceContext);
    if(nullptr == m_pInventory)
        return E_FAIL;

    Desc.vScale = m_pTransformCom->GetScale();
    Desc.vScale.x *= 0.32f;
    Desc.vScale.y *= 0.95f;
    Desc.vPosition = { -Desc.vScale.x, 0.f, 0.f };
    m_pInventory->Initialize(&Desc);
    ADD_Child(m_pInventory);

    /* 장비창 */
    m_pEquipMent = CEquipment::Create(m_pGraphic_Device, m_pDeviceContext);
    if (nullptr == m_pEquipMent)
        return E_FAIL;

    Desc.vScale.x = m_pTransformCom->GetScale().x * 0.6f;
    Desc.vPosition = { Desc.vScale.x * 0.3f, 0.f, 0.f };
    m_pEquipMent->Initialize(&Desc);
    ADD_Child(m_pEquipMent);

    return S_OK;
}

CCharacterView* CCharacterView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCharacterView* pCharacterView = new CCharacterView(pDevice, pContext);
    if (FAILED(pCharacterView->Initalize_Prototype()))
    {
        Safe_Release(pCharacterView);
        MSG_BOX("CREATE FAIL : CHARACTER VIEW");
    }
    return pCharacterView;
}

CGameObject* CCharacterView::Clone(void* pArg)
{
    CCharacterView* pCharacterView = new CCharacterView(*this);
    if (FAILED(pCharacterView->Initialize(pArg)))
    {
        Safe_Release(pCharacterView);
        MSG_BOX("CLONE FAIL : CHARACTER VIEW");
    }
    return pCharacterView;
}

void CCharacterView::Free()
{
    __super::Free();

    Safe_Release(m_pInventory);
    Safe_Release(m_pEquipMent);
}

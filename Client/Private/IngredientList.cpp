#include "IngredientList.h"

#include "GameInstance.h"
#include "IngredientUI.h"

CIngredientList::CIngredientList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{
}

CIngredientList::CIngredientList(const CIngredientList& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CIngredientList::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CIngredientList::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    return S_OK;
}

void CIngredientList::Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CIngredientList::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);
}

HRESULT CIngredientList::Render()
{
    return S_OK;
}

HRESULT CIngredientList::ADD_Components()
{
    _float3 vScale = m_pTransformCom->GetScale();
    CIngredientUI::GAMEOBJECT_DESC pIngredientDesc = {};

    _float  IngredientScaleY = vScale.y / m_iNumIngredient; 
    _float  IngredientStartPosY = -vScale.y * 0.5f + IngredientScaleY * 0.5f;

    pIngredientDesc.pParent = this;
    pIngredientDesc.vScale = { vScale.x, IngredientScaleY, 0.f};
    for (auto i = 0; i < m_iNumIngredient; ++i)
    {
        pIngredientDesc.vPosition = {0.f, IngredientStartPosY  + IngredientScaleY * i, 0.f };

        CIngredientUI* pNewIngredient = static_cast<CIngredientUI*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT(""), &pIngredientDesc));
        m_Ingredients.push_back(pNewIngredient);
        ADD_Child(pNewIngredient);
    }

    return S_OK;
}

CIngredientList* CIngredientList::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CIngredientList* pIngredientList = new CIngredientList(pDevice, pContext);
    if (FAILED(pIngredientList->Initalize_Prototype()))
    {
        Safe_Release(pIngredientList);
        MSG_BOX("CREATE FAIL : INGREDIENT LIST");
    }
    return pIngredientList;
}

CGameObject* CIngredientList::Clone(void* pArg)
{
    CIngredientList* pIngredientList = new CIngredientList(*this);
    if (FAILED(pIngredientList->Initialize(pArg)))
    {
        Safe_Release(pIngredientList);
        MSG_BOX("CLONE FAIL : INGREDIENT LIST");
    }
    return pIngredientList;
}

void CIngredientList::Free()
{
    __super::Free();

    for (auto pIngrdient : m_Ingredients)
        Safe_Release(pIngrdient);
}

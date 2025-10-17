#include "TechSlotLine.h"

#include "GameInstance.h"
#include "TechSlot.h"

#include "TechManager.h"
#include "LevelIcon.h"

CTechSlotLine::CTechSlotLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext),
    m_pTechManager(CTechManager::GetInstance())
{
    Safe_AddRef(m_pTechManager);
}

CTechSlotLine::CTechSlotLine(const CTechSlotLine& rhs) :
    CBackGround(rhs),
    m_pTechManager(CTechManager::GetInstance())
{
    Safe_AddRef(m_pTechManager);
}

HRESULT CTechSlotLine::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSlotLine::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_Childs()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CTechSlotLine::Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CTechSlotLine::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSlotLine::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTextureCom->SetTexture(0, 0);
    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CTechSlotLine::SetTechLevel(_uInt iLevel)
{
    m_iTechLevel = iLevel;
    m_pLevelIcon->SetLevel(to_wstring(m_iTechLevel).c_str());
    auto TechList = m_pTechManager->GetCategoryLevelTechList(m_iTechLevel);

    auto iter = TechList.begin();
    for (auto pTechSlot : m_pSlotList)
    {
        if (TechList.end() == iter)
            pTechSlot->SetVisibility(VISIBILITY::HIDDEN);
        else
        {
            pTechSlot->SetVisibility(VISIBILITY::VISIBLE);
            pTechSlot->SetSlotItem(m_pTechManager->GetTechData((*iter)));
            iter++;
        }
    }
}

HRESULT CTechSlotLine::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Battle_PellInfo_Background"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTechSlotLine::ADD_Childs()
{
    _float fSlotCount = 4;

    _float3 vParentScale = m_pTransformCom->GetScale();
    //여기서 Icon 하고 사이즈가져와서 하기
    CLevelIcon::GAMEOBJECT_DESC LevelIconDesc = {};
    LevelIconDesc.pParent = this;
    LevelIconDesc.vScale = { vParentScale.x / (fSlotCount + 1), vParentScale.y, 0.f };
    LevelIconDesc.vPosition = { -(vParentScale.x * 0.5f) + LevelIconDesc.vScale.x * 0.5f, 0, 0.f};

    m_pLevelIcon = CLevelIcon::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pLevelIcon->Initialize(&LevelIconDesc)))
        return E_FAIL;
    m_pLevelIcon->SetZOrder(m_iZOrder + 1);
    ADD_Child(m_pLevelIcon);
 
    CTechSlot::GAMEOBJECT_DESC TechSlotDesc = {};
    TechSlotDesc.pParent = this;
    TechSlotDesc.vScale = { vParentScale.x / (fSlotCount + 1), vParentScale.y, 0.f};
    _float vStartPos = -(vParentScale.x * 0.5f) + LevelIconDesc.vScale.x + TechSlotDesc.vScale.x * 0.5f;
    for (_uInt i = 0; i < fSlotCount; ++i)
    {
        TechSlotDesc.vPosition.x = vStartPos + TechSlotDesc.vScale.x * i;
        CTechSlot* pTechSlot = CTechSlot::Create(m_pGraphic_Device, m_pDeviceContext);

        pTechSlot->SetZOrder(m_iZOrder + 1);
        if (FAILED(pTechSlot->Initialize(&TechSlotDesc)))
            return E_FAIL;

        m_pSlotList.push_back(pTechSlot);
        ADD_Child(pTechSlot);
    }
    return S_OK;
}

CTechSlotLine* CTechSlotLine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTechSlotLine* pTechSlotLine = new CTechSlotLine(pDevice, pContext);
    if (FAILED(pTechSlotLine->Initalize_Prototype()))
    {
        Safe_Release(pTechSlotLine);
        MSG_BOX("CREATE FAIL : TECH SLOT LINE");
    }
    return pTechSlotLine;
}

CGameObject* CTechSlotLine::Clone(void* pArg)
{
    CTechSlotLine* pTechSlotLine = new CTechSlotLine(*this);
    if (FAILED(pTechSlotLine->Initialize(pArg)))
    {
        Safe_Release(pTechSlotLine);
        MSG_BOX("CLONE FAIL : TECH SLOT LINE");
    }
    return pTechSlotLine;
}

void CTechSlotLine::Free()
{
    __super::Free();

    Safe_Release(m_pLevelIcon);
    Safe_Release(m_pTechManager);

    for(auto iter : m_pSlotList)
        Safe_Release(iter);
}

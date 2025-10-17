#include "PalSpherUI.h"

#include "GameInstance.h"
#include "PalSpher.h"
#include "PallSpherPercentBar.h"

CPalSpherUI::CPalSpherUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPalSpherUI::CPalSpherUI(const CPalSpherUI& rhs) :
    CBackGround(rhs)
{
}

HRESULT CPalSpherUI::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CPalSpherUI::Initialize(void* pArg)
{
    m_bIsWorld = true;

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

void CPalSpherUI::Update(_float fDeletaTime)
{
    _float3 vScale = m_pTransformCom->GetScale();
    _float3 vPos = m_pTransformCom->GetPosition();
    _matrix WorldMat = XMLoadFloat4x4(&m_pGameInstance->GetInvMatrix(MAT_STATE::VIEW));
    for (_uInt i = 0; i < 3; ++i)
        WorldMat.r[i] = XMVector3Normalize(WorldMat.r[i]) * XMLoadFloat3(&vScale).m128_f32[i];

    WorldMat.r[3] = XMLoadFloat3(&vPos);
    if (m_pParent)
    {
        vPos = m_pParent->GetTransform()->GetPosition();
        WorldMat.r[3] += XMLoadFloat3(&vPos);
        WorldMat.r[3].m128_f32[3] = 1;
        XMStoreFloat4x4(&m_CombinedMatrix, WorldMat);
    }

  
    for (auto pChild : m_pChildList)
        pChild->Update(fDeletaTime);
}

void CPalSpherUI::Late_Update(_float fDeletaTime)
{
    for (auto pChild : m_pChildList)
        pChild->Late_Update(fDeletaTime);

    //m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPalSpherUI::Render()
{
    //여기서 폰트를 출력하던가 하면될거같음


    return S_OK;
}

void CPalSpherUI::SetPalSpherUI(_float fPercent)
{
    m_fPercent = fPercent;
    m_pPercentBar->SetPercent(m_fPercent * 0.01f);
}

HRESULT CPalSpherUI::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedMatrix));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));


    return S_OK;
}

HRESULT CPalSpherUI::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPalSpherUI::ADD_Childs()
{
    CPallSpherPercentBar::PROGRESS_DESC Desc = {};
    Desc.pParent = this;
    _float3 vParentScale = m_pTransformCom->GetScale();
    Desc.vScale = { 3.f , 3.f, 0.f };
    Desc.vColor = { 0.f, 1.f, 0.f, 1.f };

    //Pell Health Bar
    Desc.vPosition = { 0, 0, -0.01f };
    m_pPercentBar = static_cast<CPallSpherPercentBar*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalSpherInfo_PercentBar"), &Desc));
    ADD_Child(m_pPercentBar);

    return S_OK;
}

CPalSpherUI* CPalSpherUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPalSpherUI* pPalSpherUI = new CPalSpherUI(pDevice, pContext);
    if (FAILED(pPalSpherUI->Initalize_Prototype()))
    {
        Safe_Release(pPalSpherUI);
        MSG_BOX("CREATE FAIL : PALSPHER UI");
    }
    return pPalSpherUI;
}

CGameObject* CPalSpherUI::Clone(void* pArg)
{
    CGameObject* pPalSpherUI = new CPalSpherUI(*this);
    if (FAILED(pPalSpherUI->Initialize(pArg)))
    {
        Safe_Release(pPalSpherUI);
        MSG_BOX("CLONE FAIL : PALSPHER UI");
    }
    return pPalSpherUI;
}

void CPalSpherUI::Free()
{
    __super::Free();

    Safe_Release(m_pPercentBar);
}

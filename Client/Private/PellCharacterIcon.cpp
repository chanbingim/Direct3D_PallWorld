#include "PellCharacterIcon.h"

#include "PlayerManager.h"
#include "PellBase.h"
#include "GameInstance.h"

CPellCharacterIcon::CPellCharacterIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBackGround(pDevice, pContext)
{
}

CPellCharacterIcon::CPellCharacterIcon(const CPellCharacterIcon& rhs) : 
    CBackGround(rhs)
{
}

HRESULT CPellCharacterIcon::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;
    return S_OK;
}

HRESULT CPellCharacterIcon::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    PELL_CHARACTER_ICON* pDesc = static_cast<PELL_CHARACTER_ICON*>(pArg);
    if(pDesc)
        m_pSocketMatrix = pDesc->pParentMat;

    m_iZOrder = 1;
    return S_OK;
}

void CPellCharacterIcon::Update(_float fDeletaTime)
{
    _matrix pSocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
    _float3 vPos = {};
    XMStoreFloat3(&vPos, pSocketMatrix.r[3]);
    m_pTransformCom->SetPosition(vPos);
}

void CPellCharacterIcon::Late_Update(_float fDeletaTime)
{
    if (nullptr == m_pTexture)
        return;

    m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CPellCharacterIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(2);
    m_pTexture->SetTexture(0, 0);

    m_pVIBufferCom->Render_VIBuffer();

    return S_OK;
}

void CPellCharacterIcon::SetSelectPellIndex(_uInt iNumSelectPellIndex)
{
    auto PellInfo = CPlayerManager::GetInstance()->GetPellInfomation(iNumSelectPellIndex);
    if (nullptr == PellInfo)
        return;

    m_pTexture = PellInfo->GetPellInfo().pPellIconTexture;
}

HRESULT CPellCharacterIcon::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPellCharacterIcon* CPellCharacterIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPellCharacterIcon* pPellCharacterIcon = new CPellCharacterIcon(pDevice, pContext);
    if (FAILED(pPellCharacterIcon->Initalize_Prototype()))
    {
        Safe_Release(pPellCharacterIcon);
        MSG_BOX("CREATE FAIL : PELL CHARACTER ICON");
    }
    return pPellCharacterIcon;
}

CGameObject* CPellCharacterIcon::Clone(void* pArg)
{
    CPellCharacterIcon* pPellCharacterIcon = new CPellCharacterIcon(*this);
    if (FAILED(pPellCharacterIcon->Initialize(pArg)))
    {
        Safe_Release(pPellCharacterIcon);
        MSG_BOX("CLONE FAIL : PELL CHARACTER ICON");
    }
    return pPellCharacterIcon;
}

void CPellCharacterIcon::Free()
{
    __super::Free();
}

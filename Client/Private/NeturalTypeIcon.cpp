#include "NeturalTypeIcon.h"

#include "GameInstance.h"
#include "NeturalPellInfo.h"

CNeturalTypeIcon::CNeturalTypeIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CTypeIcon(pDevice, pContext)
{
}

CNeturalTypeIcon::CNeturalTypeIcon(const CNeturalTypeIcon& rhs) :
    CTypeIcon(rhs)
{
}

HRESULT CNeturalTypeIcon::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CNeturalTypeIcon::Initialize(void* pArg)
{
    m_bIsWorld = true;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CNeturalTypeIcon::Update(_float fDeletaTime)
{
    CNeturalPellInfo* pNeturalPell = dynamic_cast<CNeturalPellInfo*>(m_pParent);
    if (pNeturalPell)
    {
        _matrix ParentMat = XMLoadFloat4x4(&pNeturalPell->GetCombinedMatrix());
        for (_uInt i = 0; i < 3; ++i)
            ParentMat.r[i] = XMVector3Normalize(ParentMat.r[i]);

        _matrix WorldMat = XMLoadFloat4x4(&m_pTransformCom->GetWorldMat());

        XMStoreFloat4x4(&m_CombinedMat, WorldMat * ParentMat);
    }
}

void CNeturalTypeIcon::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CNeturalTypeIcon::Render()
{
    Apply_ConstantShaderResources();
    m_pShaderCom->Update_Shader(3);

    //이거 내가 보니까 무조건 먼저 그려지는 녀석이 확정되는거같음 여기서 설정해서 보여줘야할듯
    m_pTextureCom->SetTexture(0, m_iTypeID);
    m_pVIBufferCom->Render_VIBuffer();
  
    return S_OK;
}

HRESULT CNeturalTypeIcon::Apply_ConstantShaderResources()
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedMat));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    return S_OK;
}

CNeturalTypeIcon* CNeturalTypeIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNeturalTypeIcon* pTpyeIcon = new CNeturalTypeIcon(pDevice, pContext);
    if (FAILED(pTpyeIcon->Initalize_Prototype()))
    {
        Safe_Release(pTpyeIcon);
        MSG_BOX("CREATE FAIL : Type Icon");
    }

    return pTpyeIcon;
}

CGameObject* CNeturalTypeIcon::Clone(void* pArg)
{
    CNeturalTypeIcon* pTpyeIcon = new CNeturalTypeIcon(*this);
    if (FAILED(pTpyeIcon->Initialize(pArg)))
    {
        Safe_Release(pTpyeIcon);
        MSG_BOX("CLONE FAIL : Type Icon");
    }

    return pTpyeIcon;
}

void CNeturalTypeIcon::Free()
{
    __super::Free();
}

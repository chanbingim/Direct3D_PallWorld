#include "ProjectileSlot.h"

#include "PlayerManager.h"
#include "ItemBase.h"

#include "GameInstance.h"

CProjectileSlot::CProjectileSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CProjectileSlot::CProjectileSlot(const CProjectileSlot& rhs) :
    CPartObject(rhs)
{
}

HRESULT CProjectileSlot::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CProjectileSlot::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        PROJECTILE_DESC* pProjectTileDesc = static_cast<PROJECTILE_DESC*>(pArg);
        if (pProjectTileDesc)
        {
            m_pLeftSocketMatrix = pProjectTileDesc->fLeftSocket;
        }
    }

    m_UseSocketMatrixFlag = 0b00000010;
    return S_OK;
}

void CProjectileSlot::Priority_Update(_float fDeletaTime)
{
}

void CProjectileSlot::Update(_float fDeletaTime)
{
    const CItemBase* CurrentEuipItemInfo = CPlayerManager::GetInstance()->GetCurrentSlotItemInfo(EUQIP_TYPE::PROJECTILE);
    m_pVIBufferCom = CPlayerManager::GetInstance()->GetCurrentSlotItemModel(EUQIP_TYPE::PROJECTILE);

    if (CurrentEuipItemInfo)
    {
        const ITEM_DESC& ItemData = CurrentEuipItemInfo->GetItemData();
        if (ITEM_TYPE::CONSUM == ItemData.ItemType)
            m_bIsLeftSocket = ItemData.TypeDesc.ConsumDesc.bIsLeftSocket;
        else
            m_bIsLeftSocket = ItemData.TypeDesc.EuqipDesc.bIsLeftSocket;

        m_bIsAnimationPlay = ItemData.IsAnimModel;
        if (m_bIsAnimationPlay)
        {
            if (m_bIsAnimationPlay)
                m_pVIBufferCom->PlayAnimation(0, 0, fDeletaTime);
            else
                m_pVIBufferCom->PlayAnimation(0, 0, 0);
        }

        //왼쪽에 달지 오른쪽에 달지는 내가 결정하기
        if (m_bIsLeftSocket)
            UpdateCombinedMatrix(m_pLeftSocketMatrix);
        else
            UpdateCombinedMatrix();
    }
}

void CProjectileSlot::Late_Update(_float fDeletaTime)
{
    if (nullptr != m_pVIBufferCom)
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CProjectileSlot::Render()
{
    Bind_ShaderResources();

    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        true == m_bIsAnimationPlay ? m_pShaderCom->Update_Shader(0) : m_pNoneAimShader->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }

    return S_OK;
}

HRESULT CProjectileSlot::Bind_ShaderResources()
{
    if (m_bIsAnimationPlay)
    {
        m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
        m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
        m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
        m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();
        m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();
    }
    else
    {
        m_pEMVWorldMat = m_pNoneAimShader->GetVariable("g_WorldMatrix")->AsMatrix();
        m_pEMVViewMat = m_pNoneAimShader->GetVariable("g_ViewMatrix")->AsMatrix();
        m_pEMVProjMat = m_pNoneAimShader->GetVariable("g_ProjMatrix")->AsMatrix();
        m_pSRVEffect = m_pNoneAimShader->GetVariable("g_DiffuseTexture")->AsShaderResource();
    }

    return S_OK;
}

HRESULT CProjectileSlot::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};

    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    if (m_bIsAnimationPlay)
        m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));


    return S_OK;
}

HRESULT CProjectileSlot::ADD_Components()
{
    // 매번 CModel이 바뀔때마다 shader를 생성하는건 비효율적인거같은데 이거 그냥하나 만들어두자
// 애니메이션 셰이더
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("AnimShader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    // 애니메이션 아닐경우 사용할 셰이더
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pNoneAimShader)))
        return E_FAIL;

    return S_OK;
}

CProjectileSlot* CProjectileSlot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CProjectileSlot* pProjectileSlot = new CProjectileSlot(pDevice, pContext);
    if (FAILED(pProjectileSlot->Initalize_Prototype()))
    {
        Safe_Release(pProjectileSlot);
        MSG_BOX("CREATE FAIL : PROJECTILE OBJECT");
    }
    return pProjectileSlot;
}

CGameObject* CProjectileSlot::Clone(void* pArg)
{
    CGameObject* pProjectileSlot = new CProjectileSlot(*this);
    if (FAILED(pProjectileSlot->Initialize(pArg)))
    {
        Safe_Release(pProjectileSlot);
        MSG_BOX("CLONE FAIL : PROJECTILE OBJECT");
    }
    return pProjectileSlot;
}

void CProjectileSlot::Free()
{
    CGameObject::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pNoneAimShader);

}

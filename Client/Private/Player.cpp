#include "Player.h"

#include "GameInstance.h"

CPlayer::CPlayer(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CContainerObject(pGraphic_Device, pDeviceContext)
{
}

CPlayer::CPlayer(const CPlayer& rhs) :
    CContainerObject(rhs)
{
}

HRESULT CPlayer::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_PartObejcts()))
        return E_FAIL;

    if(m_ObejctTag.c_str() == L"")
        m_ObejctTag = TEXT("Main Player");
    return S_OK;
}

void CPlayer::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);

}

void CPlayer::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);

    //m_pVIBufferCom->PlayAnimation(4, fDeletaTime, true, "Bip001 L Thigh", "Bip001 R Thigh");
   // m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, true, "Bip001 Spine1");
    //m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, true, "Bip001");
}

void CPlayer::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayer::Render()
{
    /*_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }*/

    return S_OK;
}

HRESULT CPlayer::ADD_PartObejcts()
{

    /*if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Fiona_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;*/

    return S_OK;
}

CPlayer* CPlayer::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CPlayer* pPlayer = new CPlayer(pGraphic_Device, pDeviceContext);
    if (FAILED(pPlayer->Initalize_Prototype()))
    {
        Safe_Release(pPlayer);
        MSG_BOX("CREATE FAIL : PLAYER");
    }
    return pPlayer;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pPlayer = new CPlayer(*this);
    if (FAILED(pPlayer->Initialize(pArg)))
    {
        Safe_Release(pPlayer);
        MSG_BOX("CLONE FAIL : PLAYER");
    }
    return pPlayer;
}

void CPlayer::Free()
{
    __super::Free();
}

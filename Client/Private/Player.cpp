#include "Player.h"

#include "GameInstance.h"
#include "PartObject.h"

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
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_W))
    {
        _vector MovePos = m_pTransformCom->GetLookVector() * 5.f * fDeletaTime;
        m_pTransformCom->ADD_Position(MovePos);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_S))
    {
        _vector MovePos = m_pTransformCom->GetLookVector() * -5.f * fDeletaTime;
        m_pTransformCom->ADD_Position(MovePos);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_A))
    {
        m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), 5.f, fDeletaTime);
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_D))
    {
        m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), -5.f, fDeletaTime);
    }
    __super::Update(fDeletaTime);
}

void CPlayer::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

}

HRESULT CPlayer::Render()
{

    return S_OK;
}

HRESULT CPlayer::ADD_PartObejcts()
{
    CGameObject::GAMEOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CGameObject::GAMEOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };

    lstrcpy(Desc.ObjectTag, L"Player_Upperbody");
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Upper_Body_Default"), TEXT("Part_UpperBody"), &Desc)))
        return E_FAIL;
    
   lstrcpy(Desc.ObjectTag, L"Player_Lowerbody");
    if (FAILED(__super::AddPartObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Lower_Body_Default"), TEXT("Part_LowerBody"), &Desc)))
        return E_FAIL;

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

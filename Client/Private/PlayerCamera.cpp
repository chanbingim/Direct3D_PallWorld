#include "PlayerCamera.h"

#include "GameInstance.h"

CPlayerCamera::CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CBaseCamera(pDevice, pContext)
{
}

CPlayerCamera::CPlayerCamera(const CPlayerCamera& rhs) :
    CBaseCamera(rhs)
{
}

HRESULT CPlayerCamera::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerCamera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if(m_ObejctTag == L"")
         m_ObejctTag = TEXT("Player Camera");

    return S_OK;
}

void CPlayerCamera::Priority_Update(_float fDeletaTime)
{
   
}

void CPlayerCamera::Update(_float fDeletaTime)
{
}

void CPlayerCamera::Late_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);

}

HRESULT CPlayerCamera::Render()
{
    return E_NOTIMPL;
}

void CPlayerCamera::AttachCamera(_vector BonesTransformation)
{
    _float3 vPos = {};
    XMStoreFloat3(&vPos, BonesTransformation);
    SetLocation(vPos);
}

CPlayerCamera* CPlayerCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerCamera* pCamera = new CPlayerCamera(pDevice, pContext);
    if (FAILED(pCamera->Initalize_Prototype()))
    {
        Safe_Release(pCamera);
        MSG_BOX("CREATE FAIL : PLAYER CAMERA");
    }

    return pCamera;
}

CGameObject* CPlayerCamera::Clone(void* pArg)
{
    CPlayerCamera* pCamera = new CPlayerCamera(*this);
    if (FAILED(pCamera->Initialize(pArg)))
    {
        Safe_Release(pCamera);
        MSG_BOX("CLONE FAIL : PLAYER CAMERA");
    }

    return pCamera;
}

void CPlayerCamera::Free()
{
    __super::Free();
}

#include "ShadowCamera.h"

#include "GameInstance.h"

#include "SunLight.h"

#include "PlayerManager.h"
#include "Player.h"


CShadowCamera::CShadowCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CShadowCamera::CShadowCamera(const CShadowCamera& rhs) :
    CGameObject(rhs)
{
}

HRESULT CShadowCamera::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CShadowCamera::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    m_pPlayer = CPlayerManager::GetInstance()->GetCurrentPlayer();

    SHADOW_CAMERA_DESC* pShadowCameraDesc = static_cast<SHADOW_CAMERA_DESC*>(pArg);
    m_pShadowLightDesc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
    m_pShadowLightDesc.fFov = pShadowCameraDesc->fFov;
    m_pShadowLightDesc.fNear = pShadowCameraDesc->fNear;
    m_pShadowLightDesc.fFar = pShadowCameraDesc->fFar;

    m_pSunLight = static_cast<CSunLight *>(m_pGameInstance->GetAllObejctToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("GamePlay_Layer_Dir_Light"))->front());
    return S_OK;
}

void CShadowCamera::Priority_Update(_float fDeletaTime)
{
   
}

void CShadowCamera::Update(_float fDeletaTime)
{
    _float3 vAt = m_pPlayer->GetTransform()->GetPosition();
    m_pShadowLightDesc.vAt = { vAt.x, vAt.y, vAt.z, 0.f };

    _float4 vLightDir = m_pSunLight->GetDireaction();
    _vector vLength = -XMLoadFloat4(&vLightDir) * (m_pShadowLightDesc.fFar * 0.05f);
    XMStoreFloat4(&m_pShadowLightDesc.vEye, XMLoadFloat3(&vAt) + vLength);
    m_pGameInstance->Ready_Shadow_Light(m_pShadowLightDesc);
}

void CShadowCamera::Late_Update(_float fDeletaTime)
{
}

HRESULT CShadowCamera::Render()
{
    return S_OK;
}

CShadowCamera* CShadowCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CShadowCamera* pShadowCamera = new CShadowCamera(pDevice, pContext);
    if (FAILED(pShadowCamera->Initalize_Prototype()))
    {
        Safe_Release(pShadowCamera);
        MSG_BOX("CREATE FAIL : SHADOW CAMERA");
    }
    return pShadowCamera;
}

CGameObject* CShadowCamera::Clone(void* pArg)
{
    CShadowCamera* pShadowCamera = new CShadowCamera(*this);
    if (FAILED(pShadowCamera->Initialize(pArg)))
    {
        Safe_Release(pShadowCamera);
        MSG_BOX("CLONE FAIL : SHADOW CAMERA");
    }
    return pShadowCamera;
}

void CShadowCamera::Free()
{
    __super::Free();
}

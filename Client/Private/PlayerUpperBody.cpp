#include "PlayerUpperBody.h"

#include "GameInstance.h"
#include "PlayerCamera.h"

CPlayerUpperBody::CPlayerUpperBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerUpperBody::CPlayerUpperBody(const CPlayerUpperBody& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerUpperBody::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerUpperBody::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(ADD_ChildObject()))
        return E_FAIL;

    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerUpperBody::Priority_Update(_float fDeletaTime)
{
    m_pPlayerCamera->Priority_Update(fDeletaTime);
}

void CPlayerUpperBody::Update(_float fDeletaTime)
{
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_O))
        m_iAnimIndex += 1;
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_P))
        m_iAnimIndex -= 1;

    m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime);
}

void CPlayerUpperBody::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);

    //행렬가지고 갱신
    PlayerCameraUpdate(fDeletaTime);

    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerUpperBody::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }
    return S_OK;
}

HRESULT CPlayerUpperBody::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Player_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerUpperBody::ADD_ChildObject()
{
    CPlayerCamera::CAMERA_DESC Desc;
    ZeroMemory(&Desc, sizeof(CPlayerCamera::CAMERA_DESC));

    Desc.pParent = this;
    lstrcpy(Desc.ObjectTag, L"PlayerCamera");
    
    Desc.fFar = 1000.f;
    Desc.fNear = 0.1f;
    Desc.fFov = XMConvertToRadians(60.f);

    Desc.vEye = { 0.5f, 0.f, -8.f };
    Desc.vAt = { 0.5f, 0.f, 1.f };

    m_pPlayerCamera = CPlayerCamera::Create(m_pGraphic_Device, m_pDeviceContext);
    if (FAILED(m_pPlayerCamera->Initialize(&Desc)))
        return E_FAIL;

    __super::ADD_Child(m_pPlayerCamera);
    return S_OK;
}

void CPlayerUpperBody::PlayerCameraUpdate(_float fDeletaTime)
{
    _vector vRotation{}, vScale{}, vPosition{};
    // 뼈 이름 찾아오기
    // 어깨뼈 찾아서 이녀석의 Combine행렬을 만들어내고 세팅하면 된다.
    _matrix CameraPos = m_pVIBufferCom->GetBoneTransformation("neck_01") * XMLoadFloat4x4(&m_CombinedWorldMatrix);
    
    //타겟 암이용한 위치 갱신
    m_pPlayerCamera->AttachCamera(CameraPos.r[3] + XMVectorSet(2.f, 0.f, -5.f, 0.f));
    m_pPlayerCamera->Late_Update(fDeletaTime);
}

CPlayerUpperBody* CPlayerUpperBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerUpperBody* pUpperBody = new CPlayerUpperBody(pDevice, pContext);
    if (FAILED(pUpperBody->Initalize_Prototype()))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CREATE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

CGameObject* CPlayerUpperBody::Clone(void* pArg)
{
    CPlayerUpperBody* pUpperBody = new CPlayerUpperBody(*this);
    if (FAILED(pUpperBody->Initialize(pArg)))
    {
        Safe_Release(pUpperBody);
        MSG_BOX("CLONE FAIL : PLAYER UPPER BODY");
    }
    return pUpperBody;
}

void CPlayerUpperBody::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerCamera);
}

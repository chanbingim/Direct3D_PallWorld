#include "PlayerPartData.h"

#include "GameInstance.h"
#include "PlayerCamera.h"
#include "PlayerBody.h"

CPlayerPartData::CPlayerPartData(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerPartData::CPlayerPartData(const CPlayerPartData& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerPartData::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerPartData::Initialize(void* pArg)
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

void CPlayerPartData::Priority_Update(_float fDeletaTime)
{

}

void CPlayerPartData::Update(_float fDeletaTime)
{
    m_bIsFinished = m_pVIBufferCom->PlayAnimation(m_iAnimIndex, fDeletaTime, 5.f, m_bIsAnimLoop);
}

void CPlayerPartData::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();

    //행렬가지고 갱신
    m_pPlayerBody->Late_Update(fDeletaTime);
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayerPartData::Render()
{
    _uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
       

        //어떤 파츠데이터를 사용하여 랜더링하는지를 알려준다.
        // 0 머리
        // 1 얼굴
        // 2 몸통 등 이런순서로 랜더링을 걸어가면서 하면 될거같음
        if (i == iNumMeshes - 1)
            m_pPlayerBody->Render();
        else
            m_pVIBufferCom->Render(i);

    }
    return S_OK;
}

HRESULT CPlayerPartData::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Player_Mesh"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerPartData::ADD_ChildObject()
{
    if (FAILED(ADD_AnimParts()))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayerPartData::ADD_AnimParts()
{
    CPlayerBody::PARTOBJECT_DESC Desc;
    ZeroMemory(&Desc, sizeof(CPlayerBody::PARTOBJECT_DESC));

    Desc.pParent = this;
    Desc.vScale = { 1.f, 1.f, 1.f };
    Desc.SocketMatrix = &m_CombinedWorldMatrix;
    Desc.vRotation = { 0.f, 0.f, 0.f };
    lstrcpy(Desc.ObjectTag, L"BodyPart");

    m_pPlayerBody = CPlayerBody::Create(m_pGraphic_Device, m_pDeviceContext);
    m_pPlayerBody->Initialize(&Desc);


    //m_pPlayerBody = m_pGameInstance->Clone_Prototype<CPlayerBody>(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player_Body_Default"), &Desc);
    __super::ADD_Child(m_pPlayerBody);

    return S_OK;
}

CPlayerPartData* CPlayerPartData::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerPartData* pPlayerPartData = new CPlayerPartData(pDevice, pContext);
    if (FAILED(pPlayerPartData->Initalize_Prototype()))
    {
        Safe_Release(pPlayerPartData);
        MSG_BOX("CREATE FAIL : PLAYER PARTS DATA");
    }
    return pPlayerPartData;
}

CGameObject* CPlayerPartData::Clone(void* pArg)
{
    CPlayerPartData* pPlayerPartData = new CPlayerPartData(*this);
    if (FAILED(pPlayerPartData->Initialize(pArg)))
    {
        Safe_Release(pPlayerPartData);
        MSG_BOX("CLONE FAIL : PLAYER PARTS DATA");
    }
    return pPlayerPartData;
}

void CPlayerPartData::Free()
{
    __super::Free();

    Safe_Release(m_pPlayerBody);
}

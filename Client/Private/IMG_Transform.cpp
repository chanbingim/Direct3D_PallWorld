#include "IMG_Transform.h"

#include "GameInstance.h"
#include "GameObject.h"

CIMG_Transform::CIMG_Transform() :
    CIMG_Component()
{
}

HRESULT CIMG_Transform::Prototype_Initialize()
{
    return S_OK;
}

HRESULT CIMG_Transform::Initialize(void* pArg)
{
    return S_OK;
}

void CIMG_Transform::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Transform"))
    {
        DrawTransformUI();
        ImGui::End();
    }
}

void CIMG_Transform::Bind_Data(CGameObject* pOwner, CComponent* pComponent)
{
    m_pOwner = pOwner;

    if (m_pOwner)
    {
        m_vPostion = m_pOwner->GetTransform()->GetPosition();
        m_vRotation = m_pOwner->GetTransform()->GetRotation();
        m_vSacle = m_pOwner->GetTransform()->GetScale();
    }
}

void CIMG_Transform::DrawTransformUI()
{
    if (ImGui::InputFloat3("Position", reinterpret_cast<_float*>(&m_vPostion), "%.3f"))
    {

    }
    
    if (ImGui::InputFloat3("Rotation", reinterpret_cast<_float*>(&m_vRotation), "%.3f"))
    {

    }

    if (ImGui::InputFloat3("Scale", reinterpret_cast<_float*>(&m_vSacle), "%.3f"))
    {

    }
}

CIMG_Transform* CIMG_Transform::Create()
{
    CIMG_Transform* pTransform = new CIMG_Transform;
    if (FAILED(pTransform->Prototype_Initialize()))
    {
        Safe_Release(pTransform);
        MSG_BOX("CREATE FAIL : IMG TRANSFORM");
    }
    return pTransform;
}

CImgUIBase* CIMG_Transform::Clone(void* pArg)
{
    CIMG_Transform* pTransform = new CIMG_Transform;
    if (FAILED(pTransform->Initialize(pArg)))
    {
        Safe_Release(pTransform);
        MSG_BOX("CLONE FAIL : IMG TRANSFORM");
    }
    return pTransform;
}

void CIMG_Transform::Free()
{
    __super::Free();
}

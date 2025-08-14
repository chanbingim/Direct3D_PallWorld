#include "IMG_Transform.h"

CIMG_Transform::CIMG_Transform()
{
}

HRESULT CIMG_Transform::Prototype_Initialize()
{
    return E_NOTIMPL;
}

HRESULT CIMG_Transform::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CIMG_Transform::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Transform"))
    {
        DrawTransformUI();
        ImGui::End();
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

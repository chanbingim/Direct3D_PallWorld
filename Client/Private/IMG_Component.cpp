#include "IMG_Component.h"

CIMG_Component::CIMG_Component()
{
}

HRESULT CIMG_Component::Prototype_Initialize()
{
    return E_NOTIMPL;
}

HRESULT CIMG_Component::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

void CIMG_Component::Update(_float fDeletaTime)
{
}

void CIMG_Component::Bind_Data(CGameObject* pOwner, CComponent* pComponent)
{
}

CImgUIBase* CIMG_Component::Clone(void* pArg)
{
    return nullptr;
}

void CIMG_Component::Free()
{
    __super::Free();
}

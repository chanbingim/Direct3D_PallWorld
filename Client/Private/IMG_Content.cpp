#include "IMG_Content.h"

CIMG_Content::CIMG_Content()
{
}

HRESULT CIMG_Content::Prototype_Initialize()
{
    return S_OK;
}

HRESULT CIMG_Content::Initialize(void* pArg)
{
    return S_OK;
}

void CIMG_Content::Update(_float fDeletaTime)
{
    if (ImGui::Begin("Browser"))
    {
        ImGui::End();
    }
}

CIMG_Content* CIMG_Content::Create()
{
    CIMG_Content* pContent = new CIMG_Content();
    if (FAILED(pContent->Prototype_Initialize()))
    {
        Safe_Release(pContent);
        MSG_BOX("CREATE FAIL : CONTENT");
    }
    return pContent;
}

void CIMG_Content::Free()
{
    __super::Free();
}

#include "IMG_Create.h"

CIMG_Create::CIMG_Create()
{
}

HRESULT CIMG_Create::Prototype_Initialize()
{
	return S_OK;
}

HRESULT CIMG_Create::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_Create::Update(_float fDeletaTime)
{
}

CIMG_Create* CIMG_Create::Create()
{
    CIMG_Create* pInspector = new CIMG_Create();
    if (FAILED(pInspector->Initialize(pArg)))
    {
        Safe_Release(pInspector);
        MSG_BOX("CLONE FAIL : IMG CREATE");
    }
    return pInspector;
}

CImgUIBase* CIMG_Create::Clone(void* pArg)
{
    CIMG_Create* pInspector = new CIMG_Create();
    if (FAILED(pInspector->Initialize(pArg)))
    {
        Safe_Release(pInspector);
        MSG_BOX("CLONE FAIL : IMG CREATE");
    }
    return pInspector;
}

void CIMG_Create::Free()
{
	__super::Free();
}

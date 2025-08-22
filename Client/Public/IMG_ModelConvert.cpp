#include "IMG_ModelConvert.h"

CIMG_ModelConvert::CIMG_ModelConvert() :
	CImgUIBase()
{

}

HRESULT CIMG_ModelConvert::Prototype_Initialize()
{
	return S_OK;
}

HRESULT CIMG_ModelConvert::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_ModelConvert::Update(_float fDeletaTime)
{
	bool bIsOpen = !static_cast<_uInt>(m_eVisibility);
	if (ImGui::Begin("ModelWindow", &bIsOpen))
	{
		if (!bIsOpen)
			m_eVisibility = VISIBILITY::HIDDEN;




		ImGui::End();
	}
}

CIMG_ModelConvert* CIMG_ModelConvert::Create()
{
	CIMG_ModelConvert* pModelConvert = new CIMG_ModelConvert();
	if (FAILED(pModelConvert->Prototype_Initialize()))
	{
		Safe_Release(pModelConvert);
		MSG_BOX("CREATE FAIL : Model Convert");
	}
	return pModelConvert;
}

void CIMG_ModelConvert::Free()
{
	__super::Free();
}

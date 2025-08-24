#include "EditorLevel.h"
#include "GameInstance.h"

#include "EditorCamera.h"
#include "Terrian.h"

CEditorLevel::CEditorLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt _iID) :
    CLevel(pDevice, pContext, ENUM_CLASS(LEVEL::EDITOR))
{
}

HRESULT CEditorLevel::Initialize()
{
    if (FAILED(ADD_EditorCameraLayer(L"Editor Camera Layer")))
        return E_FAIL;
    return S_OK;
}

void CEditorLevel::Update(_float fTimeDelta)
{
  
}

HRESULT CEditorLevel::Render()
{
    return S_OK;
}

HRESULT CEditorLevel::ADD_EditorCameraLayer(const WCHAR* LayerName)
{

    return S_OK;
}

CEditorLevel* CEditorLevel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uInt iID)
{
    CEditorLevel* pEditorLevel = new CEditorLevel(pDevice, pContext, iID);
    if (FAILED(pEditorLevel->Initialize()))
    {
        Safe_Release(pEditorLevel);
        MSG_BOX("CREATE FAIL : EDITOR LEVEl");
    }
    return pEditorLevel;
}

void CEditorLevel::Free()
{
    __super::Free();
}

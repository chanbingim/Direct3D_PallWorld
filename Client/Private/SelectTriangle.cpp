#include "SelectTriangle.h"

#include "GameInstance.h"

CSelectTriangle::CSelectTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CSelectTriangle::CSelectTriangle(const CSelectTriangle& rhs) :
	CBackGround(rhs)
{
}

HRESULT CSelectTriangle::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSelectTriangle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	return S_OK;
}

void CSelectTriangle::Update(_float fDeletaTime)
{
}

void CSelectTriangle::Late_Update(_float fDeletaTime)
{
	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CSelectTriangle::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CSelectTriangle::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Button_Select_Tri"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Select_Tri"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CSelectTriangle* CSelectTriangle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSelectTriangle* pSelectTri = new CSelectTriangle(pDevice, pContext);
	if (FAILED(pSelectTri->Initalize_Prototype()))
	{
		Safe_Release(pSelectTri);
		MSG_BOX("CREATE FAIL : SELECT TIRANGLE");
	}
	return pSelectTri;
}

CGameObject* CSelectTriangle::Clone(void* pArg)
{
	CSelectTriangle* pSelectTri = new CSelectTriangle(*this);
	if (FAILED(pSelectTri->Initialize(pArg)))
	{
		Safe_Release(pSelectTri);
		MSG_BOX("CLONE FAIL : SELECT TIRANGLE");
	}
	return pSelectTri;
}

void CSelectTriangle::Free()
{
	__super::Free();
}

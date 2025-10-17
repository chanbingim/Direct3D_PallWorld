#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath)
{
	m_pBatch = new SpriteBatch(m_pContext);
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

	return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pText, const _float2& vPosition, _vector vColor)
{
	m_pBatch->Begin();
	m_pFont->DrawString(m_pBatch, pText, vPosition, vColor);
	m_pBatch->End();

	return S_OK;
}

_vector CCustomFont::GetFontBoundBox(const WCHAR* pText)
{
	return m_pFont->MeasureString(pText);
}

void CCustomFont::GetSpriteSheet(ID3D11ShaderResourceView** pTexture)
{
	m_pFont->GetSpriteSheet(pTexture);
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
	CCustomFont* pFont = new CCustomFont(pDevice, pContext);
	if (FAILED(pFont->Initialize(pFontFilePath)))
	{
		Safe_Release(pFont);
		MSG_BOX("CREATE FAIL : FONT");
	}
	return pFont;
}

void CCustomFont::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);
}
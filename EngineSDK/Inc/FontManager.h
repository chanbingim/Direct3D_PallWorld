#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CCustomFont;

class CFontManager final : public CBase
{
private:
	CFontManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFontManager() = default;

public:
	HRESULT							Add_Font(const _wstring& FontTag, const _tchar* pFontFilePath);
	HRESULT							Render(const _wstring& FontTag, const _tchar* pText, const _float2& vPosition, _vector vColor);

	_vector							GetFontBoundBox(const _wstring& FontTag, const WCHAR* pText);
	void							GetSpriteSheet(const _wstring& FontTag, ID3D11ShaderResourceView** pText);

private:
	ID3D11Device*										m_pDevice = { nullptr };
	ID3D11DeviceContext*								m_pContext = { nullptr };
	unordered_map<_wstring, CCustomFont *>				m_Fonts = {};

private :
	CCustomFont*					Find_Font(const _wstring& FontTag);

public:
	static		CFontManager*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void				Free() override;

};
NS_END
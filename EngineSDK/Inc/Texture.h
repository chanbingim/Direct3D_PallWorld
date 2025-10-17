#pragma once

#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CTexture final : public CComponent
{
private :
	CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTexture(const CTexture& rhs);
	virtual ~CTexture() = default;

public:
	virtual HRESULT			Initialize(void* pArg) override;
	HRESULT					Initialize_Prototype(const WCHAR* szFilePath, _uInt TextureCnt);

	void						SetTexture(_uInt SlotNum, _uInt TexNum);
	ID3D11ShaderResourceView*	GetTexture(size_t iTextureIndex);

private :
	vector<ID3D11ShaderResourceView*>			m_pSRVvec = {};

public :
	static		CTexture*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const WCHAR* szFilePath, _uInt TextureCnt);
	virtual		CComponent* Clone(void* pArg)	override;
	virtual		void		Free()				override;

};
NS_END
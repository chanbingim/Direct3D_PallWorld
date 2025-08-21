#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CMaterial : public CBase
{
private :
	CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMaterial() = default;

public :
	HRESULT							Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial);
	
	void							SetMaterial(_uInt iMaterialIndex, aiTextureType eType, _uInt iTextureIndex);
	ID3D11ShaderResourceView*		GetMaterial(aiTextureType eType, _uInt iTextureIndex);

private :
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	vector<ID3D11ShaderResourceView*>		m_SRVs[AI_TEXTURE_TYPE_MAX];

public :
	static		CMaterial*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const aiMaterial* pAIMaterial);
	virtual		void		Free() override;

};
NS_END
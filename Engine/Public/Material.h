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
	HRESULT							Initialize(const _char* pModelFilePath, void* MaterialDesc);
	
	void							SetMaterial(_uInt iMaterialIndex, _uInt eType, _uInt iTextureIndex);
	ID3D11ShaderResourceView*		GetMaterial(_uInt eType, _uInt iTextureIndex);

	HRESULT							Export(void* pOut);

private :
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

	char							m_MatrialName[MAX_PATH] = {};

#ifdef _DEBUG
	vector<pair<_wstring ,ID3D11ShaderResourceView*>>		m_SRVs[TEXTURE_TYPE_MAX];
#elif
	vector<ID3D11ShaderResourceView*>*		m_SRVs;
#endif // _DEBUG

	

public :
	static		CMaterial*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const aiMaterial* pAIMaterial);
	static		CMaterial*  Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, void* MaterialDesc);
	virtual		void		Free() override;

};
NS_END
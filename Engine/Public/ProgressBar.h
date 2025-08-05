#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)

class CVIBuffer_Rect;
class CTexture;
class CShader;

class ENGINE_DLL CProgressBar : public CUserInterface
{
protected:
	CProgressBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProgressBar(const CProgressBar& rhs);
	virtual ~CProgressBar() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetPercent(_float fPercent);
	_float									GetPercent();

protected :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

protected :
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private :
	LPD3D11EFFECTVARIABLE					m_pShader_Percent = nullptr;
	_float									m_fPercent = {};
	
public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
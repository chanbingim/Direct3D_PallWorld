#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)

class CVIBuffer_Rect;
class CTexture;
class CShader;

class ENGINE_DLL CProgressBar : public CUserInterface
{
public :
	typedef struct Progress_Desc : public CUserInterface::GAMEOBJECT_DESC
	{
		_float4 vColor;
	}PROGRESS_DESC;

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

	void									LerpTimeReset();

protected :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

	LPD3D11EFFECTVARIABLE					m_pShader_Percent = nullptr;
	LPD3D11EFFECTVECTORVARIABLE				m_pShader_Color = nullptr;

	_float									m_fLerpAccTime = {};

	_float									m_fPrePercent = {};
	_float									m_fPercent = {};

	_float4									m_vColor = {};
	_float4									m_vPrePercentColor = {};

protected :
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual     HRESULT						Apply_ConstantShaderResources() override;

	void									LerpAnimation(_float fTimeDeleta);

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
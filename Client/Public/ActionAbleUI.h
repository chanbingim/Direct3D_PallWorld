#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
class CActor;
NS_END

NS_BEGIN(Client)
class CActionAbleUI : public CBackGround
{
public :
	typedef struct ActionAbleDesc
	{
		CActor*						pOwner;
		_float3						vScale;
		_float3						vAwayPosition;
	}ACTION_ABLE_DESC;

private:
	CActionAbleUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CActionAbleUI(const CActionAbleUI& rhs);
	virtual ~CActionAbleUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected :
	HRESULT									Apply_ConstantShaderResources();

private:
	CFontComponent*							m_pFontCom = nullptr;
	CActor*									m_pOwner = nullptr;

	_float3									m_vAwayPosition = {};
	_float2									m_vFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static		CActionAbleUI*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
#pragma once
#include "Client_Define.h"
#include "ProgressBar.h"

#include "PellStructData.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CBossTitleName;

class CBossHealthBar : public CProgressBar
{
protected:
	CBossHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossHealthBar(const CBossHealthBar& rhs);
	virtual ~CBossHealthBar() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetBossInfo(void* PellInfo);

private:
	CBossTitleName*							m_pBossName = nullptr;
	CFontComponent*							m_pMaxHealthFontCom = nullptr;
	CFontComponent*							m_pHealthFontCom = nullptr;

	PELL_INFO*								m_pPellInfo = nullptr;
	_float2									m_MaxHealthFontPos = {};
	_float2									m_HealthFontPos = {};

	_wstring								m_MaxHealthText = {};
	_wstring								m_HealthText = {};

private:
	HRESULT									ADD_Components();

public:
	static			CBossHealthBar*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
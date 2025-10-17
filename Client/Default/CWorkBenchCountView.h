#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CButtonArrow;

class CWorkBenchCountView : public CBackGround
{
protected:
	CWorkBenchCountView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkBenchCountView(const CWorkBenchCountView& rhs);
	virtual ~CWorkBenchCountView() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	CFontComponent*							m_pFontCom = nullptr;
	_Int									m_iItemCount = {};
	
	_float2									m_vFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static			CWorkBenchCountView*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
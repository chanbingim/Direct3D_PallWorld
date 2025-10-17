#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CTechSlot;
class CLevelIcon;
class CTechManager;

class CTechSlotLine : public CBackGround
{
protected:
	CTechSlotLine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechSlotLine(const CTechSlotLine& rhs);
	virtual ~CTechSlotLine() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetTechLevel(_uInt iLevel);

private:
	CLevelIcon*								m_pLevelIcon = nullptr;
	CTechManager*							m_pTechManager = nullptr;
	list<CTechSlot *>						m_pSlotList = {};

	_uInt									m_iTechLevel = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CTechSlotLine*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
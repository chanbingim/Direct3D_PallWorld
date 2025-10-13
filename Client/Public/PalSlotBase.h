#pragma once
#include "BackGround.h"
#include "PellStructData.h"

NS_BEGIN(Client)
class CItemSlotIcon;

class CPalSlotBase : public CBackGround
{
protected:
	CPalSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalSlotBase(const CPalSlotBase& rhs);
	virtual ~CPalSlotBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetSlotNumber(_uInt iSlotNum) { m_iSlotNumber = iSlotNum; }
	_uInt									GetSlotNumber() { return m_iSlotNumber; }
	
	void									SetPalInfo(const PELL_INFO& PellInfo);
	const PELL_INFO*						GetSlotPalInfo() { return m_pItemDesc; }

protected:
	virtual		void						SwapSlot(CPalSlotBase* From);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

protected:
	_uInt									m_iSlotNumber = {};
	const PELL_INFO*						m_pItemDesc = {};

	CItemSlotIcon*							m_pSlotIcon = nullptr;

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;



};


#pragma once
#include "BackGround.h"
#include "PellStructData.h"

NS_BEGIN(Client)
class CItemSlotIcon;

class CPalBoxSlot : public CBackGround
{
public :
	enum class PAL_SLOT_TYPE { BOX, INVEN, WORK, END };

	typedef struct PalBoxDesc
	{
		PAL_SLOT_TYPE				ePalSlotType;

	}PAL_BOX_DESC;

private:
	CPalBoxSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalBoxSlot(const CPalBoxSlot& rhs);
	virtual ~CPalBoxSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	_uInt									GetPalSlotNumber() { return m_iSlotNumber; }
	void									SetPalSlotNumber(_uInt iSlotNumber);

	const		PELL_INFO*					GetSlotPalInfo() { return m_pPalInfo; }


	const PAL_SLOT_TYPE						GetPalSlotType() { return m_ePalSlotType; }
	
protected:
	virtual		void						SwapSlot(CPalBoxSlot* From);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

private:
	_uInt									m_iSlotNumber = {};
	const PELL_INFO*						m_pPalInfo = {};
	CItemSlotIcon*							m_pSlotIcon = nullptr;

	PAL_SLOT_TYPE							m_ePalSlotType = {};

private:
	HRESULT									ADD_Components();

	void									LoadSlotPalInfo(PAL_SLOT_TYPE eSlotType, _uInt SlotNumber, PELL_INFO* pOutPellInfo);


public:
	static		CPalBoxSlot*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
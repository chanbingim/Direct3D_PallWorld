#pragma once

#include "BackGround.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)
class CSlotImage;

class CSlotBase : public CBackGround
{
public :
	enum class SLOT_TYPE { EQUIP, ITEM, QUICKSLOT, END};

protected:
	CSlotBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, SLOT_TYPE eType);
	CSlotBase(const CSlotBase& rhs);
	virtual ~CSlotBase() = default;

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
	virtual		void						SwapSlot(CSlotBase* To);
	virtual		void						UseSlot(void* pArg);

	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

protected :
	CSlotImage*								m_pSlotImage = nullptr;

	SLOT_TYPE								m_eSlotType = {};
	_uInt									m_iItemID = {};
		
private :
	HRESULT									CreateSlotImage();

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
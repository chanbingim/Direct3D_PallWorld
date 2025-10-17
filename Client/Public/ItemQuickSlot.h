#pragma once
#include "BackGround.h"

NS_BEGIN(Client)

class CItemQuickSlotIcon;
class CItemQuickSlotName;
class CItemQuickSlotCount;

class CItemQuickSlot : public CBackGround
{
protected:
	CItemQuickSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemQuickSlot(const CItemQuickSlot& rhs);
	virtual ~CItemQuickSlot() = default;

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
	CItemQuickSlotIcon*						m_pQuickSlotIcon = nullptr;
	CItemQuickSlotName*						m_pQuickSlotName = nullptr;
	CItemQuickSlotCount*					m_pQuickSlotCount = nullptr;

	_float4									m_vImageColor;
private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static			CItemQuickSlot*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
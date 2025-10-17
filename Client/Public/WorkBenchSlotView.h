#pragma once
#include "Client_Define.h"
#include "TechStruct.h"
#include "ItemStruct.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CWorkBenchSlot;

class CWorkBenchSlotView : public CUserInterface
{
public :
	typedef struct WorkBenchSlotViewDesc : public GAMEOBJECT_DESC
	{
		_uInt				iSlotCount;
	}WORK_BENCH_SLOT_VIEW_DESC;

protected:
	CWorkBenchSlotView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkBenchSlotView(const CWorkBenchSlotView& rhs);
	virtual ~CWorkBenchSlotView() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetViewItemList(const vector<_uInt>& ItemList);

private:
	_uInt									m_iItemCount = {};
	vector<CWorkBenchSlot*>					m_ItemSlot = {};

private:
	HRESULT									ADD_Components();

public:
	static			CWorkBenchSlotView*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
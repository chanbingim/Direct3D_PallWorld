#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CItemQuickSlotCount final : public CUserInterface
{
protected:
	CItemQuickSlotCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItemQuickSlotCount(const CItemQuickSlotCount& rhs);
	virtual ~CItemQuickSlotCount() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetItemIndex(_Int iLoadedCount, _Int iItemTotalCOunt);

private:
	CFontComponent*							m_pFontCom = nullptr;

	WCHAR									m_szTopVeiwText[10] = { L"무한대" };
	WCHAR									m_szUnderVeiwText[10] = { L"무한대" };

	_float2									m_fFontPoint = {};

private:
	HRESULT									ADD_Components();

public:
	static			CItemQuickSlotCount*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};


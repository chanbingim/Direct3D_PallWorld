#pragma once
#include "Client_Define.h"
#include "UserInterFace.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CEquipSlotTittle : public CUserInterface
{
protected:
	CEquipSlotTittle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEquipSlotTittle(const CEquipSlotTittle& rhs);
	virtual ~CEquipSlotTittle() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;
	void									SetText(const WCHAR* szTitle);

private:
	CFontComponent*							m_pFontCom = nullptr;
	_wstring								m_szTitle = {};
	_float2									m_vTitleFontPos = {};

private:
	HRESULT									ADD_Components();

public:
	static			CEquipSlotTittle*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CItemSlot;

class CInventory : public CBackGround
{
private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& rhs);
	virtual ~CInventory() = default;

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
	HRESULT									Bind_ShaderResources();
	HRESULT									Apply_ConstantShaderResources();

private:
	_float2									m_SlotCount = {};
	vector<CItemSlot*>						m_pItemSlot = {};

	LPD3D11EFFECTVECTORVARIABLE				m_pInven_Color = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CInventory*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
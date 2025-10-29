#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class CEquipSlotTittle;
class CPlayerView;
class CEquipSlot;

class CEquipment : public CBackGround
{
private :
	CEquipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEquipment(const CEquipment& rhs);
	virtual ~CEquipment() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected:
	HRESULT									Bind_ShaderResources();
	HRESULT									Apply_ConstantShaderResources();

private :
	_uInt									m_SlotCount = {};
	vector<CEquipSlot*>						m_pEquipSlot = {};

	CEquipSlotTittle*						m_pSlotFontCom[7];

	CPlayerView*							m_pPlayerView = nullptr;	
	LPD3D11EFFECTVECTORVARIABLE				m_pEquip_Color = {};

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

	HRESULT									CreateEquipSlot(_uInt iCount, _uInt iSlotType, _float3 vStartPos, _uInt MajorType);

public :
	static		CEquipment*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
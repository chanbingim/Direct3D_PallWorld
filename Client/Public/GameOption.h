#pragma once
#include "BackGround.h"

NS_BEGIN(Client)
class COptionButton;

class CGameOption final : public CBackGround
{
private:
	CGameOption(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CGameOption(const CGameOption& rhs);
	virtual ~CGameOption() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetActive(_bool flag);
	_bool									IsActive();

private :
	_bool									m_bIsActive = false;

	vector<COptionButton*>					m_OptionButton = {};
	//원래 다른버튼도있는데 그건 없어도 될것 같음

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

	void									GameQuit();

public:
	static		CGameOption*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END
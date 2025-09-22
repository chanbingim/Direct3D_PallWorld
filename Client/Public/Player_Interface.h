#pragma once

#include "Client_Define.h"
#include "Client_Struct.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CProgressBar;
NS_END

NS_BEGIN(Client)

class	CCompass;
class	CHealthBar;

class CPlayer_Interface final : public CUserInterface
{
protected:
	CPlayer_Interface(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPlayer_Interface(const CPlayer_Interface& rhs);
	virtual ~CPlayer_Interface() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CHealthBar*								m_pHpBar = nullptr;
	CHealthBar*								m_pHugerBar = nullptr;
	CProgressBar*							m_pGuardBar = nullptr;
	CCompass*								m_pCompass = nullptr;

	float									m_Per = { };

private :
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CPlayer_Interface*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
	 
};
NS_END
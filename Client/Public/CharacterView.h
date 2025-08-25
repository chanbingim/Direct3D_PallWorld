#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CInventory;
class CEquipment;

class CCharacterView : public CUserInterface
{
protected:
	CCharacterView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacterView(const CCharacterView& rhs);
	virtual ~CCharacterView() = default;

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
	CInventory*								m_pInventory = nullptr;
	CEquipment*								m_pEquipMent = nullptr;

private:
	HRESULT									ADD_Childs();

public:
	static		CCharacterView*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CItemSlotIcon;

class CWorkBenchSlot : public CUserInterface
{
private:
	CWorkBenchSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkBenchSlot(const CWorkBenchSlot& rhs);
	virtual ~CWorkBenchSlot() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected:
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHovering();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();
	virtual		void						MouseButtonPressed();
	virtual		void						MouseButtonUp();

private :
	CItemSlotIcon*							m_pItemIcon = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static		CWorkBenchSlot*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
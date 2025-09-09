#pragma once

#include "Client_Define.h"
#include "ContainerObject.h"

NS_BEGIN(Client)
class CPellStateMachine;

class CPellBase : public CContainerObject
{
protected:
	CPellBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CPellBase(const CPellBase& rhs);
	virtual ~CPellBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected :
	CPellStateMachine*						m_pPellFsm = nullptr;


public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
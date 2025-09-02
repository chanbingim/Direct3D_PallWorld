#pragma once

#include "PellBase.h"

NS_BEGIN(Client)
class CYeti : public CPellBase
{
protected:
	CYeti(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CYeti(const CYeti& rhs);
	virtual ~CYeti() = default;

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

private:
	HRESULT									ADD_PartObjects();

public:
	static			CYeti*					Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
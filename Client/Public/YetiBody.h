#pragma once
#include "PellBody.h"

NS_BEGIN(Client)
class CYetiBody : public CPellBody
{
protected:
	CYetiBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CYetiBody(const CYetiBody& rhs);
	virtual ~CYetiBody() = default;

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
	HRESULT									ADD_Components();

public:
	static			CYetiBody*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
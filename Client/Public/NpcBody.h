#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CNpcBody : public CPartObject
{
protected:
	CNpcBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNpcBody(const CNpcBody& rhs);
	virtual ~CNpcBody() = default;

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

	void									PlayAnimation(const char* szAnimName, _bool bIsLoop = true);
	void									ResetCurrentAnimation();
	_bool									FinishedAnimation();

protected:
	_bool									m_bIsAnimFinished = false;


public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
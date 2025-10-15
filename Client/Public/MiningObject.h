#pragma once
#include "Enviormnent.h"

NS_BEGIN(Client)
class CMiningObject : public CEnviormnent
{
protected:
	CMiningObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMiningObject(const CMiningObject& rhs);
	virtual ~CMiningObject() = default;

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

	_bool									IsMiningFinished() const { return m_bIsCompleted; }
	void									PlusWorkSpeed(_float fWorkSpeed);
	void									SubWorkSpeed(_float fWorkSpeed);

protected :
	_float									m_fCompleteTime = {};

private :
	_bool									m_bIsCompleted = false;
	_float									m_fAccComplteTime = {};
	_float									m_fAccTime = {};
	
	_float									m_fWorkSpeed = {};

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
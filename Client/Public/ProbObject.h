#pragma once
#include "NoneAnimMesh.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CPlayer;

class CProbObject abstract : public CNoneAnimMesh
{
protected:
	CProbObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CProbObject(const CProbObject& rhs);
	virtual ~CProbObject() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype();
	virtual		HRESULT						Initialize(void* pArg);

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime);
	virtual		void						Update(_float fDeletaTime);
	virtual		void						Late_Update(_float fDeletaTime);

	// 랜더
	virtual		HRESULT						Render();

protected :
	const		CPlayer*					m_pPlayer = nullptr;
	CCollision*								m_pCollision = nullptr;
	_float									m_fActionDistance = { 10.f };
	
public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
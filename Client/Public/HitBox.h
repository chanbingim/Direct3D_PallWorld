#pragma once
#include "Client_Define.h"
#include "Client_Struct.h"
#include "PartObject.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CHitBox : public CPartObject
{
public :
	typedef struct HitBoxDesc : public PartObject_Desc
	{
		const CGameObject*				pOwner;
		const _float4x4*				pOwnerWorldMat;
	}HIT_BOX_DESC;

protected:
	CHitBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHitBox(const CHitBox& rhs);

	virtual ~CHitBox() = default;
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

	void									StartAttack(_float fDamage);
	void									UpdateAttack(_float fDamage);

protected :
	virtual	void							HitBegin(_float3 vDir, CGameObject* pHitActor);

private :
	const	CGameObject*					m_pOwner = nullptr;
	const	_float4x4*						m_pParentWorldMat = nullptr;

	CCollision*								m_pCollision = nullptr;
	_float									m_fHitBoxDamage = {};

private:
	HRESULT									ADD_Components(_float3 vScale);
	
public:
	static			CHitBox*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
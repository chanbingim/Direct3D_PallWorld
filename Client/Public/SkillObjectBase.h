#pragma once
#include "Client_Define.h"
#include "Actor.h"
#include "PellSkillStruct.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CPellBase;

class CSkillObjectBase abstract : public CActor
{
public :
	typedef struct SkillObjectInitDesc 
	{
		_uInt				iSkillID;
	}SKILL_OBJECT_INIT_DESC;

	typedef struct SkillObjectDesc : public GAMEOBJECT_DESC
	{
		CPellBase*			pOwner;
		_float3				vTargetDir;
	}SKILL_OBJECT_DESC;

protected:
	CSkillObjectBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkillObjectBase(const CSkillObjectBase& rhs);
	virtual ~CSkillObjectBase() = default;

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
	const CGameObject*						GetOwnerPell() { return m_pOwner; }

protected:
	_uInt									m_iNumSkill = {};
	CGameObject*							m_pOwner = nullptr;
	PAL_SKILL_NETWORK_DATA					m_SkillData = {};

	_float3									m_vTargetDir = {};
	_float									m_fTickDamageTime = {};
	_float									m_fAccTickDamageTime = {};
	_bool									m_bIsHitTick = {};

	CCollision*								m_pCollision = nullptr;
	_bool									m_bIsDissolve = false;
	list<CGameObject*>						m_pSkillEffects = {};

protected :
	virtual			void					HitOverlapEvent(_float3 vDir, CGameObject* pHitObject);
	virtual			void					HitOverlapping(_float3 vDir, CGameObject* pHitObject);
	virtual			void					HitOverlapEnd(_float3 vDir, CGameObject* pHitObject);

	void									EffectDead();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
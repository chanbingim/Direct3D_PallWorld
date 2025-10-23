#pragma once
#include "Client_Define.h"
#include "Actor.h"
#include "PellSkillStruct.h"

NS_BEGIN(Engine)
class CEffectContatiner;
class CCollision;
NS_END

NS_BEGIN(Client)
class CSkillObjectBase abstract : public CActor
{
public :
	typedef struct SkillObjectDesc  : public GAMEOBJECT_DESC
	{
		_uInt				iSkillID;
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

protected:
	_uInt									m_iNumSkill = {};
	PAL_SKILL_NETWORK_DATA					m_SkillData = {};

	_float3									m_vTargetDir = {};
	_float									m_fTickDamageTime = {};
	_float									m_fAccTickDamageTime = {};
	_bool									m_bIsHitTick = {};

	CCollision*								m_pCollision = nullptr;
	list<CEffectContatiner*>				m_pSkillEffects = {};

private :
	virtual			void					HitOverlapEvent(_float3 vDir, CGameObject* pHitObject);
	virtual			void					HitOverlapping(_float3 vDir, CGameObject* pHitObject);
	virtual			void					HitOverlapEnd(_float3 vDir, CGameObject* pHitObject);

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
#pragma once

#include "Client_Define.h"
#include "AnimEntity.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CDefaultEntity final : public CAnimEntity
{
public :
	typedef struct Default_Desc : public CAnimEntity::ANIM_DESC
	{
		CGameObject*				pOwner;
	}DEFAULT_DESC;

public :
	CDefaultEntity();
	virtual ~CDefaultEntity() = default;

public:
	//초기화
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update() override;
	virtual		void						Update() override;
	virtual		void						Late_Update() override;

	// 0 : Scale | 1 : Rotation | 2 : POSITION
	void									SetEnitityState(_uInt iType, _float3 vStart, _float3 vEnd);

	// 0 : Scale | 1 : Rotation | 2 : POSITION
	void									SetFlag(_uInt iFlag);

private :
	CGameObject*							m_pOwner = nullptr;

	_float3									m_StartTrasnform[3];
	_float3									m_EndTransform[3];
	_bool									m_ApplyFlag[3];

public:
	virtual			void					Free() override;
};
NS_END
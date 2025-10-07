#pragma once
#include "NoneAnimMesh.h"
#include "ItemStruct.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CActionAbleUI;

class CArchitecture : public CNoneAnimMesh
{
protected:
	CArchitecture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CArchitecture(const CArchitecture& rhs);
	virtual ~CArchitecture() = default;

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
	virtual		void						ArchitectureAction() {}
	
	_bool									IsBuildFinished() const { return m_bIsCompleted; }



	void									PlusWorkSpeed(_float fWorkSpeed);
	void									SubWorkSpeed(_float fWorkSpeed);

protected:
	_uInt									m_ItemID = {};
	ITEM_DESC								m_pArchitectureInfo = {};

	//OBB 박스가 달려있을거임
	CCollision*								m_pHitBoxCollision = nullptr;
	CActionAbleUI*							m_pActionUI = nullptr;

	_bool									m_bIsCompleted = false;
	_float									m_fAccComplteTime = {};
	_float									m_fAccTime = {};
	_float									m_fWorkSpeed = {};

	_float									m_fMaxHeight = {};
	_float									m_fActionDistance = {};
	_float4									m_vPercentColor = {};

protected :
	virtual HRESULT							Apply_ConstantShaderResources(_uInt iMeshIndex);
	virtual void							HitOverlapFunction(_float3 vDir, CGameObject* pHitActor);

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
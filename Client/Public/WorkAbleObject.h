#pragma once

#include "NoneAnimMesh.h"
#include "PellStructData.h"

NS_BEGIN(Client)
class CWorkAbleObject abstract : public CNoneAnimMesh
{
protected:
	CWorkAbleObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkAbleObject(const CWorkAbleObject& rhs);
	virtual ~CWorkAbleObject() = default;

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
	PELL_WORK_TYPE							GetWorkAbleType() const { return m_eWorkType; }

protected:
	PELL_WORK_TYPE							m_eWorkType = {};

protected :
	virtual		HRESULT						DeadFunction();

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
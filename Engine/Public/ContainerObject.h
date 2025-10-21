#pragma once
#include "Actor.h"

NS_BEGIN(Engine)
class CPartObject;

class ENGINE_DLL CContainerObject : public CActor
{
protected:
	CContainerObject(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CContainerObject(const CContainerObject& rhs);
	virtual ~CContainerObject() = default;

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

	void											SetAnimIndex(_uInt iIndex, _float fAnimSpeed);
	CPartObject*									FindPartObject(const _wstring& PartObjectTag);
	const 	unordered_map<_wstring, CPartObject*>* GetAllPartObejcts() { return &m_PartObjects; }


protected:
	unordered_map<_wstring, CPartObject*>	m_PartObjects;

protected :
	HRESULT									AddPartObject(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartTag, void* pArg = nullptr);

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
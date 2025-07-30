#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CGameObject;

class CLayer : public CBase
{
private :
	CLayer() = default;
	virtual ~CLayer() = default;

public:
	HRESULT						Initialize_Layer();

	//업데이트	
	void						Priority_Update(_float fDeletaTime);
	void						Update(_float fDeletaTime);
	void						Late_Update(_float fDeletaTime);

	HRESULT						ADD_GameObject(CGameObject* pGameObject);
	CGameObject*				Find_GameObject(_uInt Index = 0);
	const list<CGameObject*>*	GetAllObjects();

	void						Clear_DeadObject();
	void						Clear_Resource();

private:
	list<CGameObject*>			m_pGameObjects;

public:
	static	CLayer*				Create();
	virtual void				Free()		override;

};
NS_END
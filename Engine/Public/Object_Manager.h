#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CLayer;
class CGameObject;

class CObject_Manager final : public CBase
{
private :
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public :
	HRESULT						Initialize(_uInt _iLevelCnt);

	//업데이트	
	virtual		void			Priority_Update(_float fDeletaTime);
	virtual		void			Update(_float fDeletaTime);
	virtual		void			Late_Update(_float fDeletaTime);

	/* 원형을 찾고 복제하여 레이어에 추가한다. */
	HRESULT											Add_GameObject_ToLayer(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uInt iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT											Add_GameObject_ToLayer(_uInt iLayerLevelIndex, const _wstring& strLayerTag, CGameObject* pObject);
	const list<CGameObject*>*						GetAllObejctToLayer(_uInt iLayerIndex, const _wstring& LayerTag);
	void											Clear_Resource(_uInt iLevelIndex);
	void											Clear_DeadObject();
	const unordered_map<_wstring, CLayer*>*			GetLayer(_uInt iLevelID);

private:
	_uInt											m_iLevelNum = {};
	CGameInstance*									m_pGameInstance = { nullptr };
	unordered_map<_wstring, CLayer*>*				m_pLayers = { nullptr };

private :
	CLayer*											Find_Layer(_uInt iLevelIndex, const _wstring LayerTag);

public :
	static		CObject_Manager*					Create(_uInt _iLevelCnt);
	virtual		void								Free() override;

};
NS_END
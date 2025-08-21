#pragma once
#include "FunctionSet.h"
#include "RenderSet.h"

NS_BEGIN(Engine)
class CPrototype_Manager : public CBase
{
private :
	CPrototype_Manager() = default;
	virtual ~CPrototype_Manager() = default;

public :
	HRESULT			Initialize(_uInt _iLevelNum);

	HRESULT			Add_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag, CBase* pPrototype);
	
	template<typename T>
	T*				Clone_Prototype(_uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg);

	void			Clear_Resource(_uInt iLevelIndex);

	template<typename T>
	void			GetPrototypeList(_uInt iLevelIndex, list<pair<_wstring, T*>>* pOutList);

private :
	unordered_map<_wstring, CBase*>*	m_Prototypes = nullptr;
	_uInt								m_iLevelCnt = {};

private :
	CBase*			Find_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag);

public :
	static		CPrototype_Manager*		Create(_uInt iLevelNum);
	virtual		void					Free() override;

};
NS_END

template<typename T>
inline T* CPrototype_Manager::Clone_Prototype(_uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CBase* pPrototype = Find_Prototype(iLevelIndex, strPrototypeTag);
	if (nullptr == pPrototype)
		return nullptr;

	T* pGameObject = { nullptr };
	pGameObject = dynamic_cast<T*>(pPrototype);
	if (nullptr == pGameObject)
		return nullptr;

	return static_cast<T*>(pGameObject->Clone(pArg));
}

template<typename T>
inline void CPrototype_Manager::GetPrototypeList(_uInt iLevelIndex, list<pair<_wstring, T*>>* pOutList)
{
	for (auto& pair : m_Prototypes[iLevelIndex])
	{
		T* pObject = dynamic_cast<T*>(pair.second);
		if (pObject)
		{
			(*pOutList).push_back(make_pair(pair.first, pObject));
		}
	}
}

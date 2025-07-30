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
	pGameObject = dynamic_cast<T*>(pPrototype)->Clone(pArg);
	if (nullptr == pGameObject)
		return nullptr;

	return pGameObject;
}

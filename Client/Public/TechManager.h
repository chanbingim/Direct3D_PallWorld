#pragma once
#include "Base.h"
#include "Client_Define.h"
#include "TechStruct.h"

NS_BEGIN(Client)
class CTechManager final : public CBase
{
	DECLARE_SINGLETON(CTechManager);

private:
	CTechManager() = default;
	virtual ~CTechManager() = default;

public:
	HRESULT									Initialize(const char* FilePath = "");
	void									LernTechObject(_uInt TechItemID);

	const WCHAR*							GetTechTypeToString(TECH_TYPE eTechType);


	const TECH_ITEM_DESC&					GetTechData(_uInt iTechID);
	const list<_uInt>&						GetCategoryLevelTechList(_uInt iLevel);
	const list<_uInt>&						GetCategoryTypeTechList(TECH_TYPE eType);
	const list<_uInt>&						GetLearnTechList(TECH_TYPE eType);

private:
	vector<TECH_ITEM_DESC>					m_emptyTechData;

	// 맵을 통해서 아이템 인덱스와 아이템 구조체를 가지고있자.
	map<_uInt, list<_uInt>>					m_LevelTechItems;
	map<TECH_TYPE, list<_uInt>>				m_TypeTechItems;

	map<_uInt, TECH_ITEM_DESC>				m_TechItems;
	map<TECH_TYPE, list<_uInt>>				m_LearnTechItems;

private:
	HRESULT									LoadItemData(const char* FilePath);
	HRESULT									ParseTechData(vector<_string>& Data);

public:
	virtual		void						Free() override;

};
NS_END
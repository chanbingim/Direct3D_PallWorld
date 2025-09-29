#include "TechManager.h"

#include "CsvHelper.h"

IMPLEMENT_SINGLETON(CTechManager);

HRESULT CTechManager::Initialize(const char* FilePath)
{
    LoadItemData(FilePath);
    return S_OK;
}

void CTechManager::LernTechObject(_uInt TechItemID)
{
    m_TechItems[TechItemID].LearnTech = true;
}

const WCHAR* CTechManager::GetTechTypeToString(TECH_TYPE eTechType)
{
    switch (eTechType)
    {
    case TECH_TYPE::ITEM:
        return TEXT("아이템");
    case TECH_TYPE::ARCHITECTURE:
        return TEXT("건축물");
    }

    return TEXT("");
}

const TECH_ITEM_DESC& CTechManager::GetTechData(_uInt iLevel)
{
    return m_TechItems[iLevel];
}

const list<_uInt>& CTechManager::GetCategoryLevelTechList(_uInt iLevel)
{
    return m_LevelTechItems[iLevel];
}

const list<_uInt>& CTechManager::GetCategoryTypeTechList(TECH_TYPE eType)
{
    return m_TypeTechItems[eType];
}

HRESULT CTechManager::LoadItemData(const char* FilePath)
{
    vector<_string> Data;
    CSV_Read(FilePath, Data);
    ParseTechData(Data);
    return S_OK;
}

HRESULT CTechManager::ParseTechData(vector<_string>& Data)
{
    vector<TECH_ITEM_DESC> emptyTechList;
    emptyTechList.reserve(12);

    list<_uInt> ItemList;
    for (_uInt i = 0; i < 6; i++)
        m_LevelTechItems.emplace(i, ItemList);

    for (_uInt i = 0; i < ENUM_CLASS(TECH_TYPE::END); i++)
        m_TypeTechItems.emplace(TECH_TYPE(i), ItemList);


    TECH_ITEM_DESC TECH_ITEM_DESC;
    for (size_t i = 8; i < Data.size();)
    {
        _uInt iNumTechItem = atoi(Data[i++].c_str());
        TECH_ITEM_DESC.TechTypeID = atoi(Data[i++].c_str());
        TECH_ITEM_DESC.TechType = TECH_TYPE(atoi(Data[i++].c_str()));

        for (_uInt j = 0; j < iNumTechItem; ++j)
            TECH_ITEM_DESC.IngredientItemIDs.push_back(atoi(Data[i++].c_str()));

        for (_uInt j = 0; j < iNumTechItem; ++j)
            TECH_ITEM_DESC.IngredientItemCounts.push_back(atoi(Data[i++].c_str()));

        TECH_ITEM_DESC.NecessaryLevel = atoi(Data[i++].c_str());
        TECH_ITEM_DESC.NecessaryPoint = atoi(Data[i++].c_str());
        TECH_ITEM_DESC.ReturnItemID = atoi(Data[i++].c_str());

        if (TECH_ITEM_DESC.NecessaryLevel <= 0)
            TECH_ITEM_DESC.LearnTech = true;

        m_LevelTechItems[TECH_ITEM_DESC.NecessaryLevel].push_back(TECH_ITEM_DESC.TechTypeID);
        m_TypeTechItems[TECH_ITEM_DESC.TechType].push_back(TECH_ITEM_DESC.TechTypeID);
        m_TechItems.emplace(TECH_ITEM_DESC.TechTypeID, TECH_ITEM_DESC);
    }

    return S_OK;
}

void CTechManager::Free()
{

}

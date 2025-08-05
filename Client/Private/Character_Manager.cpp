#include "Character_Manager.h"

IMPLEMENT_SINGLETON(CCharacter_Manager);

void CCharacter_Manager::Initialize()
{
}

const CHARACTER_DESC* CCharacter_Manager::GetCharacterData(_uInt iID)
{
    auto pair = m_CharacterInfo.find(iID);
    if (pair == m_CharacterInfo.end())
        return nullptr;

    return pair->second;
}

void CCharacter_Manager::Free()
{
    for (auto& pair : m_CharacterInfo)
        Safe_Delete(pair.second);

    m_CharacterInfo.clear();
}

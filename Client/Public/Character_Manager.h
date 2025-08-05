#pragma once
#include "Base.h"
#include "Client_Define.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CCharacter_Manager final : public CBase
{
	DECLARE_SINGLETON(CCharacter_Manager);

private:
	CCharacter_Manager() = default;
	virtual ~CCharacter_Manager() = default;

public:
	void					Initialize();

	const CHARACTER_DESC*	GetCharacterData(_uInt iID);

private:
	unordered_map<_uInt, CHARACTER_DESC *> m_CharacterInfo;

public:
	virtual		void		Free() override;
};
NS_END
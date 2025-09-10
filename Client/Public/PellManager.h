#pragma once
#include "Base.h"
#include "Client_Define.h"
#include "PellStructData.h"

NS_BEGIN(Client)
class CPellManager final : public CBase
{
	DECLARE_SINGLETON(CPellManager);

private :
	CPellManager();
	virtual ~CPellManager() = default;

public :
	void									Initialize(const char* szFilePath = "");
	const PELL_SAVE_DATA*					FindPellData(_uInt iID);

private :
	unordered_map<_uInt, PELL_SAVE_DATA>	m_PellDatas;

public :
	virtual void				Free() override;

};
NS_END
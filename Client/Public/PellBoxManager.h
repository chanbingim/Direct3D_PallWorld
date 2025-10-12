#pragma once
#include "Client_Define.h"
#include "Base.h"

NS_BEGIN(Client)
class CPellBoxManager : public CBase
{
	DECLARE_SINGLETON(CPellBoxManager);

private :
	CPellBoxManager();
	virtual ~CPellBoxManager() = default;

private :
	// 여기서 펠박스 근처에 있는 오브젝트들을 여기다 담아두고 작업
	// 작업목록이 있다는말


	// 이동가능 셀들도 여기다가 보관
	// 내가 우리집안에 꺼내 놓은 펠들은 여기 셀만 탐색가능


public :
	virtual		void		Free()	override;
	
};
NS_END
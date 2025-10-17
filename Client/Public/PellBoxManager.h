#pragma once
#include "Client_Define.h"
#include "PellStructData.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
class CGameObject;
NS_END

NS_BEGIN(Client)
class CPellBase;
class CPalBox;

class CPellBoxManager final : public CBase
{
	DECLARE_SINGLETON(CPellBoxManager);

private :
	CPellBoxManager();
	virtual ~CPellBoxManager() = default;

public :
	void						Initialize_Manager();

	// 매니저에 펠박스를 등록한다.
	void						Add_PalBox(CPalBox*	pPalBox);
	void						Remove_PalBox(CPalBox* pPalBox);

#pragma region PalBox Function
	// 다른곳에서는 이함수를 호출해서 넣기만하고
	// 내부적인 동작은 매니저에서 수행해서 뿌려주자
	void						Add_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject);
	void						Remove_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject);

#pragma region PalBox
	// 펠박스에 펠을 보관하는 함수
	// 꺼낼수도 있다.
	void						StorePalBox(const PELL_INFO& PellInfo, _Int iStoreID = -1);
	const PELL_INFO*			GetPalBoxInfo(_uInt iStoreID);
	
	// 특정 번호 ID에 맞는 펠을 꺼내서 반환
	_bool						LoadPalBox(_uInt iStoreID, PELL_INFO* pOutPalInfo);

#pragma endregion
	
#pragma region Work Func
	// 내가 원하는 작업중인 펠리스트에 추가한다.
	void						Add_WorkPalList(_uInt iStoreID);
	void						Add_WorkPalList(const PELL_INFO& PellInfo, _Int WorkSlotID = -1);
	const PELL_INFO*			GetWorkPalInfo(_uInt iWorkIndex);

	// 내가 원하는 작업중인 펠리스트에서 제거한다..
	void						Remove_WorkPalList(CPellBase* pPellBase);
	_bool						Load_WorkPalList(_uInt iStoreID, PELL_INFO* pOutPalInfo);
#pragma endregion

	void						SwapPalBox(_uInt iSlotType, _uInt iToSlotIndex, _uInt iFromSlotIndex);
#pragma endregion

	// 펠박스의 UI도 여기서 관리하자
	// 특정 펠박스의 포인터를 넣으면 UI 조차 여기서 표현해주자
	void						ShowPalBoxUserInterface(CPalBox* pPalBox);

private :
	CGameInstance*							m_pGameInstance = {};
	// 이동가능 셀들은 펠박스의 설치 위치를 알아야할거같음 여기다가 보관
	// 내가 우리집안에 꺼내 놓은 펠들은 여기 셀만 탐색가능
	list<CPalBox*>							m_PalBoxList = {};
	CPalBox*								m_pSelectPalBox = nullptr;

public :
	virtual		void			Free()	override;
	
};
NS_END
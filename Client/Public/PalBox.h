#pragma once
#include "Architecture.h"
#include "PellStructData.h"

NS_BEGIN(Client)
class CPellBase;

class CPalBox : public CArchitecture
{
public :
	typedef struct		PalBoxDesc
	{
		_float			fAreaRadius;
	}PALBOX_DESC;

protected:
	CPalBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalBox(const CPalBox& rhs);
	virtual ~CPalBox() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;
	virtual		void						ArchitectureAction() override;

	_float									GetPalBoxArea() { return m_fAreaRadius; }

#pragma region PalBox Function
	// 다른곳에서는 이함수를 호출해서 넣기만하고
	// 내부적인 동작은 매니저에서 수행해서 뿌려주자
	void						Add_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject);
	void						Remove_JobListObject(PELL_WORK_TYPE eWorkType, CGameObject* pObject);

#pragma region PalBox
	// 펠박스에 펠을 보관하는 함수
	// 꺼낼수도 있다.
	void						StorePalBox(const PELL_INFO& PellInfo, _Int iStoreID);
	const PELL_INFO*			GetPalBoxInfo(_uInt iStoreID);

	// 특정 번호 ID에 맞는 펠을 꺼내서 반환
	HRESULT						LoadPalBox(_uInt iStoreID, PELL_INFO* pOutPalInfo);

#pragma endregion

#pragma region Work Func
	// 내가 원하는 작업중인 펠리스트에 추가한다.
	void						Add_WorkPalList(_uInt iStoreID);
	void						Add_WorkPalList(const PELL_INFO& PellInfo, _Int WorkSlotID);
	const PELL_INFO*			GetWorkPalInfo(_uInt iWorkIndex);

	// 내가 원하는 작업중인 펠리스트에서 제거한다..
	void						Load_WorkPalList(_uInt iStoreID, PELL_INFO* pOutPalInfo);

	// 내가 원하는 작업중인 펠리스트에서 제거한다..
	void						Remove_WorkPalList(CPellBase* pPellBase);
#pragma endregion

	// 서로 같은 슬롯에 데이터를 스왑할때
	void						SwapPalBox(_uInt iSlotType, _uInt iToSlotIndex, _uInt iFromSlotIndex);
#pragma endregion

protected :
	virtual void							HitOverlapFunction(_float3 vDir, CGameObject* pHitActor) override;

private :
	_float													m_fAreaRadius = {};
	
	// 펠박스가 자기 기준으로 안쪽에있는 오브젝트를 보관하고있자
	unordered_map<PELL_WORK_TYPE, list<CGameObject*>>		m_JobList = {};

	// 펠박스에 넣어둔 펠
	_uInt													m_iMaxBoxInPell = {};
	vector<pair<PELL_INFO, _bool>>							m_BoxInPellInfo = {};

	// 작업중인 내 영역안의 펠
	_uInt													m_iMaxSpawnPell = {};
	vector<CPellBase*>										m_SpawnPells = {};

private :
#ifdef _DEBUG
	CCollision*												m_pDebugSphere = nullptr;
#endif // _DEBUG

private :
	HRESULT									ADD_Components();

	

public:
	static			CPalBox*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
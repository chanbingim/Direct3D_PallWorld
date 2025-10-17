#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CSpawnBox : public CGameObject
{
public :
	typedef struct SpawnBoxDesc : GAMEOBJECT_DESC
	{
		WCHAR			SpawnPrototypeName[MAX_PATH];
		_float			fSpawnTime;

		_Int			iCellIndex;
		_float			fMaxSpawnCount;
		_bool			bIsDestroySpawner;
	}SPAWN_BOX_DESC;

private :
	CSpawnBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpawnBox(const CSpawnBox& rhs);
	virtual ~CSpawnBox() = default;

public :	//초기화
	virtual		HRESULT				Initalize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;

	void							Priority_Update(_float fDeletaTime) override;
	void							Update(_float fDeletaTime) override;
	void							Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT				Render() override;


private :
	//OBB 및 AABB 선택으로해서 스폰 박스 구현할 예정
#ifdef _DEBUG
	CCollision*							m_pBoxCollision = nullptr;
#endif // _DEBUG
	_Int								m_iCellIndex = {};

	_wstring							m_szSpawnPrototypeName;
	_float								m_fAccSpawnTimeer = {};

	_float								m_fSpawnTime = {};
	_float								m_fMaxSpawnCount = {};
	_bool								m_bIsDestroy = false;

	_float								m_fSpawnBoxToPlayerDistance = {};
private :
#ifdef _DEBUG
	HRESULT								ADD_Components();
#endif // _DEBUG

	HRESULT								SpawnPrototpyeObject();

public :
	static		CSpawnBox*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*			Clone(void* pArg) override;
	virtual		void					Free() override;

};
NS_END
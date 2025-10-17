#pragma once
#include "Architecture.h"

NS_BEGIN(Client)
class CWoodDoor : public CArchitecture
{
public:
	typedef struct DefenceWallDesc
	{
		_bool					bIsFinished;

	}DEFENCE_WALL_DESC;

protected:
	CWoodDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWoodDoor(const CWoodDoor& rhs);
	virtual ~CWoodDoor() = default;

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

protected:
	virtual	void							Damage(void* pArg, CActor* pAttackActor);

private:
	HRESULT									ADD_Components();

public:
	static			CWoodDoor*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
#pragma once
#include "Architecture.h"

NS_BEGIN(Client)
class CWoodWall : public CArchitecture
{
public:
	typedef struct DefenceWallDesc
	{
		_bool					bIsFinished;

	}DEFENCE_WALL_DESC;

protected:
	CWoodWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWoodWall(const CWoodWall& rhs);
	virtual ~CWoodWall() = default;

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
	static			CWoodWall*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
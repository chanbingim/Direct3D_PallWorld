#pragma once
#include "Architecture.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CFarmGround : public CArchitecture
{
public:
	enum class FARM_TYPE { TREE, END };

	typedef struct FarmGroundDesc
	{
		const WCHAR*			szCropsName;
		FARM_TYPE				e_Type;
	}FARM_GROUND_DESC;

protected:
	CFarmGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFarmGround(const CFarmGround& rhs);
	virtual ~CFarmGround() = default;

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

protected :
	CCollision*								m_pActionCollision = nullptr;

	_wstring								m_CropsName = {};
	_float									m_fAccTime = {};
	_float									m_fRegenTime = {};

private:
	HRESULT									ADD_Components();

public:
	static			CFarmGround*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
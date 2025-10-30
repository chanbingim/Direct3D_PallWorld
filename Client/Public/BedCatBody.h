#pragma once
#include "PellBody.h"

NS_BEGIN(Client)
class CHitBox;

class CBedCatBody : public CPellBody
{
protected:
	CBedCatBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBedCatBody(const CBedCatBody& rhs);
	virtual ~CBedCatBody() = default;

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

	
private :
	CHitBox*								m_pSkillHitBox[2] = { nullptr, nullptr };

private :
	HRESULT									ADD_Components();
	HRESULT									Insert_KeyFrame();

	void									AttackOnCollisionR();
	void									UpateCollisionR();

	void									AttackOnCollisionL();
	void									UpateCollisionL();

public:
	static			CBedCatBody*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
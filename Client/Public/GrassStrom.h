#pragma once
#include "SkillObjectBase.h"

NS_BEGIN(Client)
class CGrassStrom : public CSkillObjectBase
{
private :
	CGrassStrom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrassStrom(const CGrassStrom& rhs);
	virtual ~CGrassStrom() = default;

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

private:
	HRESULT									ADD_Components();

public:
	static			CGrassStrom*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
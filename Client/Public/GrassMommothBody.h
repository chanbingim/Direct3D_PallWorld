#pragma once
#include "PellBody.h"

NS_BEGIN(Client)
class CGrassMommothBody : public CPellBody
{
protected:
	CGrassMommothBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGrassMommothBody(const CGrassMommothBody& rhs);
	virtual ~CGrassMommothBody() = default;

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
	void									SetSelectSkillIndex(_uInt iIndex) { m_iSkillIndex = iIndex; }
private :
	_float									m_ProjectileSocketDistance = {};
	_float									m_iSkillIndex = {};

private:
	HRESULT									ADD_Components();
	HRESULT									Insert_AnimKeyFrameFunction();

	void									EarthquakeEvent();
	HRESULT									FarSkillEvent();

public:
	static			CGrassMommothBody*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
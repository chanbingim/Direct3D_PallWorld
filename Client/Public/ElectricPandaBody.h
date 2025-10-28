#pragma once
#include "PellBody.h"

NS_BEGIN(Client)
class CElectricPandaBody : public CPellBody
{
protected:
	CElectricPandaBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CElectricPandaBody(const CElectricPandaBody& rhs);
	virtual ~CElectricPandaBody() = default;

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
	const _float4x4*						m_HandMatrix = nullptr;
	_Int									m_iSkillIndex = {};

private:
	HRESULT									ADD_Components();
	HRESULT									ShootProjecttileObject();


public:
	static			CElectricPandaBody*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
#pragma once
#include "ItemBase.h"

NS_BEGIN(Client)
class CCostumeBase : public CItemBase
{
public:
	typedef struct InitCostumeDesc : public InitItemDesc
	{
		COSTUME_TYPE						eCostumeType;

	}INIT_COSTUME_DESC;

protected:
	CCostumeBase(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CCostumeBase(const CCostumeBase& rhs);
	virtual ~CCostumeBase() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype(void* InitDesc) override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources() override;

protected :
	COSTUME_TYPE							m_CostumType = {};


private:
	HRESULT									ADD_Components();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CPlayerManager;

class CPlayerBody : public CPartObject
{
private :
	CPlayerBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerBody(const CPlayerBody& rhs);
	virtual ~CPlayerBody() = default;

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
	HRESULT									PreviewRender();

	virtual		HRESULT						ShadowRender() override;
	void									UpdateAnimation(CModel* pVIBuffer);

protected :
	HRESULT									Apply_ConstantShaderResources(_uInt iMeshIndex);

private :
	CPlayerManager*							m_pPlayerManager = nullptr;
	CModel*									m_pClothesBuffer = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static			CPlayerBody*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
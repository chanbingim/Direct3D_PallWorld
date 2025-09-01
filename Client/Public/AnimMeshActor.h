#pragma once
#include "Client_Define.h"
#include "Actor.h"

NS_BEGIN(Engine)
class CModel;
class CShader;
NS_END

NS_BEGIN(Client)
class CAnimMeshActor : public CActor
{
protected:
	CAnimMeshActor(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CAnimMeshActor(const CAnimMeshActor& rhs);
	virtual ~CAnimMeshActor() = default;

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
	void									SetAnimIndex(_uInt iIndex);

protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

protected:
	CModel*									m_pVIBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

	ID3DX11EffectShaderResourceVariable*	m_pSRVEffect = nullptr;
	ID3DX11EffectMatrixVariable*			m_pBoneMatrixEffect = nullptr;

	_uInt									m_iAnimIndex = {};

private:
	HRESULT									ADD_Components();

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
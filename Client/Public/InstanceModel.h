#pragma once

#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Model_Instance;
NS_END

NS_BEGIN(Client)
class CInstanceModel abstract : public CGameObject
{
protected:
	CInstanceModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInstanceModel(const CInstanceModel& rhs);
	virtual ~CInstanceModel() = default;

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

	void									RefreshComputeHeight();
protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex);

protected:
	CVIBuffer_Model_Instance*				m_pVIBufferCom = nullptr;

	CShader*								m_pShaderCom = nullptr;
	ID3DX11EffectShaderResourceVariable*	m_pSRVEffect = nullptr;

public:
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
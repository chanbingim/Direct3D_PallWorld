#pragma once
#include "PellBody.h"

NS_BEGIN(Engine)
class CTexture;
class CTrailComponent;
NS_END

NS_BEGIN(Client)
class CDororongBody : public CPellBody
{
protected:
	CDororongBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDororongBody(const CDororongBody& rhs);
	virtual ~CDororongBody() = default;

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
	const _float4x4*						m_pTrailSocket = nullptr;
	_float4									m_vTrailColor = { 0.7f, 0.7f, 0.f, 1.0f };

	CTrailComponent*						m_pTrail = nullptr;
	CTexture*								m_pTrailTex = nullptr;
	CShader*								m_pTrailShader = nullptr;

private:
	HRESULT									ADD_Components();
	void									Apply_TrailShaderResource();

public:
	static			CDororongBody*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
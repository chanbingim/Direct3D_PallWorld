#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Cube;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CCubeSky : public CGameObject
{
private:
	CCubeSky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCubeSky(const CCubeSky& Prototype);
	virtual ~CCubeSky() = default;

public:
	//초기화
	virtual		HRESULT				Initalize_Prototype();
	virtual		HRESULT				Initialize(void* pArg);

	//업데이트	
	virtual		void				Priority_Update(_float fDeletaTime);
	virtual		void				Update(_float fDeletaTime);
	virtual		void				Late_Update(_float fDeletaTime);

	// 랜더
	virtual		HRESULT				Render();

private:
	CVIBuffer_Cube*					m_pVIBufferCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CShader*						m_pShaderCom = { nullptr };

private:
	HRESULT							Ready_Components();
	HRESULT							Bind_ShaderResources();

public:
	static CCubeSky*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;

};
NS_END
#pragma once

#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CBackGround : public CUserInterface
{
protected:
	CBackGround(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected:
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

protected:
	virtual		HRESULT						Bind_ShaderResources() override;

private :
	HRESULT									ADD_Components();

public:
	static			CBackGround*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END
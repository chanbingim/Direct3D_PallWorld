#pragma once

#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CBackGround final  : public CUserInterface
{
private:
	CBackGround(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CBackGround(const CBackGround& rhs);
	virtual ~CBackGround() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	_float4x4								m_ProjMatrix;

	ID3DX11EffectVariable*					m_pWorldMat = nullptr;
	ID3DX11EffectVariable*					m_pViewMat = nullptr;
	ID3DX11EffectVariable*					m_pProjMat = nullptr;

private :
	HRESULT									ADD_Components();

public:
	static			CBackGround*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END
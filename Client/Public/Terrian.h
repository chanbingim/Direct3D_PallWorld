#pragma once

#include "Client_Define.h"
#include "Actor.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrian;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CTerrian : public CActor
{
public :
	typedef struct Terrian_Desc : GAMEOBJECT_DESC
	{
		_uInt			iGridCnt;
		_bool			IsPicking;

	}TERRIAN_DESC;

protected:
	CTerrian(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CTerrian(const CTerrian& rhs);
	virtual ~CTerrian() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render();

protected :
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources() override;

private :
	_bool									m_bIsPicking = false;

	CVIBuffer_Terrian*						m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

private :
	HRESULT									ADD_Components(_uInt iGridCnt = 0);

public:
	static			CTerrian*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
#pragma once

#include "Client_Define.h"
#include "Actor.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
class CNavigation;
class CTexture;
class CShader;
NS_END

NS_BEGIN(Client)
class CTerrain : public CActor
{
public :
	typedef struct Terrian_Desc : GAMEOBJECT_DESC
	{
		_uInt			TerrianType;
		_uInt			iGridCnt;
		_bool			IsPicking;

		WCHAR			HeightMap[MAX_PATH];
		WCHAR			NavigationData[MAX_PATH];
	}TERRIAN_DESC;

protected:
	CTerrain(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;

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

	_bool									IsMoveTerrian(_vector vPosition);
	_Int									FindCell(_vector vPosition);

protected :
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources() override;

private :
	_bool									m_bIsPicking = false;
	WCHAR									szVIBuffer[MAX_PATH] = {};

	CVIBuffer_Terrain*						m_pVIBufferCom = nullptr;
	CNavigation*							m_pNavigationCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

private :
	HRESULT									ADD_Components(const TERRIAN_DESC& Desc);

public:
	static			CTerrain*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
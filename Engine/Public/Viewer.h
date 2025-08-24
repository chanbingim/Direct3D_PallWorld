#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class CShader;
class CBaseCamera;
class CGameObject;
class CVIBuffer_Rect;

class CViewer : public CUserInterface
{
private :
	CViewer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CViewer(const CViewer& rhs);
	virtual ~CViewer() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetViewCameraPosition(_float3 Pos);
	void									SetViewCameraFov(_float fFov);

	void									SetViewObject(CGameObject* pGameObject);


protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources();

private :
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;
	ID3D11Texture2D*						m_pViewTexture = nullptr;

	// 카메라도 보관해두자
	CBaseCamera*							m_pViewerCamera = nullptr;

	// 게임 오브젝트를 
	CGameObject*							m_pViewObject = nullptr;

private :
	HRESULT									CreateViewTexture();
	HRESULT									CreateViewerCamera();

	void									RenderObejct();

public :
	static		CViewer*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
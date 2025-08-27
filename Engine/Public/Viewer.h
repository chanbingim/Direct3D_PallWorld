#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class CShader;
class CBaseCamera;
class CGameObject;
class CVIBuffer_Rect;

class ENGINE_DLL CViewer : public CUserInterface
{
public :
	typedef struct ViewerDesc : GAMEOBJECT_DESC
	{
		_float fWidth;
		_float fHeight;

	}VIEWER_DESC;

protected :
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

	//셰이더 텍스처 정보를 백버퍼나 내가 원하는거만 랜더링한후 갱신할 예정
	virtual		void						UpdateShaderResource(_uInt iRenderIndex);

	void									RenderObejct();

	virtual			void					MouseHovering();

	virtual			void					MouseButtonDwon();
	virtual			void					MouseButtonPressed();
	virtual			void					MouseButtonUp();

protected:
	CVIBuffer_Rect*							m_pVIBufferCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

	ID3D11RenderTargetView*					m_pRenderTargetTex = nullptr;
	ID3D11DepthStencilView*					m_pDepthStencil = nullptr;

	ID3D11ShaderResourceView*				m_pViewTexture = nullptr;


	// 카메라도 보관해두자
	CBaseCamera*							m_pViewerCamera = nullptr;

	// 게임 오브젝트를 
	CGameObject*							m_pViewObject = nullptr;

	_float									m_fCameraDistance;

private :
	HRESULT									CreateViewTexture();
	HRESULT									CreateViewerCamera(_float fWidth = 0, _float fHeight = 0);

private :
	LPD3D11EFFECTSHADERRESOURCEVARIABLE		m_pShader_Resource = nullptr;

public :
	static		CViewer*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
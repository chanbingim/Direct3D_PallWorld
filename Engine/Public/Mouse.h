#pragma once

#include "UserInterface.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;

class ENGINE_DLL CMouse final : public CUserInterface
{
private :
	CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CMouse() = default;

public :
	//초기화
	virtual		HRESULT			Initalize_Prototype(HWND hWnd);
	virtual		HRESULT			Initialize(void* pArg) override;
	//업데이트	
	virtual		void			Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT			Render() override;

	HRESULT						SetTexture(_uInt iLevelIndex, const WCHAR* Proto_TexTag, const WCHAR* ComTex_Tag, void* pTexArg,
										   const WCHAR* Proto_ShaderTag, const WCHAR* ComShader_Tag, void* pShaderArg,
										   const WCHAR* Proto_BufferTag, const WCHAR* ComBuffer_Tag, void* pBufferArg);

	POINT&						GetMousePoint();
	_float3&					GetRayPos(RAY eRayState);

	void						SetMouseFocus(CUserInterface* Widget);
	BOOL						IsFocus(CUserInterface* Widget);

	void						SetDrag(_bool flag);
	BOOL						IsDrag();

protected:
	HRESULT						Bind_ShaderCBuffer();

private :
	HWND						m_hWnd = nullptr;

	CUserInterface*				m_pDragSlot = nullptr;
	_bool						m_IsDrag = false;

	CTexture*					m_pTextureCom = nullptr;
	CShader*					m_pShaderCom = nullptr;
	CVIBuffer_Rect*				m_pVIBufferCom = nullptr;

	POINT						m_MouseViewPortPos = {};
	_float3						m_RayPos[ENUM_CLASS(RAY::END)] = {};

	_float4x4					m_InvViewMat = {};
	_float4x4					m_InvProjMat = {};

public :
	static			CMouse*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual			void		Free() override;
	
};
NS_END
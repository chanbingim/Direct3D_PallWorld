#include "Mouse.h"

#include "GameInstance.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CUserInterface(pDevice, pContext)
{
}

HRESULT CMouse::Initalize_Prototype(HWND hWnd)
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	//만약 마우스 텍스처 크기가 32 * 32 면 64 * 64로 잡아서 크기만든다음
	//마우스 shader를 통해서 UV 값을 조절해주자
	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	m_hWnd = hWnd;
	m_pTransformCom->SetScale({ 64.f,64.f, 0.f });

	XMStoreFloat4x4(&m_InvViewMat, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ViewMatrix)));
	XMStoreFloat4x4(&m_InvProjMat, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_ProjMatrix)));

	return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{

	return S_OK;
}

void CMouse::Update(_float fDeletaTime)
{
	// 마우스 이동은 월드
	// ptInRect로 얻어온 정보는 viewPort상의 정보이다.
	// 이거 World로 내리는 작업을 해줘야 할것 같다.
	// 그래야 텍스처로 그릴수 있을거같음

	D3D11_VIEWPORT       ViewportDesc{};
	_uInt                iNumViewports = { 1 };

	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	GetCursorPos(&m_MouseViewPortPos);
	ScreenToClient(m_hWnd, &m_MouseViewPortPos);

	_vector NDCSpacePoint = { m_MouseViewPortPos.x - ViewportDesc.Width * 0.5f, 
							  m_MouseViewPortPos.y - ViewportDesc.Height * 0.5f, 0.f };

	_vector ViewPoint = XMVector3TransformCoord(NDCSpacePoint, XMLoadFloat4x4(&m_InvProjMat));
	ViewPoint = XMVector3TransformCoord(ViewPoint, XMLoadFloat4x4(&m_InvViewMat));
	XMStoreFloat3(&m_RayPos[ENUM_CLASS(RAY::VIEW)], ViewPoint);

	//여기서 월드 곱해서 월드로 내린다음 transform을 세팅하고 사용이지않을까
	_vector WorldPoint = XMVector3TransformCoord(ViewPoint, XMLoadFloat4x4(&m_pTransformCom->GetInvWorldMat()));
	XMStoreFloat3(&m_RayPos[ENUM_CLASS(RAY::WORLD)], WorldPoint);

	m_pTransformCom->SetPosition(m_RayPos[ENUM_CLASS(RAY::WORLD)]);
}

HRESULT CMouse::Render()
{
	//셰이더를 세팅하고 그린다 라는 함수 호출 한다.
	//이건 마우스 포인터에 그림이 있을때 동작하게 만들자.
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	Apply_ConstantShaderResources();

	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

HRESULT CMouse::SetTexture(_uInt iLevelIndex, const WCHAR* Proto_TexTag, const WCHAR* ComTex_Tag, void* pTexArg,
						   const WCHAR* Proto_ShaderTag, const WCHAR* ComShader_Tag, void* pShaderArg,
						   const WCHAR* Proto_BufferTag, const WCHAR* ComBuffer_Tag, void* pBufferArg)
{
	if (FAILED(__super::Add_Component(iLevelIndex, Proto_TexTag, ComTex_Tag, (CComponent**)&m_pTextureCom, pTexArg)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(iLevelIndex, Proto_ShaderTag, ComShader_Tag, (CComponent**)&m_pShaderCom, pShaderArg)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(iLevelIndex, Proto_BufferTag, ComBuffer_Tag, (CComponent**)&m_pVIBufferCom, pBufferArg)))
		return E_FAIL;

	return S_OK;
}

_float3& CMouse::GetRayPos(RAY eRayState)
{
	return m_RayPos[ENUM_CLASS(eRayState)];
}

void CMouse::SetMouseFocus(CUserInterface* Widget)
{
	m_pDragSlot = Widget;
}

void CMouse::SetDrag(_bool flag)
{
	m_IsDrag = flag;
}

BOOL CMouse::IsDrag()
{
	return m_IsDrag;
}

BOOL CMouse::IsFocus(CUserInterface* Widget)
{
	if (nullptr == m_pDragSlot)
		return true;

	return m_pDragSlot == Widget ? true : false;
}

POINT& CMouse::GetMousePoint()
{
	return m_MouseViewPortPos;
}

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CMouse* pMouse = new CMouse(pDevice, pContext);
	if (FAILED(pMouse->Initalize_Prototype(hWnd)))
	{
		Safe_Release(pMouse);
		MSG_BOX("CREATE FAIL : MOUSE");
	}

	return pMouse;
}

void CMouse::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}

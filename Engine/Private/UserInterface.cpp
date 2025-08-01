#include "UserInterface.h"

#include "GameInstance.h"

CUserInterface::CUserInterface(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
	CGameObject(pGraphic_Device, pDeviceContext)
{
	_matrix mat = XMMatrixIdentity();
	XMStoreFloat4x4(&m_ViewMatrix, mat);
	XMStoreFloat4x4(&m_ProjMatrix, mat);
}

CUserInterface::CUserInterface(const CUserInterface& rhs) :
	CGameObject(rhs),
	m_ViewMatrix(rhs.m_ViewMatrix),
	m_ProjMatrix(rhs.m_ProjMatrix)
{
}

HRESULT CUserInterface::Initalize_Prototype()
{
	D3D11_VIEWPORT       ViewportDesc{};
	_uInt                iNumViewports = { 1 };

	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	return S_OK;
}

HRESULT CUserInterface::Initialize(void* pArg)
{
	if(FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CUserInterface::Update(_float fDeletaTime)
{
	UpdateRectSize();
	OverlapEvent();
}

HRESULT CUserInterface::Render()
{
	return S_OK;
}

_uInt CUserInterface::GetZOrder()
{
	return m_iZOrder;
}

void CUserInterface::OverlapEvent()
{
	//마우스 정보를 가져온다음 
	/*auto MousPoint = POINT();
	if (PtInRect(&m_UISize, MousPoint))
		m_bIsHover = true;
	else
		m_bIsHover = false;*/
}

void CUserInterface::UpdateRectSize()
{
	_vector ParentposVec{};
	_float3 vParentpos{};

	if (m_pParent)
	{
		auto Parent = m_pParent;
		while (Parent)
		{
			auto vPos = Parent->GetTransform()->GetPosition();
			ParentposVec += XMLoadFloat3(&vPos);

			Parent = Parent->GetParent();
		}
		XMStoreFloat3(&vParentpos, ParentposVec);
	}
	
	_float3 vScale = m_pTransformCom->GetScale();
	_float3 vPosition = m_pTransformCom->GetPosition();

	m_UISize = {static_cast<long>((vParentpos.x + vPosition.x) - vScale.x),
				static_cast<long>((vParentpos.y + vPosition.y) - vScale.y),
				static_cast<long>((vParentpos.x + vPosition.x) + vScale.x),
				static_cast<long>((vParentpos.y + vPosition.y) + vScale.y)};
}

CGameObject* CUserInterface::Clone(void* pArg)
{
	return nullptr;
}

void CUserInterface::Free()
{
	__super::Free();
}

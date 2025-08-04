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
	if(FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (pArg)
	{
		GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
		SetRotation(pObjectDesc->vRotation);
		SetScale(pObjectDesc->vScale);

		D3D11_VIEWPORT viewportDesc;
		_uInt          iNumViewports = { 1 };

		m_pDeviceContext->RSGetViewports(&iNumViewports, &viewportDesc);

		//이렇게 해야하는 이유 처음엔 몰랐는데
		//마우스 rect검사를 할때 좀더 편하게 하려면 해야함
		pObjectDesc->vPosition.x -= viewportDesc.Width * 0.5f;
		pObjectDesc->vPosition.y = -pObjectDesc->vPosition.y + viewportDesc.Height * 0.5f;
		
		SetLocation(pObjectDesc->vPosition);
		if (pObjectDesc->pParent)
		{
			SetParent(pObjectDesc->pParent);
			UpdateRectSize();
		}
	}

	return S_OK;
}

void CUserInterface::Update(_float fDeletaTime)
{
	if(OBJECT_TYPE::DYNAMIC == m_eType)
		UpdateRectSize();

	if(m_bIsMouseEvent)
		OverlapEvent();
}

void CUserInterface::Late_Update(_float fDeletaTime)
{
}

HRESULT CUserInterface::Render()
{
	return S_OK;
}

_uInt CUserInterface::GetZOrder()
{
	return m_iZOrder;
}

void CUserInterface::SetLocation(_float3 vPosition)
{
	__super::SetLocation(vPosition);
	UpdateRectSize();
}

void CUserInterface::SetRotation(_float3 vRotation)
{
	__super::SetRotation(vRotation);
	UpdateRectSize();
}

void CUserInterface::SetScale(_float3 vScale)
{
	__super::SetScale(vScale);
	UpdateRectSize();
}

void CUserInterface::ADDPosition(_vector vAddPos)
{
	__super::ADDPosition(vAddPos);
	UpdateRectSize();
}

void CUserInterface::ADDRotation(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta)
{
	__super::ADDRotation(vAxis, fTurnSpeed, fTimeDeleta);
	UpdateRectSize();
}

void CUserInterface::MouseHoverEnter()
{

}

void CUserInterface::MouseHovering()
{

}

void CUserInterface::MouseHoverExit()
{

}

void CUserInterface::MouseButtonDwon()
{

}

void CUserInterface::MouseButtonPressed()
{

}

void CUserInterface::MouseButtonUp()
{

}

HRESULT CUserInterface::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_ViewMatrix));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_ProjMatrix));

	return S_OK;
}

void CUserInterface::OverlapEvent()
{
	if (!m_pGameInstance->IsMouseFocus(this))
		return;
	
	//그다음 얻은 포커스 및 드래그 플래그를 통해서
	//Pressed 및 Up 반환
	if (m_pGameInstance->IsMouseDrag())
	{
		if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0) || m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 1))
		{
			
			MouseButtonPressed();
		}

		if (m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 0) || m_pGameInstance->KeyUp(KEY_INPUT::MOUSE, 1))
		{
			MouseButtonUp();
			m_pGameInstance->SetDrag(false);
		}
	}
	else
	{
		if (PtInRect(&m_UISize, m_pGameInstance->GetMousePoint()))
		{
			if (!m_bIsHover)
			{
				MouseHoverEnter();
				m_pGameInstance->SetMouseFocus(this);
			}

			m_bIsHover = true;
			MouseHovering();

			if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0) || m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1))
			{
				MouseButtonDwon();

				//여기서 Drag 밑 포커스 얻기
				m_pGameInstance->SetDrag(true);
			}
		}
		else
		{
			if (!m_pGameInstance->IsMouseDrag())
			{
				if (m_bIsHover)
				{
					MouseHoverExit();
					m_pGameInstance->SetMouseFocus(nullptr);
				}
			}
			m_bIsHover = false;
		}
	}
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

	D3D11_VIEWPORT       ViewportDesc{};
	_uInt                iNumViewports = { 1 };

	m_pDeviceContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float		RectCenterX = (vParentpos.x + vPosition.x) + ViewportDesc.Width * 0.5f;
	_float		RectCenterY = -(vParentpos.y + vPosition.y) + ViewportDesc.Height * 0.5f;

	m_UISize = {static_cast<long>(RectCenterX - (vScale.x * 0.5f)),
				static_cast<long>(RectCenterY - (vScale.y * 0.5f)),
				static_cast<long>(RectCenterX + (vScale.x * 0.5f)),
				static_cast<long>(RectCenterY + (vScale.y * 0.5f))};
}

CGameObject* CUserInterface::Clone(void* pArg)
{
	return nullptr;
}

void CUserInterface::Free()
{
	__super::Free();
}

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
	_float2 WinSize = m_pGameInstance->GetScreenSize();
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(WinSize.x, WinSize.y, 0.f, 1.f));

	return S_OK;
}

HRESULT CUserInterface::Initialize(void* pArg)
{
	if(!m_bIsWorld)
	{
		if (FAILED(__super::Initialize(nullptr)))
			return E_FAIL;

		if (pArg)
		{
			GAMEOBJECT_DESC* pObjectDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
			SetRotation(pObjectDesc->vRotation);
			SetScale(pObjectDesc->vScale);

			m_fPos.x = pObjectDesc->vPosition.x;
			m_fPos.y = pObjectDesc->vPosition.y;

			if (pObjectDesc->pParent)
				SetParent(pObjectDesc->pParent);

			UpdateRectSize();
		}
	}
	else
	{
		if (FAILED(__super::Initialize(pArg)))
			return E_FAIL;
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
	memcpy(&m_fPos, &vPosition, sizeof(_float2));
	UpdateRectSize();
}

void CUserInterface::SetRotation(_float3 vRotation)
{
	m_pTransformCom->SetRotation(vRotation);
	UpdateRectSize();
}

void CUserInterface::SetScale(_float3 vScale)
{
	m_pTransformCom->SetScale(vScale);
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

_float2 CUserInterface::GetViewPos()
{
	return m_fPos;
}

const RECT& CUserInterface::GetRectSize()
{
	return m_UISize;
}

_float2 CUserInterface::GetScreenPos()
{
	_float2 vScreenPos = { (_float)(m_UISize.left + (m_UISize.right - m_UISize.left) * 0.5f),
						   (_float)(m_UISize.top + (m_UISize.bottom - m_UISize.top) * 0.5f)};

	return vScreenPos;
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

HRESULT CUserInterface::Bind_ShaderResources()
{
	return S_OK;
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
			if (m_bIsHover)
			{
				MouseButtonUp();
				m_pGameInstance->SetDrag(false);
			}
		}
	}

	if (PtInRect(&m_UISize, m_pGameInstance->GetMousePoint()))
	{
		if (!m_bIsHover)
		{
			MouseHoverEnter();
		}

		m_bIsHover = true;
		MouseHovering();

		if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0) || m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1))
		{
			//여기서 Drag 밑 포커스 얻기
			m_pGameInstance->SetDrag(true);
			MouseButtonDwon();
		}
	}
	else
	{
		if (m_bIsHover)
		{
			MouseHoverExit();
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
			_float3 vPos = {};

			auto UIObject = dynamic_cast<CUserInterface*>(Parent);
			if (UIObject)
			{
				_float2 ParentPos = UIObject->GetViewPos();
				vPos = { ParentPos.x, ParentPos.y, 1.f };
			}
			else
				vPos = Parent->GetTransform()->GetPosition();

			ParentposVec += XMLoadFloat3(&vPos);
			Parent = Parent->GetParent();
		}
		XMStoreFloat3(&vParentpos, ParentposVec);
	}

	_float3 vScale = m_pTransformCom->GetScale();
	auto ScreenSize = m_pGameInstance->GetScreenSize();

	_float		RectCenterX = (vParentpos.x + m_fPos.x);
	_float		RectCenterY = (vParentpos.y + m_fPos.y);

	m_pTransformCom->SetPosition({ RectCenterX - ScreenSize.x * 0.5f,
								  -RectCenterY + ScreenSize.y * 0.5f, 0.f});

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

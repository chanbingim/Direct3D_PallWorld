#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBaseCamera(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera& rhs) :
	CBaseCamera(rhs)
{
}

HRESULT CCamera::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_ObejctTag = TEXT("Main Camera");
	return S_OK;
}

void CCamera::Priority_Update(_float fDeletaTime)
{
	if (GAMEMODE::GAME == m_pGameInstance->GetGameMode())
		Input_KeyBoard(fDeletaTime);

	__super::Priority_Update(fDeletaTime);
}

void CCamera::Update(_float fDeletaTime)
{
}

void CCamera::Late_Update(_float fDeletaTime)
{
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Input_KeyBoard(_float fDeletaTime)
{
	//키 코드를 이용해서 움직일지 플레이어를 따라다닐지는 결정해봐야 알것같은데
	//플레이어 따라다닐거같음 모델띄우면 그때 장착시키고 그전까지는
	//Key 입력으로 움직이는걸로하자
	//이건 쌤 수업코드 나가는거보고 좋은부분은 가져와야겠음
	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_UPARROW))
	{
		//앞방향 이동
		auto vLook = XMVector3Normalize(m_pTransformCom->GetLookVector());
		ADDPosition(vLook * m_fSpeed * fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_DOWNARROW))
	{
		//뒷방향 이동
		auto vLook = XMVector3Normalize(m_pTransformCom->GetLookVector());
		ADDPosition(vLook * -m_fSpeed * fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_RIGHTARROW))
	{
		//앞방향 이동
		auto vRight = XMVector3Normalize(m_pTransformCom->GetRightVector());
		ADDPosition(vRight * m_fSpeed * fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LEFTARROW))
	{
		//뒷방향 이동
		auto vRight = XMVector3Normalize(m_pTransformCom->GetRightVector());
		ADDPosition(vRight * -m_fSpeed * fDeletaTime);
	}

	_long		MouseMove = {};
	if (MouseMove = m_pGameInstance->GetMouseAxis(0))
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f),  MouseMove * m_fMouseSensor, fDeletaTime);
	}

	if (MouseMove = m_pGameInstance->GetMouseAxis(1))
	{
		m_pTransformCom->Turn(m_pTransformCom->GetRightVector(), MouseMove * m_fMouseSensor, fDeletaTime);
	}
}

CCamera* CCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCamera* pCamera = new CCamera(pDevice, pContext);
	if (FAILED(pCamera->Initalize_Prototype()))
	{
		Safe_Release(pCamera);
		MSG_BOX("CREATE FAIL : CAMERA");
	}

	return pCamera;
}

CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pCamera = new CCamera(*this);
	if (FAILED(pCamera->Initialize(pArg)))
	{
		Safe_Release(pCamera);
		MSG_BOX("CLONE FAIL : CAMERA");
	}

	return pCamera;
}

void CCamera::Free()
{
	__super::Free();
}

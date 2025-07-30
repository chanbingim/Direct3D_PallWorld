#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CCamera(pDevice, pContext)
{
}

CCamera::CCamera(const CCamera& rhs) :
	CCamera(rhs)
{
}

HRESULT CCamera::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_CameraInfo = *static_cast<CAMERA_DESC*>(pArg);

	m_ViewMatrix = m_pTransformCom->GetInvWorldMat();
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(m_CameraInfo.ffov, m_CameraInfo.fWidth / m_CameraInfo.fHegiht, m_CameraInfo.fNear, m_CameraInfo.fFar));

	return S_OK;
}

void CCamera::Update(_float fDeletaTime)
{
	Input_KeyBoard(fDeletaTime);
	m_ViewMatrix = m_pTransformCom->GetInvWorldMat();

	// 혹시나 종횡비 또는 far, Near값이 바뀌거나 Fov가 바뀔수있기때문에
	// 매프레임 갱신 한다.
	XMStoreFloat4x4(&m_ProjectionMatrix, XMMatrixPerspectiveFovLH(m_CameraInfo.ffov, m_CameraInfo.fWidth / m_CameraInfo.fHegiht, m_CameraInfo.fNear, m_CameraInfo.fFar));
}

// 뷰메트릭스의 역행렬을 월드이기때문에 월드 반환을 시킴
// 이는 추후 빌보드 행렬로 이용됨
const _float4x4& CCamera::GetWorldMatrix()
{
	return m_pTransformCom->GetWorldMat();
}

const _float4x4& CCamera::GetViewMatrix()
{
	return m_ViewMatrix;
}

const _float4x4& CCamera::GetProjectionMatrix()
{
	return m_ProjectionMatrix;
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
		auto vRight = XMVector3Normalize(m_pTransformCom->GetLookVector());
		ADDPosition(vRight * m_fSpeed * fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LEFTARROW))
	{
		//뒷방향 이동
		auto vRight = XMVector3Normalize(m_pTransformCom->GetLookVector());
		ADDPosition(vRight * -m_fSpeed * fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LEFTARROW))
	{
		// 시계방향 회전
		ADDRotation(m_pTransformCom->GetUpVector(), m_fSpeed, fDeletaTime);
	}

	if (m_pGameInstance->KeyPressed(KEY_INPUT::KEYBOARD, DIK_LEFTARROW))
	{
		//반시계 방향 회전
		ADDRotation(m_pTransformCom->GetUpVector(), -m_fSpeed, fDeletaTime);
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

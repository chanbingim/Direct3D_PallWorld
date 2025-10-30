#include "PalSpher.h"

#include "GameInstance.h"
#include "PlayerManager.h"

#include "PellBase.h"
#include "PalSpherUI.h"

CPalSpher::CPalSpher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CProjectileObject(pDevice, pContext)
{
}

CPalSpher::CPalSpher(const CPalSpher& rhs) :
	CProjectileObject(rhs)
{
}

HRESULT CPalSpher::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CPalSpher::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

    if (m_ObejctTag.c_str() == L"")
        m_ObejctTag = TEXT("Pal Spher");

    return S_OK;
}

void CPalSpher::Priority_Update(_float fDeletaTime)
{
	if (m_pHitPell)
	{
		m_fAccTime += fDeletaTime;

		if (m_fAccTime >= 1.f)
		{
			// 여기서  포획 시작
			m_pTransformCom->SetPosition(m_ReflectPoint);
		}
		else
		{
			_float3 vPos = {};
			XMStoreFloat3(&vPos, XMVectorLerp(XMLoadFloat3(&m_HitStartPoint), XMLoadFloat3(&m_ReflectPoint), m_fAccTime));
			m_pTransformCom->SetPosition(vPos);
		}
	}
	else
	{
		m_pTransformCom->ADD_Position(XMLoadFloat3(&m_fThorwDireaction) * m_fThorowSpeed * fDeletaTime);
		m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
	}
	
}

void CPalSpher::Update(_float fDeletaTime)
{
	if (m_pHitPell)
	{
		m_fAccTime += fDeletaTime;

		if (m_fAccTime >= 1.f)
		{
			if (m_bIsPlayAnim)
			{
				m_pVIBufferCom->PlayAnimation(0, 0, 0);
				m_pTransformCom->Turn(m_pTransformCom->GetUpVector(), XMConvertToRadians(-180.f), fDeletaTime);
				ComputeCatchPellSuccess(fDeletaTime);
				m_pPalSpherUI->SetPalSpherUI(m_fAccPercent);
				m_pPalSpherUI->Update(fDeletaTime);
			}
			else
			{
				_bool bIsFinished = m_pVIBufferCom->PlayAnimation(0, 0, fDeletaTime, 10.f, m_bIsPlayAnim);
				if (bIsFinished)
					m_bIsPlayAnim = true;
			}

			m_pTransformCom->SetPosition(m_ReflectPoint);
		}
		else
		{
			_float3 vPos = {};
			XMStoreFloat3(&vPos, XMVectorLerp(XMLoadFloat3(&m_HitStartPoint), XMLoadFloat3(&m_ReflectPoint), m_fAccTime));
			m_pTransformCom->SetPosition(vPos);
		}
	}
	else
	{
		m_pVIBufferCom->PlayAnimation(0, 0, 0);
		m_pTransformCom->ADD_Position(XMLoadFloat3(&m_fThorwDireaction) * m_fThorowSpeed * fDeletaTime);
	}

	m_CombinedWorldMatrix = m_pTransformCom->GetWorldMat();
}

void CPalSpher::Late_Update(_float fDeletaTime)
{
	if (m_bIsPlayAnim)
		m_pPalSpherUI->Late_Update(fDeletaTime);

	if (!m_pHitPell)
	{
		_float3 vPalSpherPos = m_pTransformCom->GetPosition();
		auto fDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&vPalSpherPos) - m_pGameInstance->GetCameraState(WORLDSTATE::POSITION)));
		if (fDistance >= m_pGameInstance->GetCameraINFO().y)
			m_IsDead = true;

		m_pGameInstance->ADD_CollisionList(m_pCollision);
	}

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPalSpher::Render()
{
	_uInt iNumMeshes = m_pVIBufferCom->GetNumMeshes();

	for (_uInt i = 0; i < iNumMeshes; ++i)
	{
		Apply_ConstantShaderResources(i);

		m_pShaderCom->Update_Shader(0);

		m_pVIBufferCom->Render(i);
	}

	return S_OK;
}

HRESULT CPalSpher::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_PalSpher"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_AnimMesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	CPalSpherUI::GAMEOBJECT_DESC PalSpherDesc = {};
	PalSpherDesc.pParent = this;
	PalSpherDesc.vScale = { 0.3f, 0.3f, 0.3f };
	m_pPalSpherUI = static_cast<CPalSpherUI *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_PalSpherInfo_UI"), &PalSpherDesc));

	CSphereCollision::SPEHRE_COLLISION_DESC SphereDesc = {};
	ZeroMemory(&SphereDesc, sizeof(CSphereCollision::SPEHRE_COLLISION_DESC));
	SphereDesc.pOwner = this;
	SphereDesc.Radius = 0.2f;
	SphereDesc.vCneter = _float3(0.f, SphereDesc.Radius, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionSphere"), TEXT("Collison_Com"), (CComponent**)&m_pCollision, &SphereDesc)))
		return E_FAIL;
	m_pCollision->BindBeginOverlapEvent([this](_float3 vDir, CGameObject* pTarget) { BeginOverlapEvent(vDir, pTarget); });

	return S_OK;
}

void CPalSpher::BeginOverlapEvent(_float3 vDir, CGameObject* pTarget)
{
	CPellBase* pHitPell = dynamic_cast<CPellBase*>(pTarget);
	if (pHitPell)
	{
		if (ACTOR_TEAM::NEUTRAL != pHitPell->GetPellTeam())
			return;

		//여기서 펠 플래그나 상태 바꿔주고 모든로직을 잠깐 멈춰둠
		//pHitPell->
		auto pModel = static_cast<CModel *>(pTarget->Find_Component(TEXT("VIBuffer_Com")));

		CCollision::DEFAULT_HIT_DESC DefaultHitDesc = {};
		CCollision* pCollision = static_cast<CCollision*>(pTarget->Find_Component(TEXT("Collision_Com")));

		if(pCollision)
			m_pCollision->RayIntersect(m_pCollision->GetCollisionType(), pCollision, DefaultHitDesc);

		_float3 vOwnerPosition = m_pTransformCom->GetPosition();
		_float3 vTargetPosition = pTarget->GetTransform()->GetPosition();

		_vector vNormal = XMLoadFloat3(&DefaultHitDesc.vNormal);
		_vector vDirecation = XMLoadFloat3(&vTargetPosition) - XMLoadFloat3(&vOwnerPosition);

		/* 반사 백터 공식 */
		/* P = 입사각 백터, N = 법선 백터, R = 반사 백터*/
		/* R = P + 2N(-P * N)*/
		_vector ProjVector = vNormal * XMVector3Dot(vDirecation * -1.f, vNormal);
		_vector vReflectVector = vDirecation + ProjVector * 2.f;
		_float RandomOffset = m_pGameInstance->Random(0.4f, 1.f);

		XMStoreFloat3(&m_ReflectDir, vReflectVector);
		m_HitStartPoint = DefaultHitDesc.vHitPoint;

		XMStoreFloat3(&m_ReflectPoint, XMLoadFloat3(&DefaultHitDesc.vHitPoint) + vReflectVector * RandomOffset);
		m_fAccTime = 0.f;
		m_bIsPlayAnim = false;

		m_pHitPell = pHitPell;
		m_pHitPell->Damage(nullptr, this);
	}
}

void CPalSpher::ComputeCatchPellSuccess(_float fDeletaTime)
{
	ViewPellCatchUI();
	m_fCurComputeTime += fDeletaTime;

	//플레이어와 펠의 레벨차이를 통한 일정 시간 및 확률을 통해 캐치
	auto PlayerData = CPlayerManager::GetInstance()->GetPlayerData();
	auto PellData = m_pHitPell->GetPellInfo();

	if (0.5f <= m_fCurComputeTime)
	{
		if (PlayerData.iLevel >= PellData.iLevel)
		{
			//낮으면 잡게 하자
			m_fAccPercent += m_pGameInstance->Random(20.f, 25.f);
		}
		else
		{
			//높으면 못잡고
			_uInt iPellOver = PellData.iLevel - PlayerData.iLevel;
			_float fRandomValue = m_pGameInstance->Random(20.f, 25.f);
			_float fApplyPercent = fRandomValue - iPellOver;

			fApplyPercent = Clamp<_float>(fApplyPercent, 0.f, 25.f);
			m_fAccPercent += fApplyPercent;
		}

		m_iAccCount++;
		m_fCurComputeTime = 0.f;
	}

	// 누적 연산이 끝나면 잡았는지 안잡았는지 확인
	if (4 <= m_iAccCount)
	{
		if (80 <= m_fAccPercent)
		{
			// 성공
			// 따로 플레그를 주지않은이상 움직이지 않는걸로
			m_pGameInstance->Manager_PlaySound(TEXT("06-caught-a-pokemon.mp3"), CHANNELID::EFFECT, 0.5f);
			CPlayerManager::GetInstance()->ADDOwnerPellList(m_pHitPell);

		}

		m_pHitPell->Damage(nullptr, this);
		m_pHitPell = nullptr;
		m_IsDead = true;
	}
}

void CPalSpher::ViewPellCatchUI()
{
	//여기서 펠을 잡았는지 안잡았는지 UI로 표현할 예정
}

CPalSpher* CPalSpher::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPalSpher* pPalSpher = new CPalSpher(pDevice, pContext);
	if (FAILED(pPalSpher->Initalize_Prototype()))
	{
		Safe_Release(pPalSpher);
		MSG_BOX("CREATE FAIL : Pal Spher");
	}
	return pPalSpher;
}

CGameObject* CPalSpher::Clone(void* pArg)
{
	CPalSpher* pPalSpher = new CPalSpher(*this);
	if (FAILED(pPalSpher->Initialize(pArg)))
	{
		Safe_Release(pPalSpher);
		MSG_BOX("CLONE FAIL : Pal Spher");
	}
	return pPalSpher;
}

void CPalSpher::Free()
{
	__super::Free();

	Safe_Release(m_pPalSpherUI);
}

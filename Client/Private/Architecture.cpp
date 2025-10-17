#include "Architecture.h"

#include "GameInstance.h"
#include "ItemManager.h"

#include "PlayerManager.h"
#include "PellBoxManager.h"
#include "ActionAbleUI.h"

CArchitecture::CArchitecture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CWorkAbleObject(pDevice, pContext)
{
}

CArchitecture::CArchitecture(const CArchitecture& rhs) :
	CWorkAbleObject(rhs),
	m_ItemID(rhs.m_ItemID),
	m_pArchitectureInfo(rhs.m_pArchitectureInfo)
{
}

HRESULT CArchitecture::Initalize_Prototype()
{
	auto ItemDesc = CItemManager::GetInstance()->GetItemInfo(m_ItemID);
	if(ItemDesc)
		m_pArchitectureInfo = *ItemDesc;
	return S_OK;
}

HRESULT CArchitecture::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CActionAbleUI::ACTION_ABLE_DESC ActionDesc = {};
	ActionDesc.pOwner = this;
	ActionDesc.vScale = {50.f, 20.f, 0.f};
	ActionDesc.vAwayPosition = {10.f, -70.f, 0.f};

	m_pActionUI = CActionAbleUI::Create(m_pGraphic_Device, m_pDeviceContext);
	if(FAILED(m_pActionUI->Initialize(&ActionDesc)))
		return E_FAIL;

	m_fActionDistance = 10.f;
	m_fCurHealth = m_pArchitectureInfo.TypeDesc.ArchitectureDesc.fMaxHealth;
	CPellBoxManager::GetInstance()->Add_JobListObject(m_eWorkType, this);
	return S_OK;
}

void CArchitecture::Priority_Update(_float fDeletaTime)
{
}

void CArchitecture::Update(_float fDeletaTime)
{
	if (!m_bIsCompleted )
	{
		m_fAccTime += fDeletaTime * m_fWorkSpeed;
		if (1.f <= m_fAccTime)
		{
			m_fAccComplteTime++;
			m_fAccTime = 0;
		}

		if (m_fAccComplteTime  >= m_pArchitectureInfo.TypeDesc.ArchitectureDesc.fCompleteTime )
		{
			m_bIsCompleted = true;
			CPellBoxManager::GetInstance()->Remove_JobListObject(m_eWorkType, this);
		}
		else
		{
			if (m_pHitBoxCollision)
			{
				auto pObbCollision = static_cast<COBBCollision*>(m_pHitBoxCollision);
				auto BoundBox = pObbCollision->GetBounding();
				m_fMaxHeight = BoundBox.Extents.y;
				m_fMaxHeight = m_fMaxHeight * (m_fAccComplteTime / m_pArchitectureInfo.TypeDesc.ArchitectureDesc.fCompleteTime * 30.f);
			}
		}
	}
}

void CArchitecture::Late_Update(_float fDeletaTime)
{
}

HRESULT CArchitecture::Render()
{
	return S_OK;
}

void CArchitecture::PlusWorkSpeed(_float fWorkSpeed)
{
	m_fWorkSpeed += fWorkSpeed;
}

void CArchitecture::SubWorkSpeed(_float fWorkSpeed)
{
	m_fWorkSpeed -= fWorkSpeed;
	if (0 > m_fWorkSpeed)
		m_fWorkSpeed = 0.f;
}

HRESULT CArchitecture::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

	ID3D11ShaderResourceView* pResourceVeiw = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
	if (pResourceVeiw)
		m_pSRVEffect->SetResource(pResourceVeiw);

	if (false == m_bIsCompleted)
	{
		m_pShaderCom->Bind_RawValue("g_vColor", &m_vPercentColor, sizeof(_float4));
		m_pShaderCom->Bind_RawValue("g_fMaxHeight", &m_fMaxHeight, sizeof(_float));
	}

	return S_OK;
}

void CArchitecture::HitOverlapFunction(_float3 vDir, CGameObject* pHitActor)
{
	_float3 vHitActorPos = pHitActor->GetTransform()->GetPosition();
	_vector vCalHitActorPos = XMLoadFloat3(&vHitActorPos);

	_float3 vColisionExtents = {};
	if (m_pHitBoxCollision)
	{
		auto pOBBColision = static_cast<COBBCollision*>(m_pHitBoxCollision);
		_float3 vBoundCenter = pOBBColision->GetBounding().Center;
		_vector vCalBoundCenter = XMLoadFloat3(&vBoundCenter);
		_vector vDir = XMVector3Normalize(vCalBoundCenter - vCalHitActorPos);

		_float vDist = {};
		_float fLength = XMVectorGetX(XMVector3Length(vCalBoundCenter - vCalHitActorPos));

		if (0 < fLength)
		{
			pOBBColision->GetBounding().Intersects(vCalHitActorPos, vDir, vDist);
			if (0 > vDist)
				pHitActor->ADDPosition(vDir * vDist);
		}
	}
}

void CArchitecture::Damage(void* pArg, CActor* pDamagedActor)
{
	if (false == m_pArchitectureInfo.TypeDesc.ArchitectureDesc.bIsDaamge)
		return;

	DEFAULT_DAMAGE_DESC* pDamageDesc = static_cast<DEFAULT_DAMAGE_DESC*>(pArg);
	if (pDamageDesc)
	{
		m_fCurHealth -= pDamageDesc->fDmaged;
		if (0 >= m_fCurHealth)
			DeadFunction();
		else
		{
			//여기서 애니메이션 처리
			//있다면

		}
	}
}

HRESULT CArchitecture::DeadFunction()
{
	if (PELL_WORK_TYPE::END != m_eWorkType)
		CPellBoxManager::GetInstance()->Remove_JobListObject(m_eWorkType, this);

	m_IsDead = true;
	return S_OK;
}

void CArchitecture::UpdateActionUI(_float fDeletaTime)
{
	_vector vCalCamereaPos = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION);
	_vector vCalCamereaLook = m_pGameInstance->GetCameraState(WORLDSTATE::LOOK);

	CCollision::DEFAULT_HIT_DESC	RayHitDesc = {};
	if (m_pHitBoxCollision->RayHit(vCalCamereaPos, vCalCamereaLook, RayHitDesc))
	{
		if (RayHitDesc.vfDistance < m_fActionDistance)
		{
			CPlayerManager::GetInstance()->SetNearArchitecture(this);
			m_pActionUI->Update(fDeletaTime);
		}
	}
}

CGameObject* CArchitecture::Clone(void* pArg)
{
	return nullptr;
}

void CArchitecture::Free()
{
	__super::Free();

	Safe_Release(m_pHitBoxCollision);
	Safe_Release(m_pActionUI);
}

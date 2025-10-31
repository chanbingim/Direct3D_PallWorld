#include "HitBox.h"

#include "GameInstance.h"

CHitBox::CHitBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CPartObject(pDevice, pContext)
{
}

CHitBox::CHitBox(const CHitBox& rhs) :
	CPartObject(rhs)
{
}

HRESULT CHitBox::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CHitBox::Initialize(void* pArg)
{
	HIT_BOX_DESC* pDesc = static_cast<HIT_BOX_DESC*>(pArg);
	m_pOwner = pDesc->pOwner;
	m_SocketMatrix = pDesc->SocketMatrix;
	m_pParentWorldMat = pDesc->pOwnerWorldMat;

	if (FAILED(__super::Initialize(nullptr)))
		return E_FAIL;

	if (FAILED(ADD_Components(pDesc->vScale)))
		return E_FAIL;

	m_pTransformCom->SetScale({ 1.f,1.f,1.f });
	return S_OK;
}

void CHitBox::Priority_Update(_float fDeletaTime)
{
}

void CHitBox::Update(_float fDeletaTime)
{
}

void CHitBox::Late_Update(_float fDeletaTime)
{
	_matrix ScoketMatrix = XMLoadFloat4x4(m_SocketMatrix);
	for (_uInt i = 0; i < 3; ++i)
		ScoketMatrix.r[i] = XMVector4Normalize(ScoketMatrix.r[i]);

	XMStoreFloat4x4(&m_CombinedWorldMatrix,
		XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * ScoketMatrix * XMLoadFloat4x4(m_pParentWorldMat));

	m_pCollision->UpdateColiision(XMLoadFloat4x4(&m_CombinedWorldMatrix));
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CHitBox::Render()
{
	//m_pCollision->Render();
    return S_OK;
}

void CHitBox::StartAttack(_float fDamage)
{
	m_fHitBoxDamage = fDamage;
	m_pCollision->ResetCollision();
	m_pGameInstance->ADD_CollisionList(m_pCollision);
}

void CHitBox::UpdateAttack(_float fDamage)
{
	m_fHitBoxDamage = fDamage;
	m_pGameInstance->ADD_CollisionList(m_pCollision);
}

void CHitBox::HitBegin(_float3 vDir, CGameObject* pHitActor)
{
	if (pHitActor == m_pOwner)
		return;

	DEFAULT_DAMAGE_DESC DamageDesc = {};
	DamageDesc.fDmaged = m_fHitBoxDamage;

	auto Hit = static_cast<CActor*>(pHitActor);
	if (Hit)
	{
		Hit->Damage(&DamageDesc, this);
	}
}

HRESULT CHitBox::ADD_Components(_float3 vScale)
{
	COBBCollision::OBB_COLLISION_DESC OBBColDesc = {};
	OBBColDesc.pOwner = this;
	OBBColDesc.vExtents = vScale;
	OBBColDesc.vCneter = {};

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ColisionOBB"), TEXT("Collision_Com"), (CComponent**)&m_pCollision, &OBBColDesc)))
		return E_FAIL;

	m_pCollision->BindBeginOverlapEvent([&](_float3 vDir, CGameObject* pHitActor) { HitBegin(vDir, pHitActor); });

	return S_OK;
}

CHitBox* CHitBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHitBox* pHitBox = new CHitBox(pDevice, pContext);
	if (FAILED(pHitBox->Initalize_Prototype()))
	{
		Safe_Release(pHitBox);
		MSG_BOX("CREATE FAIL : HIT BOX");
	}
    return pHitBox;
}

CGameObject* CHitBox::Clone(void* pArg)
{
	CHitBox* pHitBox = new CHitBox(*this);
	if (FAILED(pHitBox->Initialize(pArg)))
	{
		Safe_Release(pHitBox);
		MSG_BOX("CLONE FAIL : HIT BOX");
	}
	return pHitBox;
}

void CHitBox::Free()
{
	__super::Free();
	Safe_Release(m_pCollision);
}

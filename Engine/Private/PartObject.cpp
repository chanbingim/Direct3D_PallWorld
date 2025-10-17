#include "PartObject.h"

#include "GameInstance.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)

{

}

CPartObject::CPartObject(const CPartObject& rhs) :
	CGameObject(rhs)
{
}

HRESULT CPartObject::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr == pArg)
	{
		
	}
	else
	{
		PARTOBJECT_DESC* Desc = static_cast<PARTOBJECT_DESC*>(pArg);
		m_SocketMatrix = Desc->SocketMatrix;
		m_UseSocketMatrixFlag = Desc->UseSocketMatrixFlag;
	}

	return S_OK;
}

void CPartObject::Priority_Update(_float fDeletaTime)
{
}

void CPartObject::Update(_float fDeletaTime)
{
	
}

void CPartObject::Late_Update(_float fDeletaTime)
{

}

HRESULT CPartObject::Render()
{
	return S_OK;
}

void CPartObject::SetAnimIndex(_uInt iIndex, _bool bIsLoop)
{
	if (-1 == iIndex)
		return;

	m_iAnimIndex = iIndex;
	m_bIsAnimLoop = bIsLoop;
}

_uInt CPartObject::GetAnimIndex(const char* szName)
{
	return	m_pVIBufferCom->GetNumAnimation(szName);
}

void CPartObject::SetSocketFlag(_char bitFlag)
{
	m_UseSocketMatrixFlag = bitFlag;
}

const _float4x4* CPartObject::GetBoneMatrix(const char* szBone) const
{
	return m_pVIBufferCom->GetCombinedTransformationMatrixPtr(szBone);;
}

HRESULT CPartObject::Bind_ShaderResources()
{
	if (nullptr == m_pShaderCom)
	    return E_FAIL;

	m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
	m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
	m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
	m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();
	m_pBoneMatrixEffect = m_pShaderCom->GetVariable("g_BoneMatrices")->AsMatrix();
	return S_OK;
}

HRESULT CPartObject::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));
	m_pBoneMatrixEffect->SetMatrixArray(reinterpret_cast<const float*>(m_pVIBufferCom->GetBoneMatrices(iMeshIndex)), 0, m_pVIBufferCom->GetMeshNumBones(iMeshIndex));
	
	ID3D11ShaderResourceView* pResourceVeiw = {};
	m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
	if (pResourceVeiw)
	    m_pSRVEffect->SetResource(pResourceVeiw);
	
	return S_OK;
}

void CPartObject::UpdateCombinedMatrix(const _float4x4* SocketMat)
{
	if (nullptr == SocketMat)
		SocketMat = m_SocketMatrix;

	//이거 회전 바꾸기
	if (SocketMat)
	{
		_matrix ScoketMatrix = XMLoadFloat4x4(SocketMat);

		if (m_UseSocketMatrixFlag & 0b00000001)
		{
			for (_uInt i = 0; i < 3; ++i)
				ScoketMatrix.r[i] = XMVector4Normalize(ScoketMatrix.r[i]);
		}
		if (m_UseSocketMatrixFlag & 0b00000010)
		{
			ScoketMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
			ScoketMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			ScoketMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		}

		XMStoreFloat4x4(&m_CombinedWorldMatrix,
			XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * ScoketMatrix * XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat()));
	}
	else
		XMStoreFloat4x4(&m_CombinedWorldMatrix,
			XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat()));
}

CGameObject* CPartObject::Clone(void* pArg)
{
	return nullptr;
}

void CPartObject::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	
}

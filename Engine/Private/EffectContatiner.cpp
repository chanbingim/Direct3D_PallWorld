#include "EffectContatiner.h"

#include "GameInstance.h"

#include "EffectManager.h"
#include "EffectPartObject.h"
#include "StringHelper.h"

CEffectContatiner::CEffectContatiner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CGameObject(pDevice, pContext)
{
}

CEffectContatiner::CEffectContatiner(const CEffectContatiner& rhs) :
	CGameObject(rhs)
{
	for(auto&iter : rhs.m_PartObjects)
		m_PartObjects.emplace(iter.first, nullptr);
}

HRESULT CEffectContatiner::Initalize_Prototype()
{
	return S_OK;
}

HRESULT CEffectContatiner::Initalize_Prototype(const char* pFilePath)
{
	if (FAILED(ReadFileData(pFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectContatiner::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	auto iter = m_PartObjects.begin();
	if (iter != m_PartObjects.end())
	{
		if (nullptr == m_PartObjects.begin()->second)
			Bind_PartObject();
	}

	return S_OK;
}

void CEffectContatiner::Priority_Update(_float fDeletaTime)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Priority_Update(fDeletaTime);
}

void CEffectContatiner::Update(_float fDeletaTime)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Update(fDeletaTime);

	if (m_bIsDissolve)
	{
		for (auto& Pair : m_PartObjects)
		{
			if (Pair.second->IsDead())
				break;
		}

		if (m_bIsDissolveEndFunc)
			m_bIsDissolveEndFunc();
		SetDead(true);
	}
}

void CEffectContatiner::Late_Update(_float fDeletaTime)
{
	ComputeCombinedMatrix();
	for (auto& Pair : m_PartObjects)
		Pair.second->Late_Update(fDeletaTime);
}

HRESULT CEffectContatiner::Render()
{
	return S_OK;
}

void CEffectContatiner::EffectDead(function<void()> EndEffectFunc)
{
	m_bIsDissolve = true;
	m_bIsDissolveEndFunc = EndEffectFunc;

	for (auto& pPartEffects : m_PartObjects)
		static_cast<CEffectPartObject*>(pPartEffects.second)->EffectDead();
}

#ifdef _DEBUG
void CEffectContatiner::ADD_EffectPartObject(const wstring& EffectPartTag, void* pArg)
{
	auto pPartEffect = m_pGameInstance->EffectClone_Object(ENUM_CLASS(CEffectManager::EFFECT_TYPE::PART_OBJECT), EffectPartTag.c_str());
	auto iter = m_PartObjects.find(EffectPartTag);
	if (iter == m_PartObjects.end())
		m_PartObjects.emplace(EffectPartTag, pPartEffect);
	else
		iter->second = pPartEffect;
}

void CEffectContatiner::Remove_EffectPartObject(const WCHAR* EffectPartTag)
{
	auto iter = m_PartObjects.find(EffectPartTag);
	if (iter != m_PartObjects.end())
	{
		Safe_Release(iter->second);
		m_PartObjects.erase(iter);
	}
}

void CEffectContatiner::ExportData(void* pArg)
{
	EFFECT_SAVE_SESC* SaveDesc = static_cast<EFFECT_SAVE_SESC*>(pArg);
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;
	char ResoucePath[MAX_PATH] = {};

	char    ConvertPath[MAX_PATH];
	ofstream file(SaveDesc->szFilePath, flag);
	if (file.is_open())
	{
		CStringHelper::ConvertWideToUTF(m_ObejctTag.c_str(), ConvertPath);
		file << ConvertPath << endl;
		file << m_PartObjects.size() << endl;
		for (auto& pEffectPart : m_PartObjects)
		{
			CStringHelper::ConvertWideToUTF(pEffectPart.first.c_str(), ConvertPath);
			file << ConvertPath << endl;
		}
	}

	file.close();
}

#endif // _DEBUG

CGameObject* CEffectContatiner::FindPartObject(const WCHAR* szTag)
{
	auto pPartEffect = m_PartObjects.find(szTag);
	if (pPartEffect == m_PartObjects.end())
		return nullptr;

	return pPartEffect->second;
}

HRESULT CEffectContatiner::ReadFileData(const char* szFilePath)
{
	ios_base::openmode flag;
	flag = ios::in;

	char     ResoucePath[MAX_PATH] = {};
	WCHAR    ConvertPath[MAX_PATH];

	ifstream file(szFilePath, flag);
	if (file.is_open())
	{
		file >> ResoucePath;
		CStringHelper::ConvertUTFToWide(ResoucePath, ConvertPath);
		m_ObejctTag = ConvertPath;
			
		_Int iNumPartObject = {};
		file >> iNumPartObject;
		
		for (_Int i = 0; i < iNumPartObject; ++i)
		{
			file >> ResoucePath;
			CStringHelper::ConvertUTFToWide(ResoucePath, ConvertPath);
			m_PartObjects.emplace(ConvertPath, nullptr);
		}
	}
	else
		return E_FAIL;

	file.close();
	return S_OK;
}

HRESULT CEffectContatiner::Bind_PartObject()
{
	for (auto& iter : m_PartObjects)
	{
		auto pPartObject = m_pGameInstance->EffectClone_Object(ENUM_CLASS(CEffectManager::EFFECT_TYPE::PART_OBJECT), iter.first.c_str(), nullptr);
		pPartObject->SetParent(this);
		iter.second = pPartObject;
	}

	return S_OK;
}

void CEffectContatiner::ComputeCombinedMatrix()
{
	if (m_pParent)
	{
		_matrix ParentWorldMat = XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat());
		for (_uInt i = 0; i < 3; i++)
			ParentWorldMat.r[i] = XMVector3Normalize(ParentWorldMat.r[i]);

		XMStoreFloat4x4(&m_CombinedMatrix, XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * ParentWorldMat);
	}
	else
		m_CombinedMatrix = m_pTransformCom->GetWorldMat();
}

CEffectContatiner* CEffectContatiner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffectContatiner* pEffectPartObject = new CEffectContatiner(pDevice, pContext);
	if (FAILED(pEffectPartObject->Initalize_Prototype()))
	{
		Safe_Release(pEffectPartObject);
		MSG_BOX("CREAT FAIL : EFFECT PARTOBJECT");
	}
	return pEffectPartObject;
}

CEffectContatiner* CEffectContatiner::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pFilePath)
{
	CEffectContatiner* pEffectPartObject = new CEffectContatiner(pDevice, pContext);
	if (FAILED(pEffectPartObject->Initalize_Prototype(pFilePath)))
	{
		Safe_Release(pEffectPartObject);
		MSG_BOX("CREAT FAIL : EFFECT PARTOBJECT");
	}
	return pEffectPartObject;
}

CGameObject* CEffectContatiner::Clone(void* pArg)
{
	CEffectContatiner* pEffectPartObject = new CEffectContatiner(*this);
	if (FAILED(pEffectPartObject->Initialize(pArg)))
	{
		Safe_Release(pEffectPartObject);
		MSG_BOX("CREAT FAIL : EFFECT PARTOBJECT");
	}
	return pEffectPartObject;
}

void CEffectContatiner::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);

	m_PartObjects.clear();
}
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
}

void CEffectContatiner::Late_Update(_float fDeletaTime)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Late_Update(fDeletaTime);
}

HRESULT CEffectContatiner::Render()
{
	return S_OK;
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
		m_PartObjects.emplace(ConvertPath, nullptr);
	}
	else
		return E_FAIL;

	file.close();
	return S_OK;
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
	if (FAILED(pEffectPartObject->Initalize_Prototype()))
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
#include "EffectManager.h"

#include "GameObject.h"
#include "Texture.h"
#include "Model.h"
#include "VIBuffer_Rect.h"
#include "Shader.h"

#include "EffectPartObject.h"
#include "EffectContatiner.h"

CEffectManager::CEffectManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : 
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CEffectManager::Initalize()
{
	if(FAILED(LoadEffectTexture()))
		return E_FAIL;

	if(FAILED(LoadEffectShader()))
		return E_FAIL;

	if(FAILED(LoadEffectModel()))
		return E_FAIL;

	if (FAILED(LoadPartEffect()))
		return E_FAIL;

	if (FAILED(LoadContainerEffect()))
		return E_FAIL;

	return S_OK;
}

CTexture* CEffectManager::GetTextureResource(const WCHAR* TextureTag)
{
	auto pair = m_pTextures.find(TextureTag);
	if (pair == m_pTextures.end())
		return nullptr;

	Safe_AddRef(pair->second);
	return pair->second;
}

CShader* CEffectManager::GetShaderResource(const WCHAR* ShaderTag)
{
	auto pair = m_pShaders.find(ShaderTag);
	if (pair == m_pShaders.end())
		return nullptr;

	Safe_AddRef(pair->second);
	return pair->second;
}

CComponent* CEffectManager::GetModelResource(const WCHAR* ModelTag)
{
	auto pair = m_pVIBuffers.find(ModelTag);
	if (pair == m_pVIBuffers.end())
		return nullptr;

	Safe_AddRef(pair->second);
	return pair->second;
}

CGameObject* CEffectManager::Clone_Object(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag, void* pArg)
{
	auto pair = m_pEffects[ENUM_CLASS(eEffectType)].find(szEffectTag);
	if (pair == m_pEffects[ENUM_CLASS(eEffectType)].end())
		return nullptr;

	return pair->second->Clone(pArg);
}

HRESULT CEffectManager::ADD_Effects(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag, CGameObject* pEffect)
{
	auto pair = m_pEffects[ENUM_CLASS(eEffectType)].find(szEffectTag);
	if (pair == m_pEffects[ENUM_CLASS(eEffectType)].end())
		m_pEffects[ENUM_CLASS(eEffectType)].emplace(szEffectTag, pEffect);
	else
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectManager::Remove_Effect(EFFECT_TYPE eEffectType, const WCHAR* szEffectTag)
{
	auto pair = m_pEffects[ENUM_CLASS(eEffectType)].find(szEffectTag);
	if (pair == m_pEffects[ENUM_CLASS(eEffectType)].end())
		return E_FAIL;

	m_pEffects[ENUM_CLASS(eEffectType)].erase(pair);
	return S_OK;
}

HRESULT CEffectManager::LoadEffectTexture()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddatai64_t  fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst64("../Bin/Resources/Textures/Effects/*.png*", &fd);

	if (handle == -1)
		return E_FAIL;;

	int iResult = 0;

	_wstring FrontPath = TEXT("../Bin/Resources/Textures/Effects/");
	while (iResult != -1)
	{
		int iLength = strlen(fd.name) + 1;
		WCHAR* pFileName = new WCHAR[iLength];
		ZeroMemory(pFileName, sizeof(WCHAR) * iLength);

		// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
		MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pFileName, iLength);

		_wstring FullPath = FrontPath + pFileName;
		auto pTexture = CTexture::Create(m_pDevice, m_pContext, FullPath.c_str(), 1);
		_wstring szTextureTag = pFileName;
		m_pTextures.emplace(szTextureTag, pTexture);
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext64(handle, &fd);
	}

	_findclose(handle);
	return S_OK;
}

HRESULT CEffectManager::LoadEffectShader()
{
	m_pShaders.emplace(TEXT("Effect_Model_Shader"), CShader::Create(m_pDevice, m_pContext, VTX_MESH::Elements, VTX_MESH::iNumElements, TEXT("../Bin/ShaderFiles/Effect_Mesh.hlsl")));
	m_pShaders.emplace(TEXT("Effect_VIRect_Shader"), CShader::Create(m_pDevice, m_pContext, VTX_TEX::Elements, VTX_TEX::iNumElements, TEXT("../Bin/ShaderFiles/Effect_Rect.hlsl")));
	return S_OK;
}

HRESULT CEffectManager::LoadEffectModel()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddatai64_t  fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst64("../Bin/Resources/Models/Effects/*.fbx*", &fd);

	if (handle == -1)
		return E_FAIL;

	int iResult = 0;

	_string FrontPath = "../Bin/Resources/Models/Effects/";
	_matrix PreMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
	while (iResult != -1)
	{
		int iLength = strlen(fd.name) + 1;
		WCHAR* pFileName = new WCHAR[iLength];
		ZeroMemory(pFileName, sizeof(WCHAR) * iLength);
		// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
		MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pFileName, iLength);

		_string FullPath = FrontPath + fd.name;
		auto pModel = CModel::Create(m_pDevice, m_pContext, MODEL_TYPE::NONANIM, FullPath.c_str(), PreMatrix);
		_wstring szTextureTag = pFileName;
		m_pVIBuffers.emplace(szTextureTag, pModel);
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext64(handle, &fd);
	}

	_findclose(handle);


	m_pVIBuffers.emplace(TEXT("VI_Rect"), CVIBuffer_Rect::Create(m_pDevice, m_pContext));
	return S_OK;
}

HRESULT CEffectManager::LoadPartEffect()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddatai64_t  fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst64("../Bin/Resources/Effects/PartEffects/*.txt*", &fd);

	if (handle == -1)
		return S_OK;

	int iResult = 0;

	_string FrontPath = "../Bin/Resources/Effects/PartEffects/";
	while (iResult != -1)
	{
		int iLength = strlen(fd.name) + 1;
		WCHAR* pFileName = new WCHAR[iLength];
		ZeroMemory(pFileName, sizeof(WCHAR) * iLength);
		// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
		MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pFileName, iLength);

		_string FullPath = FrontPath + fd.name;

		auto pEffectPart = CEffectPartObject::Create(m_pDevice, m_pContext, FullPath.c_str());
		m_pEffects[ENUM_CLASS(EFFECT_TYPE::PART_OBJECT)].emplace(pEffectPart->GetObjectTag(), pEffectPart);
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext64(handle, &fd);
	}

	_findclose(handle);
	return S_OK;
}

HRESULT CEffectManager::LoadContainerEffect()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddatai64_t  fd;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	intptr_t handle = _findfirst64("../Bin/Resources/Effects/ContainerEffects/*.txt*", &fd);

	if (handle == -1)
		return S_OK;

	int iResult = 0;

	_string FrontPath = "../Bin/Resources/Effects/ContainerEffects/";
	while (iResult != -1)
	{
		int iLength = strlen(fd.name) + 1;
		WCHAR* pFileName = new WCHAR[iLength];
		ZeroMemory(pFileName, sizeof(WCHAR) * iLength);
		// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수
		MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pFileName, iLength);

		_string FullPath = FrontPath + fd.name;

		auto pEffect = CEffectContatiner::Create(m_pDevice, m_pContext, FullPath.c_str());
		m_pEffects[ENUM_CLASS(EFFECT_TYPE::CONTATINER)].emplace(pEffect->GetObjectTag(), pEffect);
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext64(handle, &fd);
	}

	_findclose(handle);
	return S_OK;
}

CEffectManager* CEffectManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	auto pEffectManager = new CEffectManager(pDevice, pContext);
	if (FAILED(pEffectManager->Initalize()))
	{
		Safe_Release(pEffectManager);
		MSG_BOX("CREATE FAIL : EFFECT MANAGER");
	}
	return pEffectManager;
}

void CEffectManager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& pair : m_pTextures)
		Safe_Release(pair.second);
	m_pTextures.clear();

	for (auto& pair : m_pShaders)
		Safe_Release(pair.second);
	m_pShaders.clear();

	for (auto& pair : m_pVIBuffers)
		Safe_Release(pair.second);
	m_pVIBuffers.clear();

	for (_uInt i = 0; i < ENUM_CLASS(EFFECT_TYPE::END); ++i)
	{
		for (auto& pair : m_pEffects[i])
			Safe_Release(pair.second);
		m_pEffects[i].clear();
	}

}

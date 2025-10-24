#include "RenderTagetManager.h"

#include "RenderTarget.h"

CRenderTagetManager::CRenderTagetManager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	m_pDevice(pDevice),
	m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderTagetManager::Add_RenderTarget(const _wstring& strTargetTag, _uInt iSizeX, _uInt iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	// 같은 이름으로 만들어진 랜더타겟이 있다면 실패
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	//랜더 타겟을 만들어서 랜더타겟 리스트에다 저장한다.
	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iSizeX, iSizeY, ePixelFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTagetManager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	//랜더 타겟을 연산을 하기위한 용도로 구분하여 묶어둔다.

	// 이때 묶어둘 랜더타겟이 없다면 실패
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	// 랜더 타겟을 묶어둘 그룹을 찾아서 그룹이 있다면
	// 그룹에 추가하고 그룹이 없다면 그룹을 만들어준다.
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>       MRTList;
		MRTList.push_back(pRenderTarget);
		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);
	return S_OK;

}

HRESULT CRenderTagetManager::Begin_MRT(const _wstring& strMRTTag)
{
	// 랜더 타겟을 내가 지정한 그룹으로 바꾼다.
	// 이과정에서 이전에 세팅되어있는 백버퍼의 뎁스 스텐실 뷰와
	// 랜더타겟을 저장해둔다 이 저장해둔 녀석은 End 호출때 다시 되돌릴 용도로 사용
	m_pContext->OMGetRenderTargets(1, &m_pBackBufferRTV, &m_pOriginalDSV);

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	// 이때 랜더타겟은 최대 8개까지 바인딩이 가능하다.
	// 때문에 8개의 배열을 선언하고 nullptr 초기화를 해둔뒤
	// 사용할 녀석들의 값을 채워서 바인딩한다.
	_uInt iNumRenderTargets = {};
	ID3D11RenderTargetView* pRenderTargets[8] = {};

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear();
		pRenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}
	
	if (0 >= iNumRenderTargets)
		return E_FAIL;

	m_pContext->OMSetRenderTargets(iNumRenderTargets, pRenderTargets, m_pOriginalDSV);

	return S_OK;
}

HRESULT CRenderTagetManager::End_MRT()
{
	// 내가 세팅한 랜더타겟에 기록이 끝났다면
	// End를 호출하여 기존 백버퍼의 값으로 되돌리는 역할을 한다.
	ID3D11RenderTargetView* pRenderTargets[8] = { m_pBackBufferRTV };



	m_pContext->OMSetRenderTargets(8, pRenderTargets, m_pOriginalDSV);

	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pOriginalDSV);

	return S_OK;
}

HRESULT CRenderTagetManager::Bind_RenderTarget(const _wstring& strTargetTag, CShader* pShader, const _char* pConstantName)
{
	// 셰이더를 넘겨받아 특정 셰이더 상수의 이름에 내가 지정한
	// 랜더타겟을 바인딩 하는 용도이다.

	// 내가 원하는 타겟이 없다면 실패
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(pShader, pConstantName);
}

#ifdef _DEBUG
ID3D11ShaderResourceView* CRenderTagetManager::GetRenderTargetRSV(const _wstring& strTargetTag)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->GetRSV();
}
HRESULT CRenderTagetManager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;
	return pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTagetManager::Render_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);

	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Render_Debug(pShader, pVIBuffer);
	}

	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CRenderTagetManager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto    iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CRenderTagetManager::Find_MRT(const _wstring& strMRTTag)
{
	auto    iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

CRenderTagetManager* CRenderTagetManager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CRenderTagetManager(pDevice, pContext);
}

void CRenderTagetManager::Free()
{
	__super::Free();

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

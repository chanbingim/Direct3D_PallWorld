#include "TechSelectView.h"

#include "GameInstance.h"
#include "TechSelectPreView.h"
#include "TechListViewSlot.h"

#include "ItemManager.h"
#include "TechManager.h"

CTechSelectView::CTechSelectView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext),
	m_pTechManager(CTechManager::GetInstance()),
	m_pItemManager(CItemManager::GetInstance())
{
	Safe_AddRef(m_pTechManager);
	Safe_AddRef(m_pItemManager);
}

CTechSelectView::CTechSelectView(const CTechSelectView& rhs) :
	CBackGround(rhs),
	m_pTechManager(CTechManager::GetInstance()),
	m_pItemManager(CItemManager::GetInstance())
{
	Safe_AddRef(m_pTechManager);
	Safe_AddRef(m_pItemManager);
}

HRESULT CTechSelectView::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;
	return S_OK;
}

HRESULT CTechSelectView::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(ADD_TechViewListSlot()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_vImageColor = { 0.21f, 0.32f, 0.36f, 0.5f };
	return S_OK;
}

void CTechSelectView::Update(_float fDeletaTime)
{
	auto& TechList = m_pTechManager->GetCategoryTypeTechList(m_eTechType);

	auto pTechID = next(TechList.begin(), m_iViewStartIndex);
	for (size_t i = 0; i < m_pViewSlotList.size(); ++i)
	{
		if (pTechID == TechList.end())
		{
			m_pViewSlotList[i]->SetViewItemInfo(nullptr);
		}
		else
		{
			auto& TechInfo = m_pTechManager->GetTechData((*pTechID));
			auto ItemDesc = m_pItemManager->GetItemInfo(TechInfo.ReturnItemID);

			//여기서 슬롯에다가 세팅
			m_pViewSlotList[i]->SetViewItemInfo(ItemDesc);
			pTechID++;
		}
	}


	if(m_SelectViewTechItem)
		m_pSelectPreView->SetViewItemInfo(m_pItemManager->GetItemInfo(m_SelectViewTechItem->ReturnItemID));

	for (auto pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CTechSelectView::Late_Update(_float fDeletaTime)
{
	for (auto pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CTechSelectView::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Bind_RawValue("g_vColor", &m_vImageColor, sizeof(_float4));

	m_pShaderCom->Update_Shader(4);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	return S_OK;
}

void CTechSelectView::SetViewTechType(TECH_TYPE eType)
{
	m_eTechType = eType;
}

HRESULT CTechSelectView::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_Tech_Crate_OutCircle"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxTex"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTechSelectView::ADD_Childs()
{
	_float3	vSclae = m_pTransformCom->GetScale();
	CTechSelectPreView::GAMEOBJECT_DESC pObjectDesc = {};
	pObjectDesc.pParent = this;
	pObjectDesc.vScale = { vSclae.x * 0.5f, vSclae.y * 0.5f, 0.f };

	m_pSelectPreView = CTechSelectPreView::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pSelectPreView->SetZOrder(m_iZOrder + 1);
	if (FAILED(m_pSelectPreView->Initialize(&pObjectDesc)))
		return E_FAIL;

	ADD_Child(m_pSelectPreView);
	return S_OK;
}

HRESULT CTechSelectView::ADD_TechViewListSlot()
{
	m_pViewSlotList.reserve(12);
	CGameObject::GAMEOBJECT_DESC pObejctDesc = {};
	pObejctDesc.pParent = this;

	//중점을 가져온다
	_float2 vCircleCenterPos = GetScreenPos();

	// 안쪽원 반지름 + (밖깥원 반지름 - 안쪽원 반지름) * 0.5 는 중간원 반지름
	// 그러면 중점 X에다가 중간원의 반지름을 더하면 중간원의 0도일때의 좌표가 나옴
	_float InCircleRadius = (_float)m_pSelectPreView->GetRectSize().right;
	_float OutCircleRadius = (_float)GetRectSize().right;

	_float RadiusDiff = OutCircleRadius - InCircleRadius;
	_float MiddleCircleRadius = InCircleRadius - vCircleCenterPos.x + RadiusDiff * 0.5f;

	pObejctDesc.vScale = { RadiusDiff, RadiusDiff, 0.f};
	for (_float i = 15; i < 360.f; i += 30.f)
	{
		pObejctDesc.vPosition.x = MiddleCircleRadius * cosf(XMConvertToRadians(i));
		pObejctDesc.vPosition.y =  MiddleCircleRadius * sinf(XMConvertToRadians(i));

		CTechListViewSlot* pTechListSlot = static_cast<CTechListViewSlot*>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Tech_List_View_Slot"), &pObejctDesc));
		if (nullptr == pTechListSlot)
			return E_FAIL;

		pTechListSlot->SetZOrder(m_iZOrder + 1);
		m_pViewSlotList.push_back(pTechListSlot);
		ADD_Child(pTechListSlot);
	}

	return S_OK;
}

CTechSelectView* CTechSelectView::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTechSelectView* pTechSelectView = new CTechSelectView(pDevice, pContext);
	if (FAILED(pTechSelectView->Initalize_Prototype()))
	{
		Safe_Release(pTechSelectView);
		MSG_BOX("CREATE FAIL : TECH SELECT VIEW");
	}
	return pTechSelectView;
}

CUserInterface* CTechSelectView::Clone(void* pArg)
{
	CTechSelectView* pTechSelectView = new CTechSelectView(*this);
	if (FAILED(pTechSelectView->Initialize(pArg)))
	{
		Safe_Release(pTechSelectView);
		MSG_BOX("CLONE FAIL : TECH SELECT VIEW");
	}
	return pTechSelectView;
}

void CTechSelectView::Free()
{
	__super::Free();

	for (auto iter : m_pViewSlotList)
		Safe_Release(iter);
	 
	Safe_Release(m_pSelectPreView);

	Safe_Release(m_pItemManager);
	Safe_Release(m_pTechManager);
}

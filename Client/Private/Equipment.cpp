#include "Equipment.h"

#include "GameInstance.h"
#include "EquipSlot.h"
#include "ItemStruct.h"

#include "PlayerView.h"
#include "EquipSlotTittle.h"

CEquipment::CEquipment(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CBackGround(pDevice, pContext)
{
}

CEquipment::CEquipment(const CEquipment& rhs) :
	CBackGround(rhs)
{
}

HRESULT CEquipment::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEquipment::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	m_SlotCount = 15;
	if(FAILED(ADD_Childs()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_eType = OBJECT_TYPE::STATIC;
	m_iZOrder = 3;
	return S_OK;
}

void CEquipment::Update(_float fDeletaTime)
{
	for (auto& pChild : m_pChildList)
		pChild->Update(fDeletaTime);
}

void CEquipment::Late_Update(_float fDeletaTime)
{
	for (auto& pChild : m_pChildList)
		pChild->Late_Update(fDeletaTime);
}

HRESULT CEquipment::Render()
{
	if (FAILED(Apply_ConstantShaderResources()))
		return E_FAIL;

	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	for (_uInt i = 0; i < 7; ++i)
		m_pSlotFontCom[i]->Render();

	for (auto& pChild : m_pChildList)
		pChild->Render();

	return S_OK;
}

HRESULT CEquipment::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pEquip_Color = m_pShaderCom->GetVariable("g_Color")->AsVector();
	return S_OK;
}

HRESULT CEquipment::Apply_ConstantShaderResources()
{
	m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
	m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_ViewMatrix));
	m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_ProjMatrix));

	_float     vColor[4] = { 0.2f, 0.2f, 0.2f, 0.2f };
	m_pEquip_Color->SetFloatVector(vColor);
	return S_OK;
}

HRESULT CEquipment::ADD_Components()
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_InGameMenu_Category_BackGround"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEquipment::ADD_Childs()
{
	CUserInterface::GAMEOBJECT_DESC Desc;
	ZeroMemory(&Desc, sizeof(CUserInterface::GAMEOBJECT_DESC));

	CUserInterface::GAMEOBJECT_DESC SlotTittleDesc;
	ZeroMemory(&SlotTittleDesc, sizeof(CUserInterface::GAMEOBJECT_DESC));
	SlotTittleDesc.pParent = this;
	SlotTittleDesc.vScale = { 50, 20, 0.f };

	_float3 ParentScale = m_pTransformCom->GetScale();
	_float SlotPosX = ParentScale.x * 0.4f;
	_float SlotPosY = ParentScale.y * 0.32f;

	auto pBaseTittle = CEquipSlotTittle::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == pBaseTittle)
		return E_FAIL;

	/* Weapon */
	_float3 vWeaponPos = { -SlotPosX, -SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { -SlotPosX, -SlotPosY - 40.f, 0.f };
	m_pSlotFontCom[0] = static_cast<CEquipSlotTittle *>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[0]->SetText(TEXT("무기"));
	CreateEquipSlot(4, ENUM_CLASS(EUQIP_TYPE::WEAPON), vWeaponPos, 0);

	/* Head */
	SlotPosX = ParentScale.x * 0.15f;
	_float3 vHeadPos = { SlotPosX, -SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { SlotPosX, -SlotPosY - 41 , 0.f };
	m_pSlotFontCom[1] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[1]->SetText(TEXT("머리"));
	CreateEquipSlot(1, ENUM_CLASS(EUQIP_TYPE::HEAD), vHeadPos, 0);

	/* Body */
	SlotPosY = ParentScale.y * 0.17f;
	_float3 vBodyPos = { SlotPosX, -SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { SlotPosX, -SlotPosY - 40 , 0.f };
	m_pSlotFontCom[2] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[2]->SetText(TEXT("몸"));
	CreateEquipSlot(1, ENUM_CLASS(EUQIP_TYPE::BODY), vBodyPos, 0);

	/* Shield */
	SlotPosY = ParentScale.y * 0.02f;
	_float3 vShieldPos = { SlotPosX, -SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { SlotPosX, -SlotPosY - 40 , 0.f };
	m_pSlotFontCom[3] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[3]->SetText(TEXT("쉴드"));
	CreateEquipSlot(1, ENUM_CLASS(EUQIP_TYPE::SHIELD), vShieldPos, 0);

	/* Glider */
	SlotPosY = ParentScale.y * 0.13f;
	_float3 vGliderPos = { SlotPosX, SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { SlotPosX, SlotPosY - 40 , 0.f };
	m_pSlotFontCom[4] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[4]->SetText(TEXT("글라이더"));
	CreateEquipSlot(1, ENUM_CLASS(EUQIP_TYPE::GLIDER), vGliderPos, 0);

	/* Accessory */
	SlotPosX = ParentScale.x * 0.4f;
	SlotPosY = ParentScale.y * 0.2f;
	_float3 vAccessoryPos = { -SlotPosX, SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { -SlotPosX, SlotPosY - 40 , 0.f };
	m_pSlotFontCom[5] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[5]->SetText(TEXT("악세서리"));
	CreateEquipSlot(2, ENUM_CLASS(EUQIP_TYPE::ACCESSORY), vAccessoryPos, 0);

	/* Food */
	SlotPosX = ParentScale.x * 0.3f;
	SlotPosY = ParentScale.y * 0.46f;
	_float3 vFoodPos = { -SlotPosX, SlotPosY, 0.f };
	SlotTittleDesc.vPosition = { -SlotPosX, SlotPosY - 21 , 0.f };
	m_pSlotFontCom[6] = static_cast<CEquipSlotTittle*>(pBaseTittle->Clone(&SlotTittleDesc));
	m_pSlotFontCom[6]->SetText(TEXT("음식"));
	CreateEquipSlot(5, ENUM_CLASS(EUQIP_TYPE::FOOD), vFoodPos, 1);

	CPlayerView::VIEWER_DESC ViewerDesc = {};
	ViewerDesc.pParent = this;
	ViewerDesc.vScale = { 280, 500, 0.f};
	ViewerDesc.vPosition = {-70.f, 0.f, 0.f};
	ViewerDesc.fHeight = g_iWinSizeX;
	ViewerDesc.fWidth = g_iWinSizeY;

	//부모위치잡고 세팅
	m_pPlayerView = CPlayerView::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pPlayerView)
		return E_FAIL;
	m_pPlayerView->SetZOrder(m_iZOrder + 1);
	m_pPlayerView->Initialize(&ViewerDesc);
	ADD_Child(m_pPlayerView);

	Safe_Release(pBaseTittle);
	return S_OK;
}

HRESULT CEquipment::CreateEquipSlot(_uInt iCount, _uInt iSlotType, _float3 vStartPos, _uInt MajorType)
{
	CEquipSlot::EQUIP_SLOT_DESC Desc = {};

	Desc.eSlotType = EUQIP_TYPE(iSlotType);
	Desc.pParent = this;
	Desc.vScale = { 50, 50, 0.f };
	for (_uInt i = 0; i < iCount; ++i)
	{
		Desc.vPosition = vStartPos;
		if (MajorType)
			Desc.vPosition.x += ((Desc.vScale.x + 20) * i);
		else
			Desc.vPosition.y += ((Desc.vScale.y + 20) * i);

		CEquipSlot* pEquipSlot = CEquipSlot::Create(m_pGraphic_Device, m_pDeviceContext);
		if (nullptr == pEquipSlot)
			return E_FAIL;

		Desc.iNumberSlot = i;
		//만들어보고 여기서 위치까지 잡아주자
		pEquipSlot->Initialize(&Desc);
		m_pEquipSlot.push_back(pEquipSlot);
		ADD_Child(pEquipSlot);
	}

	return S_OK;
}

CEquipment* CEquipment::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEquipment* pEquipMenet = new CEquipment(pDevice, pContext);
	if (FAILED(pEquipMenet->Initalize_Prototype()))
	{
		Safe_Release(pEquipMenet);
		MSG_BOX("CREATE FAIL : EQUIP MENT");
	}
	return pEquipMenet;
}

CUserInterface* CEquipment::Clone(void* pArg)
{
	CEquipment* pEquipMenet = new CEquipment(*this);
	if (FAILED(pEquipMenet->Initialize(pArg)))
	{
		Safe_Release(pEquipMenet);
		MSG_BOX("CLONE FAIL : EQUIP MENT");
	}
	return pEquipMenet;
}

void CEquipment::Free()
{
	__super::Free();

	for (auto& iter : m_pSlotFontCom)
		Safe_Release(iter);

	for (auto& iter : m_pEquipSlot)
		Safe_Release(iter);

	m_pEquipSlot.clear();
	Safe_Release(m_pPlayerView);
}

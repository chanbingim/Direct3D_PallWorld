#include "Equipment.h"

#include "GameInstance.h"
#include "EquipSlot.h"
#include "PlayerView.h"

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

	Desc.pParent = this;
	Desc.vScale = { 40, 40, 0.f };
	//슬롯 생성
	for (_uInt i = 0; i < m_SlotCount; ++i)
	{
		CEquipSlot* pEquipSlot = CEquipSlot::Create(m_pGraphic_Device, m_pDeviceContext);
		if (nullptr == pEquipSlot)
			return E_FAIL;

		//만들어보고 여기서 위치까지 잡아주자
		pEquipSlot->Initialize(&Desc);
		m_pEquipSlot.push_back(pEquipSlot);
		ADD_Child(pEquipSlot);
	}

	Desc.vScale = { 400, 500, 0.f};
	//부모위치잡고 세팅
	m_pPlayerView = CPlayerView::Create(m_pGraphic_Device, m_pDeviceContext);
	if (nullptr == m_pPlayerView)
		return E_FAIL;
	m_pPlayerView->Initialize(&Desc);
	ADD_Child(m_pPlayerView);

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

	for (auto& iter : m_pEquipSlot)
		Safe_Release(iter);

	m_pEquipSlot.clear();
	Safe_Release(m_pPlayerView);
}

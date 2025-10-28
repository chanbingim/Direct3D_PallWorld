#include "BossHealthBar.h"

#include "GameInstance.h"
#include "BossTitleName.h"

#include "PellBase.h"

CBossHealthBar::CBossHealthBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CProgressBar(pDevice, pContext)
{
}

CBossHealthBar::CBossHealthBar(const CBossHealthBar& rhs) :
	CProgressBar(rhs)
{
}

HRESULT CBossHealthBar::Initalize_Prototype()
{
	if (FAILED(__super::Initalize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHealthBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(ADD_Components()))
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_MaxHealthText = TEXT("/");
	if (m_pPellInfo)
		m_MaxHealthText += to_wstring(m_pPellInfo->MaxHealth);
	else
		m_MaxHealthText += to_wstring(1000);

	m_vColor = {1.f, 0.f, 0.f, 1.f};
	m_vPrePercentColor = { 0.5f, 0.5f, 0.5f, 0.8f };

	_vector TextSize = m_pHealthFontCom->GetFontBoundBox(m_MaxHealthText.c_str());
	m_MaxHealthFontPos = { m_UISize.right - TextSize.m128_f32[0], m_UISize.top -TextSize.m128_f32[1] * 0.5f};
	return S_OK;
}

void CBossHealthBar::Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	LerpAnimation(fDeletaTime);
	if (m_pPellInfo)
	{
		m_fPercent = m_pPellInfo->CurHealth / m_pPellInfo->MaxHealth;
		m_HealthText = to_wstring((_uInt)m_pPellInfo->CurHealth);
	}
	else
	{
		m_fPercent = 1.f;
		m_HealthText = to_wstring(1000);
	}

	_vector TextSize = m_pHealthFontCom->GetFontBoundBox(m_HealthText.c_str());
	m_HealthFontPos = { m_MaxHealthFontPos.x -TextSize.m128_f32[0], m_MaxHealthFontPos.y };
}

void CBossHealthBar::Late_Update(_float fDeletaTime)
{
	if (VISIBILITY::HIDDEN == m_eVisible)
		return;

	if (m_pOwnerBoss->IsDead())
	{
		m_pOwnerBoss->UnBind_DamageEvent(this);
		Safe_Release(m_pOwnerBoss);
	}

	m_pGameInstance->Add_RenderGroup(RENDER::SCREEN_UI, this);
}

HRESULT CBossHealthBar::Render()
{
	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(0);
	m_pTextureCom->SetTexture(0, 0);
	m_pVIBufferCom->Render_VIBuffer();

	m_pShader_Percent->SetRawValue(&m_fPrePercent, 0, sizeof(_float));
	m_pShader_Color->SetFloatVector((float*)&m_vPrePercentColor);
	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 1);
	m_pVIBufferCom->Render_VIBuffer();

	Apply_ConstantShaderResources();
	m_pShaderCom->Update_Shader(1);
	m_pTextureCom->SetTexture(0, 1);
	m_pVIBufferCom->Render_VIBuffer();

	

	m_pMaxHealthFontCom->Render(m_MaxHealthText.c_str(), { 1.f, 1.f, 0.f, 1.f });
	m_pHealthFontCom->Render(m_HealthText.c_str(), { 1.f, 1.f, 1.f, 1.f });
	return S_OK;
}

void CBossHealthBar::SetBossInfo(CPellBase* pBossPell)
{
	//이벤트 제거및 레퍼런스 감소
	if (nullptr != m_pOwnerBoss)
	{
		m_pOwnerBoss->UnBind_DamageEvent(this);
		Safe_Release(m_pOwnerBoss);
	}

	//이벤트 등록및 레퍼런스 증가
	m_pOwnerBoss = pBossPell;
	Safe_AddRef(m_pOwnerBoss);
	m_pPellInfo = &pBossPell->GetPellInfo();
	pBossPell->Bind_DamageCallBackEvent(this, [&](){ LerpTimeReset(); });
}

void CBossHealthBar::UnActive()
{
	m_eVisible = VISIBILITY::HIDDEN;
	Safe_Release(m_pOwnerBoss);
}

HRESULT CBossHealthBar::ADD_Components()
{
#pragma region Max Health Font
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	CFontComponent::FONT_DESC FontDesc = {};
	m_MaxHealthFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
	FontDesc.pPoint = &m_MaxHealthFontPos;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("MaxHealthFont_Com"), (CComponent**)&m_pMaxHealthFontCom, &FontDesc)))
		return E_FAIL;
#pragma endregion

#pragma region Health Font
	// 여기서 폰트를 생성해서 하자 폰트는 물론 컴포넌트로
	m_HealthFontPos = { (_float)GetRectSize().left, (_float)GetRectSize().top };
	FontDesc.pPoint = &m_HealthFontPos;
	FontDesc.szUseFontName = TEXT("HanSanFont_16");
	FontDesc.vFontSize = { 50, 100 };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_FontComponent"), TEXT("HealthFont_Com"), (CComponent**)&m_pHealthFontCom, &FontDesc)))
		return E_FAIL;
#pragma endregion

	CBossTitleName::GAMEOBJECT_DESC TitleNameDesc = {};
	TitleNameDesc.pParent = this;
	TitleNameDesc.vScale = {200.f, 50.f, 0.f};
	TitleNameDesc.vPosition = {0.f, -60.f, 0.f};

	m_pBossName = CBossTitleName::Create(m_pGraphic_Device, m_pDeviceContext);
	m_pBossName->SetZOrder(m_iZOrder + 1);
	if (FAILED(m_pBossName->Initialize(&TitleNameDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_GM_HeatlhBar_Tex"), TEXT("Texture_Com"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_ProgressBar"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CBossHealthBar* CBossHealthBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossHealthBar* pBossHealthBar = new CBossHealthBar(pDevice, pContext);
	if (FAILED(pBossHealthBar->Initalize_Prototype()))
	{
		Safe_Release(pBossHealthBar);
		MSG_BOX("CREATE FAIL : Boss Health Bar");
	}
	return pBossHealthBar;
}

CGameObject* CBossHealthBar::Clone(void* pArg)
{
	CBossHealthBar* pBossHealthBar = new CBossHealthBar(*this);
	if (FAILED(pBossHealthBar->Initialize(pArg)))
	{
		Safe_Release(pBossHealthBar);
		MSG_BOX("CLONE FAIL : Boss Health Bar");
	}
	return pBossHealthBar;
}

void CBossHealthBar::Free()
{
	__super::Free();

	Safe_Release(m_pOwnerBoss);
	Safe_Release(m_pMaxHealthFontCom);
	Safe_Release(m_pHealthFontCom);
	Safe_Release(m_pBossName);
}
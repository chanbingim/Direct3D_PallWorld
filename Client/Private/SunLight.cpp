#include "SunLight.h"

#include "GameInstance.h"
#include "Light.h"

_float3 CSunLight::vSunPositions[] = { {1, 0, 0}, { 0 , 1, 0 }, {-1, 0, 0}, {0, -1, 0} };

CSunLight::CSunLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CSunLight::CSunLight(const CSunLight& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSunLight::Initalize_Prototype()
{


    return S_OK;
}

HRESULT CSunLight::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    SUN_LIGHT_DESC* pSunLightDesc = static_cast<SUN_LIGHT_DESC*>(pArg);
    m_fRadius = pSunLightDesc->fRadius;
    m_pTransformCom->SetPosition({});

    DayLengthTimes[ENUM_CLASS(DAY_TYPE::MORNING)] = 35.f;
    DayLengthTimes[ENUM_CLASS(DAY_TYPE::LUNCH)] = 20.f;
    DayLengthTimes[ENUM_CLASS(DAY_TYPE::NIGHT)] = 35.f;
    DayLengthTimes[ENUM_CLASS(DAY_TYPE::DAWN)] = 15.f;
    return S_OK;
}

void CSunLight::Priority_Update(_float fDeletaTime)
{
    Compute_Time(fDeletaTime);

    _vector SrcDayPosition = XMLoadFloat3(&vSunPositions[ENUM_CLASS(m_eDayType)]) * m_fRadius;

    _uInt iNextIndex = ENUM_CLASS(m_eDayType) + 1;
    iNextIndex = iNextIndex % ENUM_CLASS(DAY_TYPE::END);
    _vector DestDayPosition = XMLoadFloat3(&vSunPositions[iNextIndex]) * m_fRadius;

    _vector vLerpPosition = XMVectorLerp(SrcDayPosition, DestDayPosition, m_fAccTime / DayLengthTimes[ENUM_CLASS(m_eDayType)]);
    XMStoreFloat3(&m_LightDesc[ENUM_CLASS(m_eDayType)].vPosition, vLerpPosition);

    _vector vCamPos = m_pGameInstance->GetCameraState(WORLDSTATE::POSITION);
    _vector vDir = XMVector3Normalize(vCamPos - vLerpPosition);
    XMStoreFloat4(&m_LightDesc[ENUM_CLASS(m_eDayType)].vDirection, vDir);

    m_pDireactionLight->SetLightInfo(m_LightDesc[ENUM_CLASS(m_eDayType)]);
}

void CSunLight::Update(_float fDeletaTime)
{
    
}

void CSunLight::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::PRIORITY, this);
}

HRESULT CSunLight::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(0);
    m_pVIBufferCom->Render();
    return S_OK;
}

const _float4 CSunLight::GetDireaction() const
{
    return m_LightDesc[ENUM_CLASS(m_eDayType)].vDirection;
}

void CSunLight::SetOnlyNight()
{
    m_bIsOnlyNight = true;
}

void CSunLight::ResetOnlyNight()
{
    m_bIsOnlyNight = false;
}

HRESULT CSunLight::Apply_ConstantShaderResources()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

    _float3 vCamPos = {};
    XMStoreFloat3(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float3));
    ID3D11ShaderResourceView* pTexSRV = nullptr;
    pTexSRV =  m_pTextureCom->GetTexture(0);
    if (pTexSRV)
        m_pShaderCom->Bind_SRV("g_Texture", pTexSRV);

    return S_OK;
}

HRESULT CSunLight::ADD_Components()
{
    // 돌  모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Point"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Sun_Tex"), TEXT("TextureCom"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_WorldUI"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

#pragma region MORNING_LIGHT
    m_LightDesc[ENUM_CLASS(DAY_TYPE::MORNING)].vDiffuse = { 1.00f, 0.85f, 0.60f, 1.f }; // 아침 햇살을 더 화사하고 따뜻하게
    m_LightDesc[ENUM_CLASS(DAY_TYPE::MORNING)].vAmbient = { 0.30f, 0.25f, 0.20f, 1.f }; // 주변광도 조금 더 밝게
    m_LightDesc[ENUM_CLASS(DAY_TYPE::MORNING)].vSpecular = { 1.00f, 0.80f, 0.60f, 1.f };
#pragma endregion

#pragma region LUNCH_LIGHT
    m_LightDesc[ENUM_CLASS(DAY_TYPE::LUNCH)].vDiffuse = { 1.00f, 1.00f, 0.90f, 1.f }; // 정오의 빛을 더 밝고 약간만 노랗게 유지
    m_LightDesc[ENUM_CLASS(DAY_TYPE::LUNCH)].vAmbient = { 0.30f, 0.30f, 0.28f, 1.f }; // 주변광을 높여 그림자 대비 완화 (밝은 판타지 분위기)
    m_LightDesc[ENUM_CLASS(DAY_TYPE::LUNCH)].vSpecular = { 1.00f, 0.98f, 0.90f, 1.f };
#pragma endregion

#pragma region DAWN_LIGHT
    m_LightDesc[ENUM_CLASS(DAY_TYPE::DAWN)].vDiffuse = { 0.85f, 0.60f, 0.40f, 1.f }; // 해질녘 붉은 톤을 더 생생하고 강렬하게
    m_LightDesc[ENUM_CLASS(DAY_TYPE::DAWN)].vAmbient = { 0.20f, 0.15f, 0.15f, 1.f };
    m_LightDesc[ENUM_CLASS(DAY_TYPE::DAWN)].vSpecular = { 0.90f, 0.60f, 0.45f, 1.f };
#pragma endregion

#pragma region NIGHT_LIGHT
    m_LightDesc[ENUM_CLASS(DAY_TYPE::NIGHT)].vDiffuse = { 0.25f, 0.40f, 0.80f, 1.f }; // 신비로운 밤을 위해 푸른빛을 더 강조하고 어둡게
    m_LightDesc[ENUM_CLASS(DAY_TYPE::NIGHT)].vAmbient = { 0.08f, 0.12f, 0.30f, 1.f }; // 주변광을 짙은 파란색으로 (어두운 푸른 밤)
    m_LightDesc[ENUM_CLASS(DAY_TYPE::NIGHT)].vSpecular = { 0.30f, 0.45f, 0.80f, 1.f };
#pragma endregion

    m_eDayType = DAY_TYPE::MORNING;
    m_pDireactionLight = CLight::Create(m_LightDesc[ENUM_CLASS(DAY_TYPE::MORNING)]);

    if (nullptr == m_pDireactionLight)
        return E_FAIL;

    Safe_AddRef(m_pDireactionLight);
    m_pGameInstance->ADDLight(m_pDireactionLight);

    return S_OK;
}

void CSunLight::Compute_Time(_float fDeltaTime)
{
    if (m_bIsOnlyNight)
    {
        m_eDayType = DAY_TYPE::NIGHT;
        m_fAccTime = 0.f;
    }
    else
    {
        m_fAccTime += fDeltaTime;
        if (DayLengthTimes[ENUM_CLASS(m_eDayType)] <= m_fAccTime)
        {
            _uInt iIndex = ENUM_CLASS(m_eDayType) + 1;

            m_eDayType = DAY_TYPE(iIndex % ENUM_CLASS(DAY_TYPE::END));
            m_fAccTime = 0.f;
        }
    }
}

CSunLight* CSunLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSunLight* pSunLight = new CSunLight(pDevice, pContext);
    if (FAILED(pSunLight->Initalize_Prototype()))
    {
        Safe_Release(pSunLight);
        MSG_BOX("CREATE FAIL : SUN LIGHT");
    }
    return pSunLight;
}

CGameObject* CSunLight::Clone(void* pArg)
{
    CSunLight* pSunLight = new CSunLight(*this);
    if (FAILED(pSunLight->Initialize(pArg)))
    {
        Safe_Release(pSunLight);
        MSG_BOX("CLONE FAIL : SUN LIGHT");
    }
    return pSunLight;
}

void CSunLight::Free()
{
    __super::Free();

    Safe_Release(m_pDireactionLight);

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
}

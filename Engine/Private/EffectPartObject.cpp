#include "EffectPartObject.h"

#include "GameInstance.h"
#include "StringHelper.h"

CEffectPartObject::CEffectPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CEffectPartObject::CEffectPartObject(const CEffectPartObject& rhs) :
    CGameObject(rhs),
    m_EffectData(rhs.m_EffectData)
{

}

HRESULT CEffectPartObject::Initalize_Prototype()
{
    ZeroMemory(&m_EffectData, sizeof(EFFECT_NETWORK_DESC));
    return S_OK;
}

HRESULT CEffectPartObject::Initalize_Prototype(const char* pFilePath)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    if (FAILED(ReadFileData(pFilePath)))
        return E_FAIL;

    return S_OK;
}

HRESULT CEffectPartObject::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;
   
    SetPartEffectData(m_EffectData);
    return S_OK;
}

void CEffectPartObject::Priority_Update(_float fDeletaTime)
{

}

void CEffectPartObject::Update(_float fDeletaTime)
{
    m_fLifeAccTime += fDeletaTime * m_EffectData.fRateTime;
    m_fDistotionAccTime.x += fDeletaTime * m_EffectData.fUvRateTime.x;
    m_fDistotionAccTime.y += fDeletaTime * m_EffectData.fUvRateTime.y;

    if (m_fLifeAccTime >= m_EffectData.fLifeTime)
    {
        if (m_EffectData.bIsLoop)
            m_fLifeAccTime = 0;
        else
            m_fLifeAccTime = m_EffectData.fLifeTime;
    }

    if (fabsf(m_fDistotionAccTime.x) >= m_EffectData.fLifeTime)
    {
        if (m_EffectData.bIsLoop)
            m_fDistotionAccTime.x = 0.f;
        else
            m_fDistotionAccTime.x = 1.f;
    }

    if (fabsf(m_fDistotionAccTime.y) >= m_EffectData.fLifeTime)
    {
        if (m_EffectData.bIsLoop)
            m_fDistotionAccTime.y = 0.f;
        else
            m_fDistotionAccTime.y = 1.f;
    }

    if (m_EffectData.bIsLerp)
    {
        _float fRatio = m_fLifeAccTime / m_EffectData.fLifeTime;
        _vector vStartPos = XMLoadFloat3(&m_EffectData.vPosition);
        _vector vEndPos = XMLoadFloat3(&m_EffectData.vEndPosition);

        _vector vStartScale = XMLoadFloat3(&m_EffectData.vScale);
        _vector vEndtScale = XMLoadFloat3(&m_EffectData.vEndScale);

        _float3 vPosition{}, vScale{};
        XMStoreFloat3(&vPosition, XMVectorLerp(vStartPos, vEndPos, fRatio));
        XMStoreFloat3(&vScale, XMVectorLerp(vStartScale, vEndtScale, fRatio));

        m_pTransformCom->SetPosition(vPosition);
        m_pTransformCom->SetScale(vScale);
    }
}

void CEffectPartObject::Late_Update(_float fDeletaTime)
{
    UpdateCombinedMatrix();
    
    if(m_pShaderCom && m_pVIBufferCom)
        m_pGameInstance->Add_RenderGroup(RENDER::NONLIGHT, this);
}

HRESULT CEffectPartObject::Render()
{
    _uInt ShaderIndex = {};
    if (lstrcmp(TEXT("None"), m_EffectData.DistotionTexturePath))
        ShaderIndex += 1;

    if (EFFECT_TYPE::MESH == m_EffectData.eType)
    {
        auto pModel = static_cast<CModel*>(m_pVIBufferCom);
        _uInt iNumMeshes = pModel->GetNumMeshes();
        for (_uInt i = 0; i < iNumMeshes; ++i)
        {
            Apply_ConstantShaderResources(i);
            m_pShaderCom->Update_Shader(ShaderIndex);
            pModel->Render(i);
        }
    }
    else if(EFFECT_TYPE::SPRITE == m_EffectData.eType)
    {
        auto pVIBuffer = static_cast<CVIBuffer*>(m_pVIBufferCom);
        Apply_ConstantShaderResources();
        m_pShaderCom->Update_Shader(ShaderIndex);
        pVIBuffer->Render_VIBuffer();
    }

    return S_OK;
}

void CEffectPartObject::ResetEffect()
{
    m_fLifeAccTime = 0.f;
    m_fDistotionAccTime = { 0.f, 0.f };
}

#ifdef _DEBUG
void CEffectPartObject::SetPartEffectData(EFFECT_NETWORK_DESC& Data)
{
    m_EffectData = Data;
    m_pTransformCom->SetPosition(m_EffectData.vPosition);
    m_pTransformCom->SetRotation(m_EffectData.vRotation);
    m_pTransformCom->SetScale(m_EffectData.vScale);

    if (m_pShaderCom)
        Safe_Release(m_pShaderCom);

    if (m_pVIBufferCom)
        Safe_Release(m_pVIBufferCom);

    for (_uInt i = 0; i < ENUM_CLASS(EFFECT_TEXTURE_TYPE::END); ++i)
        Safe_Release(m_pTextures[i]);

    m_pVIBufferCom = m_pGameInstance->GetModelResource(m_EffectData.ModelTag);
    m_pShaderCom = m_pGameInstance->GetShaderResource(m_EffectData.ShaderTag);
    
    m_pTextures[0] = m_pGameInstance->GetTextureResource(m_EffectData.DiffuseTexturePath);
    m_pTextures[1] = m_pGameInstance->GetTextureResource(m_EffectData.NormalTexturePath);
    m_pTextures[2] = m_pGameInstance->GetTextureResource(m_EffectData.DistotionTexturePath);
    m_pTextures[3] = m_pGameInstance->GetTextureResource(m_EffectData.MaskTexturePath);
}

void CEffectPartObject::ExportData(void* pArg)
{
    EFFECT_PARTOBJECT_DESC* SaveDesc = static_cast<EFFECT_PARTOBJECT_DESC*>(pArg);
    ios_base::openmode flag;
    flag = ios::out | ios::trunc | ios::binary;

    char    ConvertPath[MAX_PATH];
    ofstream file(SaveDesc->szFilePath, flag);
    if (file.is_open())
    {
        CStringHelper::ConvertWideToUTF(m_ObejctTag.c_str(), ConvertPath);
        file << ConvertPath << endl;

        _float3 vPosition = m_pTransformCom->GetPosition();
        file << vPosition.x << " " << vPosition.y << " " << vPosition.z << " " << endl;

        _float3 vRotation = m_pTransformCom->GetRotation();
        file << vRotation.x << " " << vRotation.y << " " << vRotation.z << " " << endl;

        file << m_EffectData.bIsLerp << endl;
        file << m_EffectData.vEndPosition.x << " " << m_EffectData.vEndPosition.y << " " << m_EffectData.vEndPosition.z << " " << endl;
        file << m_EffectData.vEndRotation.x << " " << m_EffectData.vEndRotation.y << " " << m_EffectData.vEndRotation.z << " " << endl;
        file << m_EffectData.vEndScale.x << " " << m_EffectData.vEndScale.y << " " << m_EffectData.vEndScale.z << " " << endl;

        _float3 vScale = m_pTransformCom->GetScale();
        file << vScale.x << " " << vScale.y << " " << vScale.z << " " << endl;

        file << ENUM_CLASS(m_EffectData.eType) << endl;
        file << ENUM_CLASS(m_EffectData.eBlend_Mode) << endl;
        file << m_EffectData.fLifeTime << endl;
        file << m_EffectData.fRateTime << endl;

        CStringHelper::ConvertWideToUTF(m_EffectData.DiffuseTexturePath, ConvertPath);
        file << ConvertPath << endl;

        CStringHelper::ConvertWideToUTF(m_EffectData.NormalTexturePath, ConvertPath);
        file << ConvertPath << endl;

        file << m_EffectData.fUvRateTime.x << " " << m_EffectData.fUvRateTime.y << endl;
        file << m_EffectData.fNoiseStength << endl;
        file << ENUM_CLASS(m_EffectData.eDistotionType) << endl;
        CStringHelper::ConvertWideToUTF(m_EffectData.DistotionTexturePath, ConvertPath);
        file << ConvertPath << endl;

        file << m_EffectData.bIsReverse << endl;
        file << ENUM_CLASS(m_EffectData.eMaskType) << endl;
        CStringHelper::ConvertWideToUTF(m_EffectData.MaskTexturePath, ConvertPath);
        file << ConvertPath << endl;

        CStringHelper::ConvertWideToUTF(m_EffectData.ShaderTag, ConvertPath);
        file << ConvertPath << endl;

        CStringHelper::ConvertWideToUTF(m_EffectData.ModelTag, ConvertPath);
        file << ConvertPath << endl;

        file << m_EffectData.vColor.x << " " << m_EffectData.vColor.y << " " << m_EffectData.vColor.z << " " << m_EffectData.vColor.w << endl;
        file << m_EffectData.bIsLoop << endl;
    }

    file.close();
}
#endif // DEBUG


HRESULT CEffectPartObject::Apply_ConstantShaderResources()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix",  &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::DIFFUSE)])
        m_pShaderCom->Bind_SRV("g_DiffuseTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::DIFFUSE)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NORMAL)])
        m_pShaderCom->Bind_SRV("g_NormalTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NORMAL)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NOISE)])
        m_pShaderCom->Bind_SRV("g_NoiseTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NOISE)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::MASK)])
        m_pShaderCom->Bind_SRV("g_MaskTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::MASK)]->GetTexture(0));

    m_pShaderCom->Bind_RawValue("g_bReverse", &m_EffectData.bIsReverse, sizeof(_bool));
    m_pShaderCom->Bind_RawValue("g_vColor", &m_EffectData.vColor, sizeof(_float4));
    m_pShaderCom->Bind_RawValue("g_fLifeTime", &m_EffectData.fLifeTime, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_DistionType", &m_EffectData.eDistotionType, sizeof(_uInt));
    m_pShaderCom->Bind_RawValue("g_MaskType", &m_EffectData.eMaskType, sizeof(_uInt));
    m_pShaderCom->Bind_RawValue("g_fLifeAccTime", &m_fLifeAccTime, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_fNoiseStLength", &m_EffectData.fNoiseStength, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_fDistotionAccTime", &m_fDistotionAccTime, sizeof(_float2));
    return S_OK;
}

HRESULT CEffectPartObject::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::DIFFUSE)])
        m_pShaderCom->Bind_SRV("g_DiffuseTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::DIFFUSE)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NORMAL)])
        m_pShaderCom->Bind_SRV("g_NormalTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NORMAL)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NOISE)])
        m_pShaderCom->Bind_SRV("g_NoiseTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::NOISE)]->GetTexture(0));

    if (nullptr != m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::MASK)])
        m_pShaderCom->Bind_SRV("g_MaskTexture", m_pTextures[ENUM_CLASS(EFFECT_TEXTURE_TYPE::MASK)]->GetTexture(0));

    m_pShaderCom->Bind_RawValue("g_bReverse", &m_EffectData.bIsReverse, sizeof(_bool));
    m_pShaderCom->Bind_RawValue("g_fLifeTime", &m_EffectData.fLifeTime, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_vColor", &m_EffectData.vColor, sizeof(_float4));
    m_pShaderCom->Bind_RawValue("g_DistionType", &m_EffectData.eDistotionType, sizeof(_uInt));
    m_pShaderCom->Bind_RawValue("g_MaskType", &m_EffectData.eMaskType, sizeof(_uInt));
    m_pShaderCom->Bind_RawValue("g_fLifeAccTime", &m_fLifeAccTime, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_fNoiseStLength", &m_EffectData.fNoiseStength, sizeof(_float));
    m_pShaderCom->Bind_RawValue("g_fDistotionAccTime", &m_fDistotionAccTime, sizeof(_float2));
    return S_OK;
}

void CEffectPartObject::UpdateCombinedMatrix()
{
    if (m_pParent)
    {
        _matrix vParentMatrix = XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat());
        XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * vParentMatrix);
    }
    else
        XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

HRESULT CEffectPartObject::ReadFileData(const char* pFilePath)
{
    ios_base::openmode flag;
    flag = ios::in;

    char     ResoucePath[MAX_PATH] = {};
    ifstream file(pFilePath, flag);
    if (file.is_open())
    {
        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.EffectTag);
        m_ObejctTag = m_EffectData.EffectTag;

        file >> m_EffectData.vPosition.x >> m_EffectData.vPosition.y >> m_EffectData.vPosition.z;
        file >> m_EffectData.vRotation.x >> m_EffectData.vRotation.y >> m_EffectData.vRotation.z;
        file >> m_EffectData.vScale.x >> m_EffectData.vScale.y >> m_EffectData.vScale.z;

        file >> m_EffectData.bIsLerp;
        file >> m_EffectData.vEndPosition.x >> m_EffectData.vEndPosition.y >> m_EffectData.vEndPosition.z;
        file >> m_EffectData.vEndRotation.x >> m_EffectData.vEndRotation.y >> m_EffectData.vEndRotation.z;
        file >> m_EffectData.vEndScale.x >> m_EffectData.vEndScale.y >> m_EffectData.vEndScale.z;

        _uInt iType = {};
        file >> iType;
        m_EffectData.eType = EFFECT_TYPE(iType);

        file >> iType;
        m_EffectData.eBlend_Mode = EFFECT_BELND_MODE(iType);
        file >> m_EffectData.fLifeTime;
        file >> m_EffectData.fRateTime;

        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.DiffuseTexturePath);
       
        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.NormalTexturePath);

        file >> m_EffectData.fUvRateTime.x >> m_EffectData.fUvRateTime.y;
        file >> m_EffectData.fNoiseStength;
        file >> iType;
        m_EffectData.eDistotionType = EFFECT_DISTOTION_TYPE(iType);

        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.DistotionTexturePath);

        file >> m_EffectData.bIsReverse;
        file >> iType;
        m_EffectData.eMaskType = EFFECT_MASK_TYPE(iType);

        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.MaskTexturePath);

        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.ShaderTag);

        file >> ResoucePath;
        CStringHelper::ConvertUTFToWide(ResoucePath, m_EffectData.ModelTag);

        file >> m_EffectData.vColor.x >> m_EffectData.vColor.y >> m_EffectData.vColor.z >> m_EffectData.vColor.w;
        file >> m_EffectData.bIsLoop;
    }
    else
        return E_FAIL;

    file.close();
    return S_OK;
}

CEffectPartObject* CEffectPartObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffectPartObject* pEffectParObject = new CEffectPartObject(pDevice, pContext);
    if (FAILED(pEffectParObject->Initalize_Prototype()))
    {
        Safe_Release(pEffectParObject);
        MSG_BOX("CREATE FAIL : EFFECT PARTOBJECT");
    }
    return pEffectParObject;
}

CEffectPartObject* CEffectPartObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pFilePath)
{
    CEffectPartObject* pEffectParObject = new CEffectPartObject(pDevice, pContext);
    if (FAILED(pEffectParObject->Initalize_Prototype(pFilePath)))
    {
        Safe_Release(pEffectParObject);
        MSG_BOX("CREATE FAIL : EFFECT PARTOBJECT");
    }
    return pEffectParObject;
}

CGameObject* CEffectPartObject::Clone(void* pArg)
{
    CEffectPartObject* pEffectParObject = new CEffectPartObject(*this);
    if (FAILED(pEffectParObject->Initialize(pArg)))
    {
        Safe_Release(pEffectParObject);
        MSG_BOX("CLONE FAIL : EFFECT PARTOBJECT");
    }
    return pEffectParObject;
}

void CEffectPartObject::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShaderCom);

    for (auto& pTexture : m_pTextures)
        Safe_Release(pTexture);
}

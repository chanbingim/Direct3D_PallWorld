#include "TrailEffect.h"

#include "GameInstance.h"

CTrailEffect::CTrailEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CSpriteEffect(pDevice, pContext)
{
}

CTrailEffect::CTrailEffect(const CTrailEffect& rhs) :
    CSpriteEffect(rhs)
{
}

HRESULT CTrailEffect::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CTrailEffect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(nullptr)))
        return E_FAIL;

    TRAIL_EFFECT_DESC* Desc = static_cast<TRAIL_EFFECT_DESC*>(pArg);
    m_pSocketMatrix = Desc->pSocketMatrix;
    m_vTrailDispatch = Desc->TrailDisPatch;
    m_szTrailTexTag = Desc->szTrailEffectName;
    if (FAILED(ADD_Components()))
        return E_FAIL;

    m_iNumData = m_pInstanceBuffer->GetNumInstance();
    if (FAILED(Setting_Compute_Shader()))
        return E_FAIL;

    m_CBData.fLifeTime = 1.0f;
    m_CBData.fSize = 0.2f;
    return S_OK;
}

void CTrailEffect::Priority_Update(_float fDeletaTime)
{
    __super::Priority_Update(fDeletaTime);
}

void CTrailEffect::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
   
}

void CTrailEffect::Late_Update(_float fDeletaTime)
{
    __super::Late_Update(fDeletaTime);
    if (m_bIsTrailRender)
    {
        m_CBData.iHeadIndex = (m_CBData.iHeadIndex + 1) % m_iNumData;
        m_CBData.fDeletaTime = fDeletaTime * 3.f;
        m_CBData.iMaxIndex = m_iNumData;
        if (m_pSocketMatrix)
            m_CBData.SocketMatrix = *m_pSocketMatrix;

        D3D11_MAPPED_SUBRESOURCE InstanceData = {};
        m_pInstanceBuffer->Lock(&InstanceData);
        if (InstanceData.pData)
        {
            m_pDeviceContext->UpdateSubresource(m_pResourceBuffer[0], 0, nullptr, InstanceData.pData, 0, 0);
            if (S_OK == Apply_ComputeShaderResource())
            {
                m_pComputeShader->Update_Shader(m_vTrailDispatch);
                m_pComputeShader->Bind_ShaderResource(0, nullptr);
                m_pComputeShader->Bind_UAVResource(0, nullptr);
                m_pComputeShader->Bind_Buffer(0, nullptr);
            }

            m_pDeviceContext->CopyResource(m_pReadBuffer, m_pResourceBuffer[1]);
            D3D11_MAPPED_SUBRESOURCE mapped = {};
            m_pDeviceContext->Map(m_pReadBuffer, 0, D3D11_MAP_READ, 0, &mapped);
            memcpy(InstanceData.pData, mapped.pData, sizeof(VTX_INSTANCE_DEFAULT_DESC) * m_iNumData);
            m_pDeviceContext->Unmap(m_pReadBuffer, 0);
        }

        m_pInstanceBuffer->UnLock();
    }

    ComputeMatrix();

    m_pGameInstance->Add_RenderGroup(RENDER::NONLIGHT, this);
}

HRESULT CTrailEffect::Render()
{
    Apply_ConstantShaderResources();

    m_pShaderCom->Update_Shader(0);
    m_pInstanceBuffer->Render_VIBuffer();
    return S_OK;
}

HRESULT CTrailEffect::Apply_ConstantShaderResources()
{
    m_pShaderCom->Bind_Matrix("g_WorldMatrix" , &m_pTransformCom->GetWorldMat());
    m_pShaderCom->Bind_Matrix("g_ViewMatrix" , &m_pGameInstance->GetMatrix(MAT_STATE::VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix" , &m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION));

    ID3D11ShaderResourceView* pResourceVeiw;
    if(m_pTextureCom)
        m_pShaderCom->Bind_SRV("g_DiffuseTexture", m_pTextureCom->GetTexture(0));

    _float4 vCamPos = {};
    XMStoreFloat4(&vCamPos, m_pGameInstance->GetCameraState(WORLDSTATE::POSITION));
    m_pShaderCom->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));
    m_pShaderCom->Bind_RawValue("g_vMaxLifeTime", &m_CBData.fLifeTime, sizeof(_float));
    return S_OK;
}

void CTrailEffect::ComputeMatrix()
{
    if (m_pSocketMatrix)
    {
        _matrix     SocketMat = XMLoadFloat4x4(m_pSocketMatrix);
        for (_uInt i = 0; i < 3; ++i)
            SocketMat.r[i] = XMVector3Normalize(SocketMat.r[i]);

        XMStoreFloat4x4(&m_CombinedMatrix, XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * SocketMat);
    }
    else
        XMStoreFloat4x4(&m_CombinedMatrix, XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));
}

HRESULT CTrailEffect::Apply_ComputeShaderResource()
{

    if (FAILED(m_pComputeShader->Bind_ShaderResource(0, m_pTrailInputSRV)))
        return E_FAIL;

    if (FAILED(m_pComputeShader->Bind_UAVResource(0, m_pTrailOutUAV)))
        return E_FAIL;

    m_pDeviceContext->UpdateSubresource(m_pConstTrailBuffer, 0, nullptr, &m_CBData, 0, 0);
    if (FAILED(m_pComputeShader->Bind_Buffer(0, m_pConstTrailBuffer)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTrailEffect::Setting_Compute_Shader()
{
    D3D11_MAPPED_SUBRESOURCE InstanceData = {};
    m_pInstanceBuffer->Lock(&InstanceData);

#pragma region Const Buffer
    D3D11_BUFFER_DESC BufferDesc = {};
    BufferDesc.ByteWidth = sizeof(TrailConstBufferData);
    BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    D3D11_SUBRESOURCE_DATA ConstBufferSubResource = {};
    ConstBufferSubResource.pSysMem = &m_CBData;

    if (FAILED(m_pGraphic_Device->CreateBuffer(&BufferDesc, &ConstBufferSubResource, &m_pConstTrailBuffer)))
        return E_FAIL;
#pragma endregion

#pragma region Init Buffer
    D3D11_BUFFER_DESC TrialInitBufferDesc = {};
    TrialInitBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    TrialInitBufferDesc.ByteWidth = sizeof(VTX_TRAIL_INSTANCE_DEFAULT_DESC) * m_iNumData;
    TrialInitBufferDesc.StructureByteStride = sizeof(VTX_TRAIL_INSTANCE_DEFAULT_DESC);
    TrialInitBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    TrialInitBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    D3D11_SUBRESOURCE_DATA SubResource = {};
    SubResource.pSysMem = InstanceData.pData;
    if (FAILED(m_pGraphic_Device->CreateBuffer(&TrialInitBufferDesc, &SubResource, &m_pResourceBuffer[0])))
        return E_FAIL;

    if (FAILED(m_pGraphic_Device->CreateBuffer(&TrialInitBufferDesc, &SubResource, &m_pResourceBuffer[1])))
        return E_FAIL;
#pragma endregion

#pragma region ReadBuffer
    D3D11_BUFFER_DESC ReadBufferDesc = {};
    ReadBufferDesc.Usage = D3D11_USAGE_STAGING;
    ReadBufferDesc.ByteWidth = sizeof(VTX_TRAIL_INSTANCE_DEFAULT_DESC) * m_iNumData;
    ReadBufferDesc.StructureByteStride = sizeof(VTX_INSTANCE_DEFAULT_DESC);
    ReadBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;

    if (FAILED(m_pGraphic_Device->CreateBuffer(&ReadBufferDesc, nullptr, &m_pReadBuffer)))
        return E_FAIL;
#pragma endregion

#pragma region SRV Buffer
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
    SRVDesc.Buffer.NumElements = m_iNumData;

    if (FAILED(m_pGraphic_Device->CreateShaderResourceView(m_pResourceBuffer[0], &SRVDesc, &m_pTrailInputSRV)))
        return E_FAIL;
#pragma endregion

#pragma region UAV Buffer
    D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
    UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
    UAVDesc.Buffer.NumElements = m_iNumData;

    if (FAILED(m_pGraphic_Device->CreateUnorderedAccessView(m_pResourceBuffer[1], &UAVDesc, &m_pTrailOutUAV)))
        return E_FAIL;
#pragma endregion

    m_pInstanceBuffer->UnLock();

    return S_OK;
}

HRESULT CTrailEffect::ADD_Components()
{
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Instance_Point"), TEXT("VIBuffer_Com"), (CComponent**)&m_pInstanceBuffer)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail_Instance_Point"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_ComputeShader_Trail"), TEXT("ComputeShader_Com"), (CComponent**)&m_pComputeShader)))
        return E_FAIL;

    // 여기서 텍스처도 가져오자.
    m_pTextureCom = m_pGameInstance->GetTextureResource(m_szTrailTexTag);
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    return S_OK;
}

CTrailEffect* CTrailEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTrailEffect* pTrailEffect = new CTrailEffect(pDevice, pContext);
    if (FAILED(pTrailEffect->Initalize_Prototype()))
    {
        Safe_Release(pTrailEffect);
        MSG_BOX("CREATE FAIL : TRAIL EFFECT");
    }
    return pTrailEffect;
}

CGameObject* CTrailEffect::Clone(void* pArg)
{
    CTrailEffect* pTrailEffect = new CTrailEffect(*this);
    if (FAILED(pTrailEffect->Initialize(pArg)))
    {
        Safe_Release(pTrailEffect);
        MSG_BOX("CLONE FAIL : TRAIL EFFECT");
    }
    return pTrailEffect;
}

void CTrailEffect::Free()
{
    __super::Free();

    for (_uInt i = 0; i < 2; ++i)
        Safe_Release(m_pResourceBuffer[i]);

    Safe_Release(m_pReadBuffer);
    Safe_Release(m_pTrailInputSRV);
    Safe_Release(m_pTrailOutUAV);
    Safe_Release(m_pConstTrailBuffer);

    Safe_Release(m_pComputeShader);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pInstanceBuffer);
    Safe_Release(m_pShaderCom);
}

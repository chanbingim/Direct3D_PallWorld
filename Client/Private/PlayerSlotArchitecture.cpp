#include "PlayerSlotArchitecture.h"

#include "GameInstance.h"
#include "TerrainManager.h"
#include "Architecture.h"

CPlayerSlotArchitecture::CPlayerSlotArchitecture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CPartObject(pDevice, pContext)
{
}

CPlayerSlotArchitecture::CPlayerSlotArchitecture(const CPlayerSlotArchitecture& rhs) :
    CPartObject(rhs)
{
}

HRESULT CPlayerSlotArchitecture::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CPlayerSlotArchitecture::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    PLAYTER_SLOT_ARCHITECTURE_DESC* pDesc = static_cast<PLAYTER_SLOT_ARCHITECTURE_DESC*>(pArg);
    if (FAILED(ADD_Components(pDesc)))
        return E_FAIL;

    if(FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

void CPlayerSlotArchitecture::Priority_Update(_float fDeletaTime)
{
    
}

void CPlayerSlotArchitecture::Update(_float fDeletaTime)
{
    _matrix ParentMat = XMLoadFloat4x4(&m_pParent->GetTransform()->GetWorldMat());

    ParentMat.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f);
    ParentMat.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f);
    ParentMat.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()) * ParentMat);
    _float3 vPosition = *reinterpret_cast<_float3*>(&m_CombinedWorldMatrix.m[3]);

    if (m_pNavigation->IsMove(XMLoadFloat3(&vPosition)))
    {
        // 이동가능 설치가능
        m_bIsCreateAble = true;
    }
    else
    {
        // 설치 불가능
        m_bIsCreateAble = false;
    }

}

void CPlayerSlotArchitecture::Late_Update(_float fDeletaTime)
{
    if (m_pItemModel)
    {
        m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
    }
}

HRESULT CPlayerSlotArchitecture::Render()
{
    _uInt iNumMeshes = m_pItemModel->GetNumMeshes();

    _float4 vColor = {};
    if (m_bIsCreateAble)
        vColor = { 0.f, 0.f, 1.f, .1f };
    else
        vColor = { 1.f, 0.f, 0.f, .1f };

    for (_uInt i = 0; i < iNumMeshes; ++i)
    {
        Apply_ConstantShaderResources(i);
        m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

        m_pShaderCom->Update_Shader(1);
        m_pItemModel->Render(i);
    }

    return S_OK;
}

void CPlayerSlotArchitecture::SetItemDesc(const ITEM_DESC* pDesc)
{
    if (nullptr == pDesc)
    {
        Safe_Release(m_pItemModel);
        m_pItemDesc = nullptr;
    }
    else
    {
        m_pItemDesc = pDesc;
        m_pItemModel = static_cast<CModel *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::COMPONENT, ENUM_CLASS(LEVEL::GAMEPLAY), pDesc->szItemModelPath, nullptr));
    }
}

_bool CPlayerSlotArchitecture::IsPreView()
{
    if (m_pItemDesc)
        return true;
    return false;
}

void CPlayerSlotArchitecture::CreateSlotObject()
{
    // 여기서 구조체값을 통한 객체 생성
    // 셀정보도 여기서 넘겨주기
    _uInt iCurLevel = ENUM_CLASS(LEVEL::GAMEPLAY);
    CArchitecture::GAMEOBJECT_DESC ArchitectureDesc = {};
    ArchitectureDesc.vScale = { 1.f,1.f,1.f };

    ArchitectureDesc.vPosition = *reinterpret_cast<_float3*>(&m_CombinedWorldMatrix.m[3]);
    m_pGameInstance->Add_GameObject_ToLayer(iCurLevel, TEXT("Layer_GamePlay_WorkAbleObject"),
        static_cast<CGameObject *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, iCurLevel, m_pItemDesc->TypeDesc.ArchitectureDesc.ArchitecturePrototpyeName, &ArchitectureDesc)));
}

HRESULT CPlayerSlotArchitecture::Bind_ShaderResources()
{
    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CPlayerSlotArchitecture::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_CombinedWorldMatrix));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};

    m_pShaderCom->Bind_RawValue("g_vPreviewColor", &m_vPreViewColor, sizeof(_float4));
    m_pItemModel->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    return S_OK;
}

HRESULT CPlayerSlotArchitecture::ADD_Components(PLAYTER_SLOT_ARCHITECTURE_DESC* pDesc)
{
#pragma region NAVI_MESH
    auto pTerrainManager = CTerrainManager::GetInstance();
    auto FindNaviMesh = pTerrainManager->GetNavimesh();
    if (FindNaviMesh)
    {
        CNavigation::NAVIGATION_DESC Desc = {};

        _float3 vParentPos = m_pParent->GetTransform()->GetPosition();
        _vector vCalCulationParentPos = XMLoadFloat3(&vParentPos);
        _vector vCalCulationParentLook = m_pParent->GetTransform()->GetLookVector();

        Desc.iCurrentCellIndex = pDesc->iParentCellIndex;
        m_pNavigation = static_cast<CNavigation*>(FindNaviMesh->Clone(&Desc));

        Safe_AddRef(m_pNavigation);
        m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNavigation);
    }
#pragma endregion

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    return S_OK;
}

CPlayerSlotArchitecture* CPlayerSlotArchitecture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayerSlotArchitecture* pPlayerSlotArchitecutre = new CPlayerSlotArchitecture(pDevice, pContext);
    if (FAILED(pPlayerSlotArchitecutre->Initalize_Prototype()))
    {
        Safe_Release(pPlayerSlotArchitecutre);
        MSG_BOX("CREATE FAIL : PLAYER SLOT ARCHITECTURE");
    }
    return pPlayerSlotArchitecutre;
}

CGameObject* CPlayerSlotArchitecture::Clone(void* pArg)
{
    CPlayerSlotArchitecture* pPlayerSlotArchitecutre = new CPlayerSlotArchitecture(*this);
    if (FAILED(pPlayerSlotArchitecutre->Initialize(pArg)))
    {
        Safe_Release(pPlayerSlotArchitecutre);
        MSG_BOX("CLONE FAIL : PLAYER SLOT ARCHITECTURE");
    }
    return pPlayerSlotArchitecutre;
}

void CPlayerSlotArchitecture::Free()
{
    __super::Free();

    Safe_Release(m_pNavigation);
    Safe_Release(m_pItemModel);
}

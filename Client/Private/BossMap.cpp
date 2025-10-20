#ifdef _DEBUG
#include "ImgManager.h"
#include "IMG_LandScape.h"
#endif // _DEBUG

#include "BossMap.h"

#include "GameInstance.h"
CBossMap::CBossMap(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext) :
    CNoneAnimMesh(pGraphic_Device, pDeviceContext)
{
}

CBossMap::CBossMap(const CBossMap& rhs) :
    CNoneAnimMesh(rhs)
{
}

HRESULT CBossMap::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CBossMap::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
        m_pNavigationCom->Update(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));


    m_ObejctTag = TEXT("BossMap");
    return S_OK;
}

void CBossMap::Priority_Update(_float fDeletaTime)
{
}

void CBossMap::Update(_float fDeletaTime)
{
#ifdef _DEBUG
    auto EditorUI = CImgManager::GetInstance()->Find_ImgUserInterface(TEXT("LandScape"));
    CIMG_LandScape* Img_LandScape = dynamic_cast<CIMG_LandScape*>(EditorUI);

    if (Img_LandScape->GetbIsNaviMeshPicking())
        PickingNavimesh();
    else
        PickingPoint();

#endif // _DEBUG
}

void CBossMap::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CBossMap::Render()
{
    _uInt iNum = m_pVIBufferCom->GetNumMeshes();
    for (_uInt i = 0; i < iNum; ++i)
    {
        Apply_ConstantShaderResources(i);

        m_pShaderCom->Update_Shader(0);
        m_pVIBufferCom->Render(i);
    }
    m_pNavigationCom->Render({ 1.f, 0.f,0.f,1.f });

    return S_OK;
}

void CBossMap::GetAllNaviMeshTriangle(list<NAVI_TRIANGLE>* pOut)
{
    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
        pOut->insert(pOut->end(), m_pNavigationCom->GetNaviMeshTriangleList().begin(),
            m_pNavigationCom->GetNaviMeshTriangleList().end());
}

HRESULT CBossMap::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CBossMap::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    ID3D11ShaderResourceView* pNormalResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_NORMALS, 0, &pNormalResourceVeiw);
    if (pNormalResourceVeiw)
        m_pShaderCom->Bind_SRV("g_NormalTexture", pNormalResourceVeiw);

    return S_OK;
}

HRESULT CBossMap::ADD_Components()
{
    // 돌  모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_BossMap"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    m_iTerrainCnt = m_pVIBufferCom->GetNumMeshes();
    m_pNavigationCom = CNavigation::Create(m_pGraphic_Device, m_pDeviceContext, m_pVIBufferCom);
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNavigationCom);
    return S_OK;
}

void CBossMap::PickingNavimesh()
{
    _uInt iPickMeshIndex = {};
    _float3 vOut = {};
    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F11))
        m_bViewMesh = !m_bViewMesh;

    if (m_pGameInstance->KeyDown(KEY_INPUT::KEYBOARD, DIK_F5))
        m_iDrawTriCount = 0;

    if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 0))
    {
        if (m_pVIBufferCom->IsPicking(m_pTransformCom, &vOut, &iPickMeshIndex))
        {
            //에디터용 코드
            auto EditorUI = CImgManager::GetInstance()->Find_ImgUserInterface(TEXT("LandScape"));
            CIMG_LandScape* Img_LandScape = dynamic_cast<CIMG_LandScape*>(EditorUI);
            if (Img_LandScape)
            {
                switch (Img_LandScape->GetBurshMode())
                {
                case CIMG_LandScape::BRUSH_MODE::SELECT_TERRIAN:
                    break;
                case CIMG_LandScape::BRUSH_MODE::EDIT_NAVIMESH:
                    break;
                case CIMG_LandScape::BRUSH_MODE::EDIT_ENVIORNMENT:

                    break;
                }
            }
        }
    }
    if (m_pGameInstance->KeyDown(KEY_INPUT::MOUSE, 1))
    {
        if (m_pVIBufferCom->IsPicking(m_pTransformCom, &vOut, &iPickMeshIndex))
        {
            //에디터용 코드
            auto EditorUI = CImgManager::GetInstance()->Find_ImgUserInterface(TEXT("LandScape"));
            CIMG_LandScape* Img_LandScape = dynamic_cast<CIMG_LandScape*>(EditorUI);
            if (Img_LandScape)
            {
                _float3 vPosition = {};
                switch (Img_LandScape->GetBurshMode())
                {
                case CIMG_LandScape::BRUSH_MODE::EDIT_NAVIMESH:
                {
                    switch (m_iDrawTriCount)
                    {
                    case 0:
                        vPosition = m_pNavigationCom->DrawTriangle(XMLoadFloat3(&vOut), Img_LandScape->GetBurshSize());
                        break;
                    case 1:
                        vPosition = m_pNavigationCom->DrawTriangle(XMLoadFloat3(&vOut), Img_LandScape->GetBurshSize());
                        break;
                    case 2:
                        vPosition = m_pNavigationCom->DrawTriangle(XMLoadFloat3(&vOut), Img_LandScape->GetBurshSize());
                        break;
                    }

                    if (m_iDrawTriCount < 3)
                    {
                        if (vPosition.x == -1)
                            m_CrateTriangle[m_iDrawTriCount] = vOut;
                        else
                            m_CrateTriangle[m_iDrawTriCount] = vPosition;
                        m_iDrawTriCount++;
                    }

                }
                break;
                }
            }

            if (2 < m_iDrawTriCount)
            {
                auto new_Triangle = NAVI_TRIANGLE(m_CrateTriangle[0], m_CrateTriangle[1], m_CrateTriangle[2]);
                m_pNavigationCom->InsertTriangle(new_Triangle);
                ZeroMemory(&m_CrateTriangle, sizeof(_float3) * 3);
                m_iDrawTriCount = 0;
            }

        }
    }
    if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0))
    {
        if (m_pVIBufferCom->IsPicking(m_pTransformCom, &vOut, &iPickMeshIndex))
        {
            //에디터용 코드
            auto EditorUI = CImgManager::GetInstance()->Find_ImgUserInterface(TEXT("LandScape"));
            CIMG_LandScape* Img_LandScape = dynamic_cast<CIMG_LandScape*>(EditorUI);
            if (Img_LandScape)
            {
                switch (Img_LandScape->GetBurshMode())
                {
                case CIMG_LandScape::BRUSH_MODE::EDIT_NAVIMESH:
                    m_pNavigationCom->RemoveCell(XMLoadFloat3(&vOut), Img_LandScape->GetBurshSize());
                    break;
                }
            }
        }
    }
}

void CBossMap::PickingPoint()
{
    _uInt iPickMeshIndex = {};
    _float3 vOut = {};
    if (m_pGameInstance->KeyPressed(KEY_INPUT::MOUSE, 0))
    {
        if (m_pVIBufferCom->IsPicking(m_pTransformCom, &vOut, &iPickMeshIndex))
        {
            CImgManager::GetInstance()->SetPickingPoint(vOut);
        }
    }
}

CBossMap* CBossMap::Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext)
{
    CBossMap* pBossMap = new CBossMap(pGraphic_Device, pDeviceContext);
    if (FAILED(pBossMap->Initalize_Prototype()))
    {
        Safe_Release(pBossMap);
        MSG_BOX("CREATE FAIL : Boss MAP");
    }
    return pBossMap;
}

CGameObject* CBossMap::Clone(void* pArg)
{
    CBossMap* pBossMap = new CBossMap(*this);
    if (FAILED(pBossMap->Initialize(pArg)))
    {
        Safe_Release(pBossMap);
        MSG_BOX("CLONE FAIL : DEFAUTL MAP");
    }
    return pBossMap;
}

void CBossMap::Free()
{
    __super::Free();
    Safe_Release(m_pNavigationCom);
}

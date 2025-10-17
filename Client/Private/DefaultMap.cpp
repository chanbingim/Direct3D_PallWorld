#ifdef _DEBUG
#include "ImgManager.h"
#include "IMG_LandScape.h"
#endif // _DEBUG

#include "DefaultMap.h"

#include "GameInstance.h"



CDefaultMap::CDefaultMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CNoneAnimMesh(pDevice, pContext)
{
}

CDefaultMap::CDefaultMap(const CDefaultMap& rhs) :
    CNoneAnimMesh(rhs), 
    m_iTerrainCnt(rhs.m_iTerrainCnt)
{
}

HRESULT CDefaultMap::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CDefaultMap::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
        m_pNavigationCom->Update(XMLoadFloat4x4(&m_pTransformCom->GetWorldMat()));


    m_ObejctTag = TEXT("MapObject");
    return S_OK;
}

void CDefaultMap::Priority_Update(_float fDeletaTime)
{
  
}

void CDefaultMap::Update(_float fDeletaTime)
{
    _uInt iPickMeshIndex = {};
    _float3 vOut = {};

    //UpdateCullList();

#ifdef _DEBUG
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
                    SelectRenderPlane(iPickMeshIndex);
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
                    case 0 :
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
#endif // _DEBUG
}

void CDefaultMap::Late_Update(_float fDeletaTime)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CDefaultMap::Render()
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

HRESULT CDefaultMap::Bind_ShaderResources()
{
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    m_pEMVWorldMat = m_pShaderCom->GetVariable("g_WorldMatrix")->AsMatrix();
    m_pEMVViewMat = m_pShaderCom->GetVariable("g_ViewMatrix")->AsMatrix();
    m_pEMVProjMat = m_pShaderCom->GetVariable("g_ProjMatrix")->AsMatrix();
    m_pSRVEffect = m_pShaderCom->GetVariable("g_DiffuseTexture")->AsShaderResource();

    return S_OK;
}

HRESULT CDefaultMap::Apply_ConstantShaderResources(_uInt iMeshIndex)
{
    m_pEMVWorldMat->SetMatrix(reinterpret_cast<const float*>(&m_pTransformCom->GetWorldMat()));
    m_pEMVViewMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::VIEW)));
    m_pEMVProjMat->SetMatrix(reinterpret_cast<const float*>(&m_pGameInstance->GetMatrix(MAT_STATE::PROJECTION)));

    ID3D11ShaderResourceView* pResourceVeiw = {};
    m_pVIBufferCom->GetMeshResource(iMeshIndex, aiTextureType_DIFFUSE, 0, &pResourceVeiw);
    if (pResourceVeiw)
        m_pSRVEffect->SetResource(pResourceVeiw);

    return S_OK;
}

HRESULT CDefaultMap::ADD_Components()
{
    // 돌  모델 정보
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_DefaultMap"), TEXT("VIBuffer_Com"), (CComponent**)&m_pVIBufferCom)))
        return E_FAIL;

    // NonAnimShader
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Mesh"), TEXT("Shader_Com"), (CComponent**)&m_pShaderCom)))
        return E_FAIL;

    m_iTerrainCnt = m_pVIBufferCom->GetNumMeshes();
    m_pNavigationCom = CNavigation::Create(m_pGraphic_Device, m_pDeviceContext, m_pVIBufferCom);
    m_pComponentMap.emplace(TEXT("NaviMesh_Com"), m_pNavigationCom);
    return S_OK;
}

CNavigation* CDefaultMap::FindOnTerrian(_float3 vPosition)
{
    _float OnTerrianIndex = { -1 };
    _float Distance = INFINITY;
    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
    {
        _float fLength = {};
        if (m_pNavigationCom->IsInNaviMesh(vPosition, 50, &fLength))
        {
            if (Distance > fLength)
            {
                OnTerrianIndex = i;
                Distance = min(Distance, fLength);
            }
        }
    }

    if (-1 == OnTerrianIndex)
        return nullptr;

    return m_pNavigationCom;
}

void CDefaultMap::GetAllNaviMeshTriangle(list<NAVI_TRIANGLE>* pOut)
{
    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
        pOut->insert(pOut->end(), m_pNavigationCom->GetNaviMeshTriangleList().begin(),
                                  m_pNavigationCom->GetNaviMeshTriangleList().end());
}

void CDefaultMap::SelectRenderPlane(_uInt i)
{
    auto iter = find(m_MapRenderIndex.begin(), m_MapRenderIndex.end(), i);
    if(iter == m_MapRenderIndex.end())
        m_MapRenderIndex.push_back(i);
    else
        m_MapRenderIndex.erase(iter);
}

void CDefaultMap::UpdateCullList()
{
    _float3 vPlayerPos = {};
    XMStoreFloat3(&vPlayerPos, m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION));

    for (_uInt i = 0; i < m_iTerrainCnt; ++i)
    {
        _float fLength = {};
        if (m_pNavigationCom->IsInNaviMesh(vPlayerPos, 20.f, &fLength))
           m_MapRenderIndex.push_back(i);
    }
}

CDefaultMap* CDefaultMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CDefaultMap* pDefalutMap = new CDefaultMap(pDevice, pContext);
    if (FAILED(pDefalutMap->Initalize_Prototype()))
    {
        Safe_Release(pDefalutMap);
        MSG_BOX("CREATE FAIL : DEFAUTL MAP");
    }
    return pDefalutMap;
}

CGameObject* CDefaultMap::Clone(void* pArg)
{
    CDefaultMap* pDefalutMap = new CDefaultMap(*this);
    if (FAILED(pDefalutMap->Initialize(pArg)))
    {
        Safe_Release(pDefalutMap);
        MSG_BOX("CLONE FAIL : DEFAUTL MAP");
    }
    return pDefalutMap;
}

void CDefaultMap::Free()
{
    __super::Free();
    Safe_Release(m_pNavigationCom);
}

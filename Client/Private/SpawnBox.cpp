#include "SpawnBox.h"

#include "GameInstance.h"
#include "Level.h"

CSpawnBox::CSpawnBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CGameObject(pDevice, pContext)
{
}

CSpawnBox::CSpawnBox(const CSpawnBox& rhs) :
    CGameObject(rhs)
{
}

HRESULT CSpawnBox::Initalize_Prototype()
{
    return S_OK;
}

HRESULT CSpawnBox::Initialize(void* pArg)
{
    if(FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(ADD_Components()))
        return E_FAIL;


    SPAWN_BOX_DESC*         pSapwnBoxDesc = static_cast<SPAWN_BOX_DESC*>(pArg);
    if (pSapwnBoxDesc)
    {
        m_iCellIndex = pSapwnBoxDesc->iCellIndex;
        m_szSpawnPrototypeName = pSapwnBoxDesc->SpawnPrototypeName;
        m_fSpawnTime = pSapwnBoxDesc->fSpawnTime;
        m_fMaxSpawnCount = pSapwnBoxDesc->fMaxSpawnCount;
        m_bIsDestroy = pSapwnBoxDesc->bIsDestroySpawner;
    }
    return S_OK;
}

void CSpawnBox::Priority_Update(_float fDeletaTime)
{
    _vector vPlayerPos = m_pGameInstance->GetPlayerState(WORLDSTATE::POSITION);

    _float3 vBoxPosition = m_pTransformCom->GetPosition();
    m_fSpawnBoxToPlayerDistance = XMVectorGetX(XMVector3Length(vPlayerPos - XMLoadFloat3(&vBoxPosition)));
}

void CSpawnBox::Update(_float fDeletaTime)
{
    if (200.f <= m_fSpawnBoxToPlayerDistance)
        return;

    if (m_fAccSpawnTimeer >= m_fSpawnTime)
    {
        m_fAccSpawnTimeer = 0.f;
        if(FAILED(SpawnPrototpyeObject()))
            return;
    }
}

void CSpawnBox::Late_Update(_float fDeletaTime)
{
    if (200.f <= m_fSpawnBoxToPlayerDistance)
        return;

#ifdef _DEBUG
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
#endif // _DEBUG
}

HRESULT CSpawnBox::Render()
{
#ifdef _DEBUG
    m_pBoxCollision->Render();
#endif // _DEBUG

    return S_OK;
}

#ifdef _DEBUG
HRESULT CSpawnBox::ADD_Components()
{
    // 박스의 크기를 랜더링하기위해서 보여줄 범위
    COBBCollision::COLLISION_DESC CollisionDesc{};

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), m_szSpawnPrototypeName,TEXT("Collision_Com")
        , (CComponent **)&m_pBoxCollision, &CollisionDesc)))
        return E_FAIL;

    return S_OK;
}
#endif // _DEBUG

HRESULT CSpawnBox::SpawnPrototpyeObject()
{
    //뭐 거의 펠들만 생성할거임
    GAMEOBJECT_DESC GameObjectDesc = {};

    _uInt CurrentIndex = m_pGameInstance->GetCurrentLevel()->GetLevelID();
    m_pGameInstance->Add_GameObject_ToLayer(CurrentIndex, TEXT(""), 
        static_cast<CGameObject *>(m_pGameInstance->Clone_Prototype(OBJECT_ID::GAMEOBJECT, CurrentIndex, m_szSpawnPrototypeName, &GameObjectDesc)));

    return S_OK;
}

CSpawnBox* CSpawnBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSpawnBox* pSpawnBox = new CSpawnBox(pDevice, pContext);
    if (FAILED(pSpawnBox->Initalize_Prototype()))
    {
        Safe_Release(pSpawnBox);
        MSG_BOX("CREATE FAIL : SpawnBox");
    }

    return pSpawnBox;
}

CGameObject* CSpawnBox::Clone(void* pArg)
{
    CGameObject* pSpawnBox = new CSpawnBox(*this);
    if (FAILED(pSpawnBox->Initialize(pArg)))
    {
        Safe_Release(pSpawnBox);
        MSG_BOX("CLONE FAIL : SpawnBox");
    }

    return pSpawnBox;
}

void CSpawnBox::Free()
{
    __super::Free();
    Safe_Release(m_pBoxCollision);
}

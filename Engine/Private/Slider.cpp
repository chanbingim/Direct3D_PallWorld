#include "Slider.h"

#include "GameInstance.h"
#include "SliderButton.h"

CSlider::CSlider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
    CUserInterface(pDevice, pContext)
{

}

CSlider::CSlider(const CSlider& rhs) :
    CUserInterface(rhs)
{
}

HRESULT CSlider::Initalize_Prototype()
{
    if (FAILED(__super::Initalize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSlider::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    SLIDER_DESC* SliderDesc = static_cast<SLIDER_DESC*>(pArg);
    if (SliderDesc)
        m_eSliderType = SliderDesc->eType;

    switch (m_eSliderType)
    {
    case AXIS::HORIZONTAL :
        m_fSliderMaxMin = { (_float)m_UISize.left, (_float)m_UISize.right};
        break;
    case AXIS::VERTICAL:
        m_fSliderMaxMin = { (_float)m_UISize.top, (_float)m_UISize.bottom };
        break;
    }

    m_bIsMouseEvent = true;
    return S_OK;
}

void CSlider::Update(_float fDeletaTime)
{
    __super::Update(fDeletaTime);
}

void CSlider::Late_Update(_float fDeletaTime)
{

}

//SLider를 랜더링하고 그다음 SLiderBut을 랜더링한다.
HRESULT CSlider::Render()
{
    

    return S_OK;
}

_float CSlider::GetMouseMoveDir()
{
    if (AXIS::HORIZONTAL == m_eSliderType)
        return m_MoveDir.x;
    else
        return m_MoveDir.y;

    return 0;
}

void CSlider::MouseButtonDwon()
{
    m_ClickPos = m_pGameInstance->GetMousePoint();
}

void CSlider::MouseButtonPressed()
{
    UpdateSliderBar();
}

void CSlider::MouseButtonUp()
{
    m_ClickPos = { 0, 0};
}

void CSlider::UpdateSliderBar()
{
    if (m_pSliderBut)
    {
        POINT   MousePos = m_pGameInstance->GetMousePoint();
        m_MoveDir = {};

        _float3 vPos;
        _float2 vStartPos, vEndPos;
        vStartPos = vEndPos = GetViewPos();
        
        _float3 vScale = GetTransform()->GetScale();
        _float3 vButSize = m_pSliderBut->GetTransform()->GetScale();
       
        _float fOffset = {}, fMousePoint = {}, fHalf = {};
        if (m_eSliderType == AXIS::HORIZONTAL)
        {
            m_MoveDir.x = (MousePos.x - m_ClickPos.x) > 0 ? 1.f : -1.f;
            fOffset = vButSize.x * 0.5f;
            fMousePoint = (_float)m_pGameInstance->GetMousePoint().x;
            fHalf = (vScale.x * 0.5f) - fOffset;
            vStartPos.x -= fHalf;
            vEndPos.x += fHalf;
        }
        else
        {
            m_MoveDir.y= (MousePos.y - m_ClickPos.y) > 0 ? 1.f : -1.f;
            fOffset = vButSize.y * 0.5f;
            fMousePoint = (_float)m_pGameInstance->GetMousePoint().y;
            fHalf = (vScale.y * 0.5f) - fOffset;
            vStartPos.y -= fHalf;
            vEndPos.y += fHalf;
        }
        
        _float vPercentX = m_fSliderMaxMin.x + fOffset;
        _float vPercentY = m_fSliderMaxMin.y - fOffset;
        m_fSliderCurPercent = Clamp<_float>((fMousePoint - vPercentX) / vPercentY, 0.f, 1.f);

        XMStoreFloat3(&vPos, XMVectorLerp(XMLoadFloat2(&vStartPos), XMLoadFloat2(&vEndPos), m_fSliderCurPercent));
        m_pSliderBut->SetLocation(vPos);
    }
}

CGameObject* CSlider::Clone(void* pArg)
{
    return nullptr;
}

void CSlider::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pSliderBut);
}

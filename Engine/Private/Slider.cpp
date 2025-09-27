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
    if (m_bIsHover)
        m_ClickPos = m_pGameInstance->GetMousePoint();
}

void CSlider::MouseButtonPressed()
{
    if(m_bIsHover)
        UpdateSliderBar();
}

void CSlider::MouseButtonUp()
{
    m_ClickPos = { 0, 0};
}

void CSlider::UpdateSliderBar()
{
    _float StartPoint{};
    if (m_pSliderBut)
    {
        POINT MousPsisition = m_pGameInstance->GetMousePoint();

        _float3 vButPos = m_pSliderBut->GetTransform()->GetPosition();
        _float3 vButScale = m_pSliderBut->GetTransform()->GetScale();

        _float ButHalfX = vButScale.x * 0.5f;
        _float ButHalfY = vButScale.y * 0.5f;

        //길이가 나왔다면
        _float fLength = (m_fSliderMaxMin.y - ButHalfY) - (m_fSliderMaxMin.x + ButHalfX);
        if (AXIS::HORIZONTAL == m_eSliderType)
        {
            m_MoveDir.x = 0 < MousPsisition.x - m_ClickPos.x ? 1 : -1;
            m_fSliderCurPercent = (MousPsisition.x - m_fSliderMaxMin.x) / fLength;
            StartPoint = (m_pTransformCom->GetPosition().x - fLength * 0.5f);
        }
        else
        {
            m_MoveDir.y = 0 < MousPsisition.y - m_ClickPos.y ? 1 : -1;
            m_fSliderCurPercent = (MousPsisition.y - m_fSliderMaxMin.x) / fLength;
            StartPoint = (m_pTransformCom->GetPosition().y - fLength * 0.5f);
        }
        m_fSliderCurPercent = Clamp<_float>(m_fSliderCurPercent, 0.f, 1.f);

        _float fTranslation = Lerp<_float>(0, fLength, m_fSliderCurPercent);
        if (AXIS::HORIZONTAL == m_eSliderType)
            m_pSliderBut->SetLocation({ StartPoint + fTranslation , m_pSliderBut->GetViewPos().y, 0.f });
        else
            m_pSliderBut->SetLocation({ m_pSliderBut->GetViewPos().x, StartPoint + fTranslation, 0.f });
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

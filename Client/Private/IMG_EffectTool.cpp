#include "IMG_EffectTool.h"

#include "GameInstance.h"
#include "EditorCamera.h"
#include "EffectContatiner.h"
#include "EffectPartObject.h"
#include "Level.h"

#include "StringHelper.h"

const char* CIMG_EffectTool::szShowType[ENUM_CLASS(IMG_EFFECT_SHOW_TYPE::END)] = { "PART_OBJECT", "EFFECT_CONTATINER" };
const char* CIMG_EffectTool::szEffectTexType[ENUM_CLASS(IMG_EFFECT_TEXTURE_TYPE::END)] = { "DIFFUSE", "NORMAL", "DISTOTION", "MASK"};
const char* CIMG_EffectTool::szEffectType[ENUM_CLASS(EFFECT_TYPE::END)] = { "SPRTIE", "MODEL" };
const char* CIMG_EffectTool::szBlendType[ENUM_CLASS(EFFECT_BELND_MODE::END)] = { "DPETH_TEST", "ALPHA_BLEND" };
const char* CIMG_EffectTool::szDistotionType[ENUM_CLASS(EFFECT_DISTOTION_TYPE::END)] = { "LERP", "POLAR"};
const char* CIMG_EffectTool::szMaskType[ENUM_CLASS(EFFECT_MASK_TYPE::END)] = { "ADDTIVE", "MULTIPLY", "DIVIDE"};


CIMG_EffectTool::CIMG_EffectTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CImgUIBase(pDevice, pContext)
{
}

HRESULT CIMG_EffectTool::Prototype_Initialize()
{
	ADD_ModelViewCameraLayer();

	lstrcpy(m_EffectDesc.DiffuseTexturePath, TEXT("None"));
	lstrcpy(m_EffectDesc.NormalTexturePath, TEXT("None"));
	lstrcpy(m_EffectDesc.DistotionTexturePath, TEXT("None"));
	lstrcpy(m_EffectDesc.MaskTexturePath, TEXT("None"));
	return S_OK;
}

HRESULT CIMG_EffectTool::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_EffectTool::Update(_float fDeletaTime)
{
	bool bIsOpen = !static_cast<_uInt>(m_eVisibility);
	if (ImGui::Begin("WINDOW##EFFECT_WINDOW", &bIsOpen))
	{
		ImGui::InputText("EFFECT_NAME", m_CreateEffectName, MAX_PATH);
		if (ImGui::Button("RESET SELECT OBJECT EFFECT"))
			m_pSelectObject = nullptr;

		if (ImGui::Button("ADD PART OBJECT EFFECT"))
		{
			_uInt iLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();

			CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
			auto pPartEffect = CEffectPartObject::Create(m_pDevice, m_pContext);
			CStringHelper::ConvertUTFToWide(m_CreateEffectName, ObjectDesc.ObjectTag);
			ObjectDesc.vScale = { 1.f, 1.f, 1.f };
			pPartEffect->Initialize(&ObjectDesc);
			m_pGameInstance->ADD_Effects(0, ObjectDesc.ObjectTag, pPartEffect);
		}

		if (ImGui::Button("ADD PART EFFECT"))
		{
			_uInt iLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();

			CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
			auto pPartEffect = CEffectContatiner::Create(m_pDevice, m_pContext);
			CStringHelper::ConvertUTFToWide(m_CreateEffectName, ObjectDesc.ObjectTag);
			ObjectDesc.vScale = { 1.f, 1.f, 1.f };
			pPartEffect->Initialize(&ObjectDesc);
			m_pGameInstance->ADD_Effects(1, ObjectDesc.ObjectTag, pPartEffect);
		}

		if (m_pSelectObject)
		{
			m_pModelCamera->Priority_Update(fDeletaTime);

			auto Effect_Contatiner = dynamic_cast<CEffectContatiner*>(m_pSelectObject);
			if (Effect_Contatiner)
			{
				//여기서는 컨테이너 오브젝트에 대한 동작
			}
			else
			{
				auto pPartObject = static_cast<CEffectPartObject*>(m_pSelectObject);
				pPartObject->SetPartEffectData(m_EffectDesc);
			}
			m_pSelectObject->Priority_Update(fDeletaTime);
			m_pSelectObject->Update(fDeletaTime);
			m_pSelectObject->Late_Update(fDeletaTime);
		}

	

		if (ImGui::Begin("Effect_Viwer"))
		{
			ImGui::InputText("SAVE_EFFECT_FILE_PATH", m_SaveFilePath, MAX_PATH);
			if (ImGui::Button("SELECT_PART_EFFECT_SAVE"))
			{
				if (m_pSelectObject)
				{
					auto Effect_Contatiner = dynamic_cast<CEffectContatiner*>(m_pSelectObject);
					if (Effect_Contatiner)
					{
						CEffectContatiner::EFFECT_SAVE_SESC Effect_ContatinerDesc = {};
						Effect_ContatinerDesc.szFilePath = m_SaveFilePath;
						Effect_Contatiner->ExportData(&Effect_ContatinerDesc);
					}
					else
					{
						CEffectPartObject::EFFECT_PARTOBJECT_DESC Effect_PartDesc = {};
						Effect_PartDesc.szFilePath = m_SaveFilePath;
						m_pSelectObject->ExportData(&Effect_PartDesc);
					}
					
				}
				
			}

			EffectSelect();
			EffectViewer();
		}
		ImGui::End();

		if (IMG_EFFECT_SHOW_TYPE::EFFECT_PART_OBJECT == m_eShowType)
		{
			if (ImGui::Begin("Effect_Data_Editor"))
			{
				EffectDataEditor();
				
			}
			ImGui::End();
		}
		else
		{
			if (ImGui::Begin("Effect_Container_Data_Editor"))
			{
				auto pPartEffectList = m_pGameInstance->GetALLEffects(0);
				if (ImGui::BeginCombo("SELECT_VIEW##PAROBJECTSLECTVIEW", m_SelectAddPartEffect))
				{
					if (nullptr == pPartEffectList)
						strcpy_s(m_SelectAddPartEffect, "NONE");
					else
					{
						for (auto& pair : *pPartEffectList)
						{
							CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_ConvertChar);
							if (ImGui::Selectable(m_ConvertChar, false))
							{
								
									strcpy_s(m_SelectAddPartEffect, m_ConvertChar);
							}
								
						}
					}
					ImGui::EndCombo();
				}

				if (ImGui::Button("CREATE##EFFECT_PART_CREATE_BUTTON"))
				{
					if (m_pSelectObject)
					{
						auto Effect_Contatiner = dynamic_cast<CEffectContatiner*>(m_pSelectObject);
						if (Effect_Contatiner)
						{
							WCHAR ConvertWideChar[MAX_PATH] = {};

							CStringHelper::ConvertUTFToWide(m_SelectAddPartEffect, ConvertWideChar);
							CGameObject::GAMEOBJECT_DESC ObjectDesc = {};
							ObjectDesc.vScale = { 1.f, 1.f, 1.f };
							Effect_Contatiner->ADD_EffectPartObject(ConvertWideChar, &ObjectDesc);
						}
					}
				}
			}
			ImGui::End();
		}
	}
	ImGui::End();
}

void CIMG_EffectTool::Render()
{
}

HRESULT CIMG_EffectTool::ADD_ModelViewCameraLayer()
{
	CBaseCamera::CAMERA_DESC Camera_Desc = {};

	Camera_Desc.vEye = { 0.f, 20.f, -20.f };
	Camera_Desc.vAt = { 0.f, 0.f, 0.f };

	Camera_Desc.fFov = XMConvertToRadians(60.f);
	Camera_Desc.fNear = 0.1f;
	Camera_Desc.fFar = 1000.f;

	m_pModelCamera = CEditorCamera::Create(m_pDevice, m_pContext);
	if (nullptr == m_pModelCamera)
		return E_FAIL;

	if (FAILED(m_pModelCamera->Initialize(&Camera_Desc)))
		return E_FAIL;

	return S_OK;
}

void CIMG_EffectTool::EffectSelect()
{
	// 블랜드 모드
	if (ImGui::BeginCombo("VIEW_TYPE##EFFECT_VIEW", m_SelectShowType))
	{
		for (auto i = 0; i < ENUM_CLASS(IMG_EFFECT_SHOW_TYPE::END); ++i)
		{
			if (ImGui::Selectable(szShowType[i], false))
			{
				strcpy_s(m_SelectShowType, szShowType[i]);
				m_eShowType = IMG_EFFECT_SHOW_TYPE(i);
			}
		}

		ImGui::EndCombo();
	}

	_uInt iLevel = m_pGameInstance->GetCurrentLevel()->GetLevelID();
	switch (m_eShowType)
	{
	case Client::CIMG_EffectTool::IMG_EFFECT_SHOW_TYPE::EFFECT_PART_OBJECT:
	{
		auto pPartEffectList = m_pGameInstance->GetALLEffects(0);
		if (ImGui::BeginCombo("SELECT_VIEW##PAROBJECTSLECTVIEW", m_PreVeiwEffectObject))
		{
			if (nullptr == pPartEffectList)
				strcpy_s(m_PreVeiwEffectObject, "NONE");
			else
			{
				for (auto& pair : *pPartEffectList)
				{
					CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_ConvertChar);
					if (ImGui::Selectable(m_ConvertChar, false))
					{
						strcpy_s(m_PreVeiwEffectObject, m_ConvertChar);
						m_pSelectObject = pair.second;
						auto pPartObject = static_cast<CEffectPartObject*>(m_pSelectObject);
						m_EffectDesc = pPartObject->GetEffectPartData();
						
					}
				}
			}

			ImGui::EndCombo();
		}
	}
		break;
	case Client::CIMG_EffectTool::IMG_EFFECT_SHOW_TYPE::EFFECT_CONTATINER:
	{
		auto pEffectList = m_pGameInstance->GetALLEffects(1);
		if (ImGui::BeginCombo("SELECT_VIEW##PAROBJECTSLECTVIEW", m_PreVeiwEffectObject))
		{
			if (nullptr == pEffectList)
				strcpy_s(m_PreVeiwEffectObject, "NONE");
			else
			{
				for (auto& pair : *pEffectList)
				{
					CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_ConvertChar);
					if (ImGui::Selectable(m_ConvertChar, false))
					{
						strcpy_s(m_PreVeiwEffectObject, m_ConvertChar);
						m_pSelectObject = pair.second;
					}
				}
			}

			ImGui::EndCombo();
		}
	}
		break;
	}
}

void CIMG_EffectTool::EffectViewer()
{
	// 랜더 타겟을 이용해 내가 원하는 시점에서 보게끔
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	ID3D11ShaderResourceView* pView = nullptr;
	_uInt iRenderTargetNum = m_pGameInstance->GetRenderTargetNum();
	ImGui::Image(m_pGameInstance->GetPostBuffer(iRenderTargetNum), DrawSize);
}

void CIMG_EffectTool::EffectTextureViewer()
{
	if (ImGui::Begin("TEXTURE_VIEWER##EFFECT_TEXTURE_ViEWER"))
	{
		
		if (ImGui::BeginCombo("TEXTURE_TYPE##EFFECT_TEXTURE_TYPE", m_SelectTextureType))
		{
			for (auto i = 0; i < ENUM_CLASS(IMG_EFFECT_TEXTURE_TYPE::END); ++i)
			{
				if (ImGui::Selectable(szEffectTexType[i], false))
				{
					strcpy_s(m_SelectTextureType, szEffectTexType[i]);
					m_eEffectTexType = IMG_EFFECT_TEXTURE_TYPE(i);
				}
			}

			ImGui::EndCombo();
		}

		// 블랜드 모드
		if (ImGui::Button("RESET_TEXTURE_NONE"))
		{
			switch (m_eEffectTexType)
			{
			case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::DIFFUSE:
				lstrcpy(m_EffectDesc.DiffuseTexturePath, TEXT("None"));
				break;
			case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::NORMAL:
				lstrcpy(m_EffectDesc.NormalTexturePath, TEXT("None"));
				break;
			case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::DISTOTION:
				lstrcpy(m_EffectDesc.DistotionTexturePath, TEXT("None"));
				break;
			case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::MASK:
				lstrcpy(m_EffectDesc.MaskTexturePath, TEXT("None"));
				break;
			}
		}


		auto AllDiffuseTexture = m_pGameInstance->GetALLTextureResource();
		_uInt iTexIndex = 0;
		for (auto& pTexture : *AllDiffuseTexture)
		{
			auto pSRV = pTexture.second->GetTexture(0);

			CStringHelper::ConvertWideToUTF(pTexture.first.c_str(), m_ConvertChar);
			_string Name = m_ConvertChar;
			Name += "##EFFECT_TEX";
			Name += iTexIndex;

			if (0 != iTexIndex % 3)
				ImGui::SameLine();

			if (ImGui::ImageButton(Name.c_str(), pSRV, { 70, 70 }))
			{
				switch (m_eEffectTexType)
				{
				case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::DIFFUSE:
					lstrcpy(m_EffectDesc.DiffuseTexturePath, pTexture.first.c_str());
					break;
				case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::NORMAL:
					lstrcpy(m_EffectDesc.NormalTexturePath, pTexture.first.c_str());
					break;
				case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::DISTOTION:
					lstrcpy(m_EffectDesc.DistotionTexturePath, pTexture.first.c_str());
					break;
				case Client::CIMG_EffectTool::IMG_EFFECT_TEXTURE_TYPE::MASK:
					lstrcpy(m_EffectDesc.MaskTexturePath, pTexture.first.c_str());
					break;
				}
			}

			iTexIndex++;
		}
	}
	ImGui::End();
}

void CIMG_EffectTool::EffectDataEditor()
{
#pragma region Transform
	if (ImGui::InputFloat3("EFFECT_POSITION", m_vPosition))
	{
		m_EffectDesc.vPosition = { m_vPosition[0], m_vPosition[1], m_vPosition[2] };
	}
	if (ImGui::InputFloat3("EFFECT_ROTATION", m_vRotation))
	{
		m_EffectDesc.vPosition = { XMConvertToRadians(m_vRotation[0]),
									XMConvertToRadians(m_vRotation[1]),
									XMConvertToRadians(m_vRotation[2]) };
	}
	if (ImGui::InputFloat3("EFFECT_SCALE", m_vScale))
	{
		m_EffectDesc.vScale = { m_vScale[0], m_vScale[1], m_vScale[2] };
	}
#pragma endregion

#pragma region Lerp
	ImGui::Checkbox("EFFECT_LERF_TRANSFORM", &m_EffectDesc.bIsLerp);
	if (ImGui::InputFloat3("EFFECT_LERP_END_POSITION", m_vLerpEndPosition))
	{
		m_EffectDesc.vEndPosition = { m_vLerpEndPosition[0], m_vLerpEndPosition[1], m_vLerpEndPosition[2] };
	}
	if (ImGui::InputFloat3("EFFECT_LERP_END_ROTATION", m_vLerpEndRotation))
	{
		m_EffectDesc.vEndPosition = { XMConvertToRadians(m_vLerpEndRotation[0]),
									  XMConvertToRadians(m_vLerpEndRotation[1]),
									  XMConvertToRadians(m_vLerpEndRotation[2]) };
	}
	if (ImGui::InputFloat3("EFFECT_LERP_END_SCALE", m_vLerpEndScale))
	{
		m_EffectDesc.vEndScale = { m_vLerpEndScale[0], m_vLerpEndScale[1], m_vLerpEndScale[2] };
	}
#pragma endregion

	//이펙트 타입
	if (ImGui::BeginCombo("TYPE##EFFECT_TYPE", m_SelectType))
	{
		for (auto i = 0; i < ENUM_CLASS(IMG_EFFECT_SHOW_TYPE::END); ++i)
		{
			if (ImGui::Selectable(szEffectType[i], false))
			{
				strcpy_s(m_SelectType, szEffectType[i]);
				m_EffectDesc.eType = EFFECT_TYPE(i);
			}
		}

		ImGui::EndCombo();
	}

	// 블랜드 모드
	if (ImGui::BeginCombo("BLEND_MODE##EFFECT_BLEND_MODE", m_SelectBlendMode))
	{
		for (auto i = 0; i < ENUM_CLASS(IMG_EFFECT_SHOW_TYPE::END); ++i)
		{
			if (ImGui::Selectable(szBlendType[i], false))
			{
				strcpy_s(m_SelectBlendMode, szBlendType[i]);
				m_EffectDesc.eBlend_Mode = EFFECT_BELND_MODE(i);
			}
		}

		ImGui::EndCombo();
	}

	//셰이더 선택
	auto EffectShaders = m_pGameInstance->GetALLShaderResource();
	if (ImGui::BeginCombo("SELECT_SHADER##EFFECT_SHADER", m_ShaderSelect))
	{
		for (auto& pair : *EffectShaders)
		{
			CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_ConvertChar);
			if (ImGui::Selectable(m_ConvertChar, false))
			{
				strcpy_s(m_ShaderSelect, m_ConvertChar);
				lstrcpy(m_EffectDesc.ShaderTag, pair.first.c_str());
			}
		}

		ImGui::EndCombo();
	}

	// 모델 선택
	auto ModelResources = m_pGameInstance->GetALLModelResource();
	if (ImGui::BeginCombo("SELECT_MODEL##EFFECT_MODEL", m_ModelSelect))
	{
		for (auto& pair : *ModelResources)
		{
			CStringHelper::ConvertWideToUTF(pair.first.c_str(), m_ConvertChar);
			if (ImGui::Selectable(m_ConvertChar, false))
			{
				strcpy_s(m_ModelSelect, m_ConvertChar);
				lstrcpy(m_EffectDesc.ModelTag, pair.first.c_str());
			}
		}

		ImGui::EndCombo();
	}

	ImGui::InputFloat("LifeTime##EFFECT_LifeTime", &m_EffectDesc.fLifeTime, 0);
	ImGui::InputFloat("RateTime##EFFECT_RateTime", &m_EffectDesc.fRateTime, 0);

	EffectTextureViewer();
	CStringHelper::ConvertWideToUTF(m_EffectDesc.DiffuseTexturePath, m_ConvertChar);
	ImGui::Text(m_ConvertChar);

	CStringHelper::ConvertWideToUTF(m_EffectDesc.NormalTexturePath, m_ConvertChar);
	ImGui::Text(m_ConvertChar);

	ImGui::DragFloat("NoiseStrength##EFFECT_NOSIE_STRENGTH", &m_EffectDesc.fNoiseStength, 0.01, -1.0f, 1.0f);

	if (ImGui::DragFloat2("UV_Rate_Time##EFFECT_NOSIE_SPEED", m_UVRatetime, 0.01, -1.0f, 1.0f))
	{
		m_EffectDesc.fUvRateTime = { m_UVRatetime[0], m_UVRatetime[1] };
	}

	// 블랜드 모드
	if (ImGui::BeginCombo("DISTION_MODE##EFFECT_DISTOTION_MODE", m_SelectDistotionMode))
	{
		for (auto i = 0; i < ENUM_CLASS(EFFECT_DISTOTION_TYPE::END); ++i)
		{
			if (ImGui::Selectable(szDistotionType[i], false))
			{
				strcpy_s(m_SelectDistotionMode, szDistotionType[i]);
				m_EffectDesc.eDistotionType = EFFECT_DISTOTION_TYPE(i);
			}
		}

		ImGui::EndCombo();
	}
	CStringHelper::ConvertWideToUTF(m_EffectDesc.DistotionTexturePath, m_ConvertChar);
	ImGui::Text(m_ConvertChar);

	ImGui::Checkbox("REVERSE_MASK_TEXTURE##EFFECT_REVERSE_MASK_TEX", &m_EffectDesc.bIsReverse);
	// 블랜드 모드
	if (ImGui::BeginCombo("MASK_MODE##EFFECT_MASK_MODE", m_SelectMaskMode))
	{
		for (auto i = 0; i < ENUM_CLASS(EFFECT_MASK_TYPE::END); ++i)
		{
			if (ImGui::Selectable(szMaskType[i], false))
			{
				strcpy_s(m_SelectMaskMode, szMaskType[i]);
				m_EffectDesc.eMaskType = EFFECT_MASK_TYPE(i);
			}
		}

		ImGui::EndCombo();
	}
	CStringHelper::ConvertWideToUTF(m_EffectDesc.MaskTexturePath, m_ConvertChar);
	ImGui::Text(m_ConvertChar);
	
	if (ImGui::ColorEdit4("EFEECT_COLOR##EFFECT_COLOR_4", m_vColor))
		m_EffectDesc.vColor = { m_vColor[0], m_vColor[1], m_vColor[2], m_vColor[3] };

	ImGui::Checkbox("EFEECT_LOOP##EFFECT_LOOP_BUTTON", &m_EffectDesc.bIsLoop);
}

CIMG_EffectTool* CIMG_EffectTool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	auto pImg_EffectTool = new CIMG_EffectTool(pDevice, pContext);
	if (FAILED(pImg_EffectTool->Prototype_Initialize()))
	{
		Safe_Release(pImg_EffectTool);
		MSG_BOX("CREATE FAIL : EFFECT TOOL");
	}
	return pImg_EffectTool;
}

void CIMG_EffectTool::Free()
{
	__super::Free();

	Safe_Release(m_pModelCamera);
}

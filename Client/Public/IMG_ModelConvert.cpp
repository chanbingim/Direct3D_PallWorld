#include "IMG_ModelConvert.h"

#include "EditorCamera.h"
#include "GameObject.h"
#include "AnimMeshActor.h"

#include "GameInstance.h"
#include "StringHelper.h"
#include "Level.h"

CIMG_ModelConvert::CIMG_ModelConvert(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CImgUIBase(pDevice, pContext)
{

}

HRESULT CIMG_ModelConvert::Prototype_Initialize()
{
	if (FAILED(ADD_ModelViewCameraLayer()))
		return E_FAIL;
	return S_OK;
}

HRESULT CIMG_ModelConvert::Initialize(void* pArg)
{
	return S_OK;
}

void CIMG_ModelConvert::Update(_float fDeletaTime)
{
	bool bIsOpen = !static_cast<_uInt>(m_eVisibility);
	if (ImGui::Begin("Window##ModelWindow", &bIsOpen))
	{
		if (!bIsOpen)
			m_eVisibility = VISIBILITY::HIDDEN;
		UpdateCurrentModelList();
		DrawConvertUI();

		if (ImGui::IsWindowFocused() && m_pSelectObejct)
		{
			m_pModelCamera->Priority_Update(fDeletaTime);
			m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, m_pSelectObejct);
		}
	}
	ImGui::End();
}

void CIMG_ModelConvert::Render()
{
	
}

void CIMG_ModelConvert::UpdateCurrentModelList()
{
	m_ShowGameObject.clear();
	//// 여기서  Model 컴포넌트를 가지고있는 녀석들을 확인하고 Export하게끔
	auto CurLevelLayers = m_pGameInstance->GetCurLevelLayer();
	_uInt CurLevelID = m_pGameInstance->GetCurrentLevel()->GetLevelID();

	for (auto& pair : *CurLevelLayers)
	{
		auto LayerObejcts = m_pGameInstance->GetAllObejctToLayer(CurLevelID, pair.first);
		for (auto& Object : *LayerObejcts)
		{
			auto pFindCom = Object->Find_Component(L"VIBuffer_Com");
			if (nullptr == pFindCom)
				continue;

			CModel* pModelCom = dynamic_cast<CModel*>(pFindCom);
			if (pModelCom)
				m_ShowGameObject.push_back(Object);
		}
	}
}

void CIMG_ModelConvert::DrawConvertUI()
{ 
	if (ImGui::BeginCombo("Select View", m_SelectObejctName))
	{
		_Int iIndex = 0;
		for (auto& iter : m_ShowGameObject)
		{
			CStringHelper::ConvertWideToUTF(iter->GetObjectTag().c_str(), m_showObejctName);
			sprintf_s(m_showObejctName, "%s##Editor_Model%d", m_showObejctName, iIndex);

			if (ImGui::Selectable(m_showObejctName, false))
			{
				strcpy_s(m_SelectObejctName, sizeof(m_showObejctName), m_showObejctName);
				m_pSelectObejct = iter;

				_float3 ObjectPos = m_pSelectObejct->GetTransform()->GetPosition();
				_vector vLookAt = XMLoadFloat3(&ObjectPos);

				_vector vCameraPos = m_pSelectObejct->GetTransform()->GetLookVector() * 7.f; 
				vCameraPos = vLookAt + vCameraPos;

				_float3 CameraAt = {};
				XMStoreFloat3(&CameraAt, vCameraPos);
				m_pModelCamera->SetLocation(CameraAt);
				m_pModelCamera->CameraLookAt(vLookAt);

				auto pFindCom = m_pSelectObejct->Find_Component(L"VIBuffer_Com");
				if (nullptr == pFindCom)
					continue;
				m_pSelectObjectModelCom = dynamic_cast<CModel*>(pFindCom);
			}
			iIndex++;
		}
		ImGui::EndCombo();
	}
	UpdateSelect();

	ImGui::InputText("Save Path", m_SaveFilePath, MAX_PATH);
	ImGui::SameLine();
	if (ImGui::Button("Make Binary"))
	{
		//모델에 있는 바이너리 파일 생성 호출
		auto pFindCom = m_pSelectObejct->Find_Component(L"VIBuffer_Com");
		CModel* pModelCom = dynamic_cast<CModel*>(pFindCom);
		if (pModelCom)
			pModelCom->Export(m_SaveFilePath);
	}

	// 랜더 타겟을 이용해 내가 원하는 시점에서 보게끔
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	ID3D11ShaderResourceView* pView = nullptr;
	_uInt iRenderTargetNum = m_pGameInstance->GetRenderTargetNum();
	ImGui::Image(m_pGameInstance->GetPostBuffer(iRenderTargetNum), DrawSize);
}

void CIMG_ModelConvert::UpdateSelect()
{
	if (m_pSelectObejct)
	{
		auto iter = find(m_ShowGameObject.begin(), m_ShowGameObject.end(), m_pSelectObejct);
		if (iter == m_ShowGameObject.end())
		{
			m_pSelectObejct = nullptr;
			m_pSelectObjectModelCom = nullptr;
			strcpy_s(m_SelectObejctName, MAX_PATH, "NONE");
		}
		else
		{
			if (ImGui::BeginCombo("Animation View", m_SelectObjAnimName))
			{
				if (m_pSelectObjectModelCom)
				{
					_uInt iNumAnimation = m_pSelectObjectModelCom->GetNumAnimations();
					for (_uInt i = 0; i < iNumAnimation; ++i)
					{
						if (ImGui::Selectable(m_pSelectObjectModelCom->GetAnimationName(i), false))
						{
							auto AnimMesh = dynamic_cast<CContainerObject*>(m_pSelectObejct);
							if (AnimMesh)
							{
								AnimMesh->SetAnimIndex(i);
								strcpy_s(m_SelectObjAnimName, m_pSelectObjectModelCom->GetAnimationName(i));
							}
						}
					}
				}
				ImGui::EndCombo();
			}
			
		}
	}
}

HRESULT CIMG_ModelConvert::ADD_ModelViewCameraLayer()
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

CIMG_ModelConvert* CIMG_ModelConvert::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIMG_ModelConvert* pModelConvert = new CIMG_ModelConvert(pDevice, pContext);
	if (FAILED(pModelConvert->Prototype_Initialize()))
	{
		Safe_Release(pModelConvert);
		MSG_BOX("CREATE FAIL : Model Convert");
	}
	return pModelConvert;
}

void CIMG_ModelConvert::Free()
{
	__super::Free();

	Safe_Release(m_pModelCamera);
}

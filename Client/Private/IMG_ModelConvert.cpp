#include "IMG_ModelConvert.h"

#include "EditorCamera.h"
#include "GameObject.h"
#include "ImgViewModel.h"

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

	if (FAILED(ADD_ModelViewLayer()))
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
		DrawMakeMappingDataUI();

		if (ImGui::IsWindowFocused() && m_pSelectObjectModelCom)
		{
			m_pModelCamera->Priority_Update(fDeletaTime);

			m_pViewModel->Update(fDeletaTime);
			m_pViewModel->Late_Update(fDeletaTime);
			m_pViewModel->Render();
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
	m_pGameInstance->GetPrototypeList<CModel>(CurLevelID, &m_ShowGameObject);
}

void CIMG_ModelConvert::DrawConvertUI()
{ 
	if (ImGui::BeginCombo("Select View", m_SelectObejctName))
	{
		_Int iIndex = 0;
		for (auto& iter : m_ShowGameObject)
		{
			CStringHelper::ConvertWideToUTF(iter.first.c_str(), m_showObejctName);
			sprintf_s(m_showObejctName, "%s##Editor_Model%d", m_showObejctName, iIndex);

			if (ImGui::Selectable(m_showObejctName, false))
			{
				strcpy_s(m_SelectObejctName, sizeof(m_showObejctName), m_showObejctName);
				m_pSelectObjectModelCom = iter.second;

				_float3 ObjectPos = { 0, 0, 0 };
				_vector vLookAt = XMLoadFloat3(&ObjectPos);

				_vector vCameraPos = XMVectorSet(0, 0, 1, 0) * 7.f; 
				vCameraPos = vLookAt + vCameraPos;

				_float3 CameraAt = {};
				XMStoreFloat3(&CameraAt, vCameraPos);
				m_pModelCamera->SetLocation(CameraAt);
				m_pModelCamera->CameraLookAt(vLookAt);

				m_pViewModel->Bind_Model(m_pSelectObjectModelCom);
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
		if(m_pSelectObjectModelCom)
			m_pSelectObjectModelCom->Export(m_SaveFilePath);
	}

	// 랜더 타겟을 이용해 내가 원하는 시점에서 보게끔
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	ID3D11ShaderResourceView* pView = nullptr;
	_uInt iRenderTargetNum = m_pGameInstance->GetRenderTargetNum();
	ImGui::Image(m_pGameInstance->GetPostBuffer(iRenderTargetNum), DrawSize);
}

void CIMG_ModelConvert::UpdateSelect()
{
	if (m_pSelectObjectModelCom)
	{
		auto iter = find_if(m_ShowGameObject.begin(), m_ShowGameObject.end(), [&](auto& pair)
			{
				if (m_pSelectObjectModelCom == pair.second)
					return true;

				return false;
			});

		if (iter == m_ShowGameObject.end())
		{
			m_pSelectObjectModelCom = nullptr;
			m_pViewModel->Bind_Model(m_pSelectObjectModelCom);
			strcpy_s(m_SelectObejctName, MAX_PATH, "NONE");
		}
		else
		{
			_uInt iNumAnimation = m_pSelectObjectModelCom->GetNumAnimations();
			if (iNumAnimation <= 0)
				m_pViewModel->SetAnimFlag(false);
			else
				m_pViewModel->SetAnimFlag(true);

			if (ImGui::BeginCombo("Animation View", m_SelectObjAnimName))
			{
				for (_uInt i = 0; i < iNumAnimation; ++i)
				{
					if (ImGui::Selectable(m_pSelectObjectModelCom->GetAnimationName(i), false))
					{
						m_pViewModel->SetAnimationIndex(i);
						strcpy_s(m_SelectObjAnimName, m_pSelectObjectModelCom->GetAnimationName(i));
					}
				}
				ImGui::EndCombo();
			}
			
		}
	}
}

void CIMG_ModelConvert::DrawMakeMappingDataUI()
{
	DrawMappingUI();
	ImGui::InputText("Model Mapping SavePath", m_SaveMappingFilePath, MAX_PATH);
	if (ImGui::Button("Make MappingTable"))
	{
		unordered_map<_string, pair<_Int, _Int>> MappingTable;
		m_pSelectObjectModelCom->ExportMappingData(m_pMappingModelCom, &MappingTable);

		ios_base::openmode flag;
		flag = ios::out | ios::trunc;

		ofstream file(m_SaveMappingFilePath, flag);
		if (file.is_open())
		{
			for (auto MappingTableData : MappingTable)
				file << MappingTableData.first << " " << MappingTableData.second.first << " " << MappingTableData.second.second << endl;
		}
		file.close();
	}
}

void CIMG_ModelConvert::DrawMappingUI()
{
	if (ImGui::BeginCombo("Select View##MappingSelect", m_SelectMappingObejctName))
	{
		_Int iIndex = 0;
		for (auto& iter : m_ShowGameObject)
		{
			CStringHelper::ConvertWideToUTF(iter.first.c_str(), m_ShowMappingObjName);
			sprintf_s(m_ShowMappingObjName, "%s##Mapping_Model%d", m_ShowMappingObjName, iIndex);

			if (ImGui::Selectable(m_ShowMappingObjName, false))
			{
				strcpy_s(m_SelectMappingObejctName, sizeof(m_ShowMappingObjName), m_ShowMappingObjName);
				m_pMappingModelCom = iter.second;
			}
			iIndex++;
		}
		ImGui::EndCombo();
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

HRESULT CIMG_ModelConvert::ADD_ModelViewLayer()
{
	m_pViewModel = CImgViewModel::Create(m_pDevice, m_pContext);
	if (nullptr == m_pViewModel)
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
	Safe_Release(m_pViewModel);
}

#include "Model.h"

#include "GameInstance.h"

#pragma region Model child
#include "Mesh.h"
#include "Bone.h"
#include "Material.h"
#include "Animation.h"
#pragma endregion

#include "StringHelper.h"
#include "PaseDataHeader.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) :
	CComponent(pDevice, pContext)
{
}

CModel::CModel(const CModel& rhs) :
	CComponent(rhs),
	m_Meshes(rhs.m_Meshes),
	m_iNumMeshes(rhs.m_iNumMeshes),
	m_iNumMaterials(rhs.m_iNumMaterials),
	m_iNumBones(rhs.m_iNumBones),
	m_iNumAnimations(rhs.m_iNumAnimations),
	m_PreTransformMatrix(rhs.m_PreTransformMatrix),
	m_Materials(rhs.m_Materials),
	m_PreAnimBones(rhs.m_PreAnimBones),
	m_RetargetIndices(rhs.m_RetargetIndices),
	m_CurAnimBones(rhs.m_CurAnimBones),
	m_CurrentAnimIndexs(rhs.m_CurrentAnimIndexs),
	m_eType(rhs.m_eType)
{
	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

	for (auto& pBone : rhs.m_Bones)
		m_Bones.push_back(pBone->Clone());

	for (auto& pAnimation : rhs.m_Animations)
		m_Animations.push_back(pAnimation->Clone());
}

HRESULT CModel::Initialize_Prototype(MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat, const char* RetargetFile, _uInt iLayerCount)
{
	_char		szEXT[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);
	m_eType = eType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreModelMat);
	if (!strcmp(szEXT, ".fbx"))
	{
		_uInt iFlags = {};

		iFlags = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
		if (MODEL_TYPE::NONANIM == m_eType)
			iFlags |= aiProcess_PreTransformVertices;
		
		m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlags);
		if (nullptr == m_pAIScene)
			return E_FAIL;

		if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
			return E_FAIL;
		

		if (FAILED(Ready_Meshes(PreModelMat)))
			return E_FAIL;

		if (FAILED(Ready_Materials(pModelFilePath)))
			return E_FAIL;

		if (FAILED(Ready_Animations()))
			return E_FAIL;

	}
	else if ((!strcmp(szEXT, ".dat")))
	{
		if (MODEL_TYPE::NONANIM == m_eType)
		{
			SAVE_MODEL_DESC Data = {};
			ReadModelFile(&Data, pModelFilePath);

			if (FAILED(Ready_Meshes(&Data, PreModelMat)))
				return E_FAIL;

			if (FAILED(Ready_Materials(&Data, pModelFilePath)))
				return E_FAIL;
		}
		else
		{
			SAVE_ANIM_MODEL_DESC Data = {};
			ReadAnimModelFile(&Data, pModelFilePath);
			if (FAILED(Ready_Bones(&Data)))
				return E_FAIL;

			if (FAILED(Ready_Meshes(&Data, PreModelMat)))
				return E_FAIL;

			if (FAILED(Ready_Materials(&Data, pModelFilePath)))
				return E_FAIL;

			if (FAILED(Ready_Animations(&Data)))
				return E_FAIL;
		}
	}
	m_iNumBones = (_uInt)m_Bones.size();
	if (strcmp(RetargetFile, ""))
		ReadReTargetlFile(RetargetFile);

	m_PreAnimBones.resize(m_iNumBones, false);
	m_CurAnimBones.resize(m_iNumBones, false);
	m_CurrentAnimIndexs.resize(iLayerCount, -1);
	return S_OK;
}

HRESULT CModel::Initialize(void* pArg, const char* RetargetFile)
{
	return S_OK;
}

HRESULT CModel::Render(_uInt iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render_VIBuffer();

	return S_OK;
}

HRESULT CModel::GetMeshResource(_uInt iMeshIndex, aiTextureType eType, _uInt iTextureIndex, ID3D11ShaderResourceView** ppOut)
{
	if (m_iNumMeshes <= iMeshIndex || 0 > iMeshIndex)
		return E_FAIL;

	_uInt MatrialIndex =m_Meshes[iMeshIndex]->GetMatrialIndex();
	if (MatrialIndex >= m_iNumMaterials || MatrialIndex < 0)
		return E_FAIL;

	*ppOut = m_Materials[MatrialIndex]->GetMaterial(eType, iTextureIndex);
	if (nullptr == *ppOut)
		return E_FAIL;

	return S_OK;
}

_uInt CModel::GetMeshNumBones(_uInt iMeshIndex) const
{
	_uInt iNumBone = m_Meshes[iMeshIndex]->GetMeshNumBone();
	if (512 < iNumBone)
	{
		MSG_BOX("OUT BOUND : NUM BONE (MAX : 512)");
		return 512;
	}

	return iNumBone;
}

_matrix CModel::GetBoneTransformation(const char* szBoneName)
{
	_uInt BoneiIndex = GetBoneIndex(szBoneName);
	return m_Bones[BoneiIndex]->GetCombinedTransformationMatrix();
}

_matrix CModel::GetBoneTransformation(_uInt iIndex)
{
	return m_Bones[iIndex]->GetBoneTransformMatrix();
}

_Int CModel::GetBoneIndex(const char* szBoneName) const
{
	_Int	iBoneIndex = {0};
	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)
		{
			if (pBone->CompareName(szBoneName))
				return true;

			iBoneIndex++;
			return false;
		});

	if (iter == m_Bones.end())
		return -1;
	return iBoneIndex;
}

_float4x4* CModel::GetBoneMatrices(_uInt iMeshIndex)
{
	return m_Meshes[iMeshIndex]->GetMeshBoneMatrices(m_Bones);
}

const _float4x4* CModel::GetCombinedTransformationMatrixPtr(const char* szBoneName) const
{
	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)
	{
		return pBone->CompareName(szBoneName);
	});

	if (iter == m_Bones.end())
		return nullptr;
	return (*iter)->GetCombinedTransformationMatrixPtr();
}

_bool CModel::PlayAnimation(_uInt iAnimLayerIndex, _uInt iCurrentAnimIndex, _float fDeletaTime, _float fAnimSpeed, _bool bIsLoop, const char* BoneName, const char* EndBoneName)
{
	_bool Finished = false;
	if (-1 == iCurrentAnimIndex ||
		iCurrentAnimIndex >= m_iNumAnimations)
		return Finished;

	_Int RootBoneIndex{}, ChildEndIndex{};
	if (strcmp(BoneName, "Root Node"))
	{
		RootBoneIndex = GetBoneIndex(BoneName);
		if (strcmp(EndBoneName, ""))
			ChildEndIndex = GetBoneIndex(EndBoneName);
		else
			ChildEndIndex = m_Bones[RootBoneIndex]->GetChildCount();
	}
	else
	{
		RootBoneIndex = 0;
		ChildEndIndex = (_uInt)m_Bones.size();
	}

	ChangeAnimation(iAnimLayerIndex, iCurrentAnimIndex);

	if (0 > m_CurrentAnimIndexs[iAnimLayerIndex] || m_Animations.size() <= m_CurrentAnimIndexs[iAnimLayerIndex])
		return false;

	if (!m_bIsLerpAnimation)
		/* 내가 재생하고자하는 애니메이션(공격모션)이 이용하고 있는 뼈들의 상태 변환정보(TransformationMatrix)를 갱신해준다.*/
		Finished = m_Animations[m_CurrentAnimIndexs[iAnimLayerIndex]]->UpdateTransformationMatrices(m_Bones, fDeletaTime, { RootBoneIndex, RootBoneIndex + ChildEndIndex }, bIsLoop);
	else
		LerpAnimation(iAnimLayerIndex, fDeletaTime, fAnimSpeed, { RootBoneIndex, RootBoneIndex + ChildEndIndex });

	/* 모든 뼈를 순회하면서 CombinedTransformationMatrix를 갱신한다. */
	for (auto& pBone : m_Bones)
		pBone->UpdateCombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

	return Finished;
}


void CModel::BindParentAnim(CModel* DstData)
{
	if (m_RetargetIndices.empty())
		return;

	_uInt i = 0;
	for (_uInt i = 0; i < m_iNumBones; ++i)
	{
		if (-1 != m_RetargetIndices[i])
			m_Bones[i]->SetBoneTransformMatrix(DstData->GetBoneTransformation(m_RetargetIndices[i]));

		m_Bones[i]->UpdateCombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}
}

const char* CModel::GetAnimationName(_uInt iIndex)
{
	if (0 > iIndex || m_iNumAnimations <= iIndex)
		return "";

	return m_Animations[iIndex]->GetAnimationName();
}

_Int CModel::GetNumAnimation(const char* szName)
{
	_uInt iIndex = 0;
	for (auto iter : m_Animations)
	{
		if (!strcmp(iter->GetAnimationName(), szName))
			return iIndex;

		iIndex++;
	}
	return -1;
}

HRESULT CModel::Ready_Meshes(void* MeshDesc, _matrix PreModelMat)
{
	if (MODEL_TYPE::NONANIM == m_eType)
	{
		SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(MeshDesc);
		m_iNumMeshes = ExportData->iNumMeshes;
		for (auto& Meshs : ExportData->MeshDesc)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, &Meshs, PreModelMat);
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}
	}
	else
	{
		SAVE_ANIM_MODEL_DESC* ExportData = static_cast<SAVE_ANIM_MODEL_DESC*>(MeshDesc);
		m_iNumMeshes = ExportData->iNumMeshes;
		for (auto& Meshs : ExportData->MeshDesc)
		{
			CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, &Meshs, PreModelMat);
			if (nullptr == pMesh)
				return E_FAIL;

			m_Meshes.push_back(pMesh);
		}
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(void* MatrialDesc, const _char* pModelFilePath)
{
	SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(MatrialDesc);
	m_iNumMaterials = ExportData->iNumMaterials;
	for (auto& Matrials : ExportData->MatrialDesc)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, &Matrials);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(void* BoneDesc)
{
	SAVE_ANIM_MODEL_DESC* ExportData = static_cast<SAVE_ANIM_MODEL_DESC*>(BoneDesc);
	m_iNumBones = ExportData->iNumBones;

	for (auto& BoneDesc : ExportData->BoneDesc)
	{
		CBone* pBone = CBone::Create(&BoneDesc);
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.push_back(pBone);
	}
	return S_OK;
}

HRESULT CModel::Ready_Animations(void* AnimationDesc)
{
	SAVE_ANIM_MODEL_DESC* ExportData = static_cast<SAVE_ANIM_MODEL_DESC*>(AnimationDesc);
	m_iNumAnimations = ExportData->iNumAnimations;
	for (auto& AnimationDesc : ExportData->AnimationDesc)
	{
		CAnimation* pAnimation = CAnimation::Create(&AnimationDesc);
		if (nullptr == pAnimation)
			continue;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

#ifdef _DEBUG
HRESULT CModel::Ready_Meshes(_matrix PreModelMat)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	for (_uInt i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eType, this, m_pAIScene->mMeshes[i], PreModelMat);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;
	for (_uInt i = 0; i < m_iNumMaterials; ++i)
	{
		CMaterial* pMaterial = CMaterial::Create(m_pDevice, m_pContext, pModelFilePath, m_pAIScene->mMaterials[i]);
		if (nullptr == pMaterial)
			return E_FAIL;

		m_Materials.push_back(pMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _Int iParentIndex)
{
	CBone* pBone = CBone::Create(pAINode, iParentIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);
	_Int	iParent = (_Int)(m_Bones.size() - 1);
	for (size_t i = 0; i < pAINode->mNumChildren; i++)
	{
		Ready_Bones(pAINode->mChildren[i], iParent);
	}

	size_t BoneSize = m_Bones.size() - 1;
	pBone->Set_ChildCount(_uInt(BoneSize - iParent));

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uInt i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnim = CAnimation::Create(this, m_pAIScene->mAnimations[i], true);
		if (nullptr == pAnim)
			continue;

		m_Animations.push_back(pAnim);
	}
	m_iNumAnimations = (_uInt)m_Animations.size();
	return S_OK;
}

void CModel::Export(const char* FilePath)
{
	if (MODEL_TYPE::NONANIM == m_eType)
	{
		SAVE_MODEL_DESC ExportData;
		//데이터를 여기서 구조체로 만들어서 Export한다.
		// 
		//Mesh Data Export
		ExportNonAnim(&ExportData);

		//여기서 직렬화
		if (FAILED(SaveModelFile(&ExportData, FilePath)))
			MSG_BOX("SAVE FAIL : MODEL DATA");
	}
	else
	{
		SAVE_ANIM_MODEL_DESC ExportData;
		//데이터를 여기서 구조체로 만들어서 Export한다.
		// 
		//Mesh Data Export
		ExportAnim(&ExportData);

		//여기서 직렬화
		if (FAILED(SaveAnimModelFile(&ExportData, FilePath)))
			MSG_BOX("SAVE FAIL : ANIM MODEL DATA");
	}
}

void CModel::ExportMappingData(CModel* DstData, unordered_map<_string, pair<_Int, _Int>>* pOut)
{
	_uInt iIndex = { };
	for (auto pBones : m_Bones)
	{
		_string MappingName = pBones->GetBoneName();
		_Int i = DstData->GetBoneIndex(MappingName.c_str());
		
		(*pOut).emplace(MappingName, make_pair(iIndex, i));
		iIndex++;
	}
}

void CModel::ExportNonAnim(void* pArg)
{
	SAVE_MODEL_DESC* ExportData = static_cast<SAVE_MODEL_DESC*>(pArg);

	ExportData->iNumMeshes = m_iNumMeshes;
	ExportData->MeshDesc.reserve(ExportData->iNumMeshes);
	for (auto iter : m_Meshes)
	{
		SAVE_MESH_DESC	MeshDesc;
		if (FAILED(iter->Export(&MeshDesc)))
			continue;
		else
			ExportData->MeshDesc.push_back(MeshDesc);
	}
	//Matrial Data Export
	ExportData->iNumMaterials = m_iNumMaterials;
	ExportData->MatrialDesc.reserve(ExportData->iNumMaterials);
	for (auto iter : m_Materials)
	{
		SAVE_MATERIAL_DESC MatrialDesc;
		if (FAILED(iter->Export(&MatrialDesc)))
			continue;
		else
			ExportData->MatrialDesc.push_back(MatrialDesc);
	}


}

void CModel::ExportAnim(void* pArg)
{
	SAVE_ANIM_MODEL_DESC* ExportData = static_cast<SAVE_ANIM_MODEL_DESC*>(pArg);

	ExportData->iNumMeshes = m_iNumMeshes;
	ExportData->MeshDesc.reserve(ExportData->iNumMeshes);
	for (auto iter : m_Meshes)
	{
		SAVE_ANIM_MESH_DESC	MeshDesc;
		if (FAILED(iter->AnimExport(&MeshDesc)))
			continue;
		else
			ExportData->MeshDesc.push_back(MeshDesc);
	}
	//Matrial Data Export
	ExportData->iNumMaterials = m_iNumMaterials;
	ExportData->MatrialDesc.reserve(ExportData->iNumMaterials);
	for (auto iter : m_Materials)
	{
		SAVE_MATERIAL_DESC MatrialDesc;
		if (FAILED(iter->Export(&MatrialDesc)))
			continue;
		else
			ExportData->MatrialDesc.push_back(MatrialDesc);
	}

	ExportData->iNumBones = m_iNumBones;
	
	ExportData->BoneDesc.reserve(ExportData->iNumBones);
	for (auto iter : m_Bones)
	{
		SAVE_BONE_DESC BoneDesc;
		iter->Export(&BoneDesc);
		ExportData->BoneDesc.push_back(BoneDesc);
	}

	ExportData->iNumAnimations = m_iNumAnimations;
	ExportData->AnimationDesc.reserve(ExportData->iNumAnimations);
	for (auto iter : m_Animations)
	{
		SAVE_ANIMATION_DESC AnimationDesc;
		iter->Export(&AnimationDesc);
		ExportData->AnimationDesc.push_back(AnimationDesc);
	}
}

HRESULT CModel::SaveModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_MODEL_DESC ExportData = *static_cast<SAVE_MODEL_DESC *>(Data);
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;
	char ResoucePath[MAX_PATH] = {};

	ofstream file(FilePath, flag);
	if (file.is_open())
	{
		file.write(reinterpret_cast<char*>(&ExportData.iNumMeshes), sizeof(_uInt));
		for (auto& MeshDesc : ExportData.MeshDesc)
		{
			file.write(MeshDesc.szName, MAX_PATH);
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumVertices), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumFaces), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumMaterialIndex), sizeof(_uInt));
			for (VTX_MESH& Vertex : MeshDesc.Vertices)
				file.write(reinterpret_cast<char*>(&Vertex), sizeof(VTX_MESH));

			for (_uInt& Index : MeshDesc.Indices)
				file.write(reinterpret_cast<char*>(&Index), sizeof(_uInt));
		}

		file.write(reinterpret_cast<char*>(&ExportData.iNumMaterials), sizeof(_uInt));
		for (auto& MatrialDesc : ExportData.MatrialDesc)
		{
			file.write(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (auto& TextureType : MatrialDesc.TextureType)
			{
				file.write(reinterpret_cast<char*>(&TextureType.iNumShaderResourceView), sizeof(_uInt));
				for (auto& ResourcePath : TextureType.TexturePath)
				{
					CStringHelper::ConvertWideToUTF(ResourcePath.c_str(), ResoucePath);
					file.write(ResoucePath, MAX_PATH);
				}
			}
		}
	}

	file.close();
	return S_OK;
}

HRESULT CModel::SaveAnimModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_ANIM_MODEL_DESC* ExportData = static_cast<SAVE_ANIM_MODEL_DESC*>(Data);
	ios_base::openmode flag;
	flag = ios::out | ios::trunc | ios::binary;
	char ResoucePath[MAX_PATH] = {};

	ofstream file(FilePath, flag);
	if (file.is_open())
	{
		file.write(reinterpret_cast<char*>(&ExportData->iNumMeshes), sizeof(_uInt));
		for (auto& MeshDesc : ExportData->MeshDesc)
		{
			file.write(MeshDesc.szName, MAX_PATH);
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumVertices), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumFaces), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumMaterialIndex), sizeof(_uInt));
			file.write(reinterpret_cast<char*>(&MeshDesc.iNumBones), sizeof(_uInt));
			for (VTX_ANIM_MESH& Vertex : MeshDesc.Vertices)
				file.write(reinterpret_cast<char*>(&Vertex), sizeof(VTX_ANIM_MESH));

			for (_uInt& Index : MeshDesc.Indices)
				file.write(reinterpret_cast<char*>(&Index), sizeof(_uInt));

			for (_uInt& Index : MeshDesc.BoneIndices)
				file.write(reinterpret_cast<char*>(&Index), sizeof(_uInt));

			for (_float4x4& OffsetMatrix : MeshDesc.OffsetMatrices)
				file.write(reinterpret_cast<char*>(&OffsetMatrix), sizeof(_float4x4));
		}

		file.write(reinterpret_cast<char*>(&ExportData->iNumMaterials), sizeof(_uInt));
		for (auto& MatrialDesc : ExportData->MatrialDesc)
		{
			file.write(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (auto& TextureType : MatrialDesc.TextureType)
			{
				file.write(reinterpret_cast<char*>(&TextureType.iNumShaderResourceView), sizeof(_uInt));
				for (auto& ResourcePath : TextureType.TexturePath)
				{
					CStringHelper::ConvertWideToUTF(ResourcePath.c_str(), ResoucePath);
					file.write(ResoucePath, MAX_PATH);
				}
			}
		}

		file.write(reinterpret_cast<char*>(&ExportData->iNumBones), sizeof(_uInt));
		for (auto& BoneDesc : ExportData->BoneDesc)
		{
			file.write(BoneDesc.szName, MAX_PATH);
			file.write(reinterpret_cast<char*>(&BoneDesc.PreTransformMat), sizeof(_float4x4));
			file.write(reinterpret_cast<char*>(&BoneDesc.iParentBoneIndex), sizeof(int));
			file.write(reinterpret_cast<char*>(&BoneDesc.iChildCnt), sizeof(int));
		}

		file.write(reinterpret_cast<char*>(&ExportData->iNumAnimations), sizeof(_uInt));
		for (auto& AnimDesc : ExportData->AnimationDesc)
		{
			file.write(AnimDesc.szAnimName, MAX_PATH);
			file.write(reinterpret_cast<char*>(&AnimDesc.fLength), sizeof(_float));
			file.write(reinterpret_cast<char*>(&AnimDesc.fTickPerSecond), sizeof(_float));
			file.write(reinterpret_cast<char*>(&AnimDesc.bIsLoop), sizeof(_bool));
			file.write(reinterpret_cast<char*>(&AnimDesc.iNumChannels), sizeof(_uInt));
			for (auto& ChannelDesc : AnimDesc.Channels)
			{
				file.write(ChannelDesc.szName, MAX_PATH);
				file.write(reinterpret_cast<char*>(&ChannelDesc.iBoneIndex), sizeof(_Int));
				file.write(reinterpret_cast<char*>(&ChannelDesc.iNumKeyFrames), sizeof(_uInt));
				for (auto& KeyDesc : ChannelDesc.KeyFrames)
					file.write(reinterpret_cast<char*>(&KeyDesc), sizeof(KEYFRAME));
			}
		}
	}

	file.close();
	return S_OK;
}
#endif // _DEBUG

HRESULT CModel::ReadModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_MODEL_DESC* ImportData = static_cast<SAVE_MODEL_DESC*>(Data);
	ios_base::openmode flag;
	flag = ios::in | ios::binary;
	char ResoucePath[MAX_PATH] = {};
	WCHAR WideResoucePath[MAX_PATH] = {};

	ifstream file(FilePath, flag);
	if (file.is_open())
	{
		file.read(reinterpret_cast<char*>(&ImportData->iNumMeshes), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumMeshes; ++i)
		{
			SAVE_MESH_DESC MeshDesc;
			file.read(MeshDesc.szName, MAX_PATH);
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumVertices), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumFaces), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&MeshDesc.iNumMaterialIndex), sizeof(_uInt));

			MeshDesc.Vertices.reserve(MeshDesc.iNumVertices);
			for (_uInt j = 0; j < MeshDesc.iNumVertices; ++j)
			{
				VTX_MESH Vertex;
				file.read(reinterpret_cast<char*>(&Vertex.vPosition), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vNormal), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vTangent), sizeof(_float3));
				file.read(reinterpret_cast<char*>(&Vertex.vTexcoord), sizeof(_float2));
				MeshDesc.Vertices.push_back(Vertex);
			}

			MeshDesc.Indices.reserve(MeshDesc.iNumFaces * 3);
			for (_uInt j = 0; j < MeshDesc.iNumFaces * 3; ++j)
			{
				_uInt Index;
				file.read(reinterpret_cast<char*>(&Index), sizeof(_uInt));
				MeshDesc.Indices.push_back(Index);
			}
			ImportData->MeshDesc.push_back(MeshDesc);
		}

		file.read(reinterpret_cast<char*>(&ImportData->iNumMaterials), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumMaterials; ++i)
		{
			SAVE_MATERIAL_DESC MatrialDesc;
			file.read(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (_uInt j = 0; j < MatrialDesc.iTextureTypeMax; ++j)
			{
				MATRIAL_TEXTURE_TYPE_DESC TextureDesc;
				file.read(reinterpret_cast<char*>(&TextureDesc.iNumShaderResourceView), sizeof(_uInt));

				for (_uInt k = 0; k < TextureDesc.iNumShaderResourceView; ++k)
				{
					file.read(ResoucePath, MAX_PATH);

					CStringHelper::ConvertUTFToWide(ResoucePath, WideResoucePath);
					TextureDesc.TexturePath.push_back(WideResoucePath);
				}

				MatrialDesc.TextureType.push_back(TextureDesc);
			}

			ImportData->MatrialDesc.push_back(MatrialDesc);
		}
	}

	file.close();
	return S_OK;
}

HRESULT CModel::ReadReTargetlFile(const char* FilePath)
{
	m_RetargetIndices.resize(m_iNumBones, 0);
	ios_base::openmode flag;
	flag = ios::in;

	ifstream file(FilePath, flag);
	if (file.is_open())
	{
		char szFileData[MAX_PATH];
		while (file)
		{
			_Int From, To;
			file >> szFileData >> From >> To;
			m_RetargetIndices[From] = To;
		}
	}
	file.close();
	return S_OK;
}

HRESULT CModel::ReadAnimModelFile(void* Data, const char* FilePath)
{
	if (nullptr == Data)
		return E_FAIL;

	SAVE_ANIM_MODEL_DESC* ImportData = static_cast<SAVE_ANIM_MODEL_DESC*>(Data);
	ios_base::openmode flag;
	flag = ios::in | ios::binary;
	char ResoucePath[MAX_PATH] = {};
	WCHAR WideResoucePath[MAX_PATH] = {};

	ifstream file(FilePath, flag);
	if (file.is_open())
	{
#pragma region Mesh Import
		file.read(reinterpret_cast<char*>(&ImportData->iNumMeshes), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumMeshes; ++i)
		{
			SAVE_ANIM_MESH_DESC AnimMeshDesc = {};
			file.read(AnimMeshDesc.szName, MAX_PATH);
			file.read(reinterpret_cast<char*>(&AnimMeshDesc.iNumVertices), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&AnimMeshDesc.iNumFaces), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&AnimMeshDesc.iNumMaterialIndex), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&AnimMeshDesc.iNumBones), sizeof(_uInt));

			AnimMeshDesc.Vertices.reserve(AnimMeshDesc.iNumVertices);
			for (_uInt i = 0; i < AnimMeshDesc.iNumVertices; ++i)
			{
				VTX_ANIM_MESH Vertex = {};
				file.read(reinterpret_cast<char*>(&Vertex), sizeof(VTX_ANIM_MESH));
				AnimMeshDesc.Vertices.push_back(Vertex);
			}

			_uInt iIndices = AnimMeshDesc.iNumFaces * 3;
			AnimMeshDesc.Indices.reserve(iIndices);
			for (_uInt i = 0; i < iIndices; ++i)
			{
				_uInt iIndex = {};
				file.read(reinterpret_cast<char*>(&iIndex), sizeof(_uInt));
				AnimMeshDesc.Indices.push_back(iIndex);
			}

			for (_uInt i = 0; i < AnimMeshDesc.iNumBones; ++i)
			{
				_uInt iBoneIndex = {};
				file.read(reinterpret_cast<char*>(&iBoneIndex), sizeof(_uInt));
				AnimMeshDesc.BoneIndices.push_back(iBoneIndex);
			}

			for (_uInt i = 0; i < AnimMeshDesc.iNumBones; ++i)
			{
				_float4x4 iOffsetMat = {};
				file.read(reinterpret_cast<char*>(&iOffsetMat), sizeof(_float4x4));
				AnimMeshDesc.OffsetMatrices.push_back(iOffsetMat);
			}
			ImportData->MeshDesc.push_back(AnimMeshDesc);
		}
#pragma endregion

#pragma region Matrial Import
		file.read(reinterpret_cast<char*>(&ImportData->iNumMaterials), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumMaterials; ++i)
		{
			SAVE_MATERIAL_DESC MatrialDesc;
			file.read(reinterpret_cast<char*>(&MatrialDesc.iTextureTypeMax), sizeof(_uInt));
			for (_uInt j = 0; j < MatrialDesc.iTextureTypeMax; ++j)
			{
				MATRIAL_TEXTURE_TYPE_DESC TextureDesc;
				file.read(reinterpret_cast<char*>(&TextureDesc.iNumShaderResourceView), sizeof(_uInt));

				for (_uInt k = 0; k < TextureDesc.iNumShaderResourceView; ++k)
				{
					file.read(ResoucePath, MAX_PATH);

					CStringHelper::ConvertUTFToWide(ResoucePath, WideResoucePath);
					TextureDesc.TexturePath.push_back(WideResoucePath);
				}

				MatrialDesc.TextureType.push_back(TextureDesc);
			}

			ImportData->MatrialDesc.push_back(MatrialDesc);
		}
#pragma endregion

#pragma region Bone Import
		file.read(reinterpret_cast<char*>(&ImportData->iNumBones), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumBones; ++i)
		{
			SAVE_BONE_DESC BoneDesc = {};
			file.read(BoneDesc.szName, MAX_PATH);
			file.read(reinterpret_cast<char*>(&BoneDesc.PreTransformMat), sizeof(_float4x4));
			file.read(reinterpret_cast<char*>(&BoneDesc.iParentBoneIndex), sizeof(_uInt));
			file.read(reinterpret_cast<char*>(&BoneDesc.iChildCnt), sizeof(_uInt));
			ImportData->BoneDesc.push_back(BoneDesc);
		}
#pragma endregion

#pragma region Animation Import
		file.read(reinterpret_cast<char*>(&ImportData->iNumAnimations), sizeof(_uInt));
		for (_uInt i = 0; i < ImportData->iNumAnimations; ++i)
		{
			SAVE_ANIMATION_DESC AnimDesc = {};
			file.read(AnimDesc.szAnimName, MAX_PATH);
			file.read(reinterpret_cast<char*>(&AnimDesc.fLength), sizeof(_float));
			file.read(reinterpret_cast<char*>(&AnimDesc.fTickPerSecond), sizeof(_float));
			file.read(reinterpret_cast<char*>(&AnimDesc.bIsLoop), sizeof(_bool));
			file.read(reinterpret_cast<char*>(&AnimDesc.iNumChannels), sizeof(_uInt));
			for (_uInt j = 0; j < AnimDesc.iNumChannels; ++j)
			{
				SAVE_CHANNEL_DESC ChannelDesc = {};
				file.read(ChannelDesc.szName, MAX_PATH);
				file.read(reinterpret_cast<char*>(&ChannelDesc.iBoneIndex), sizeof(_uInt));
				file.read(reinterpret_cast<char*>(&ChannelDesc.iNumKeyFrames), sizeof(_uInt));
				for (_uInt k = 0; k < ChannelDesc.iNumKeyFrames; ++k)
				{
					KEYFRAME KeyFrame = {};
					file.read(reinterpret_cast<char*>(&KeyFrame), sizeof(KEYFRAME));
					ChannelDesc.KeyFrames.push_back(KeyFrame);
				}
				AnimDesc.Channels.push_back(ChannelDesc);
			}
			ImportData->AnimationDesc.push_back(AnimDesc);
		}
#pragma endregion
	}

	file.close();
	return S_OK;
}

void CModel::ChangeAnimation(_uInt iAnimLayerIndex, _uInt iAnimIndex)
{
	// 여기서 Bone 가져올 필요가없음
	// Vector랑 같이 넘겨서 받아오자.
	if (0 > iAnimLayerIndex || m_CurrentAnimIndexs.size() <= iAnimLayerIndex)
		return;

	if (-1 != iAnimIndex)
	{
		if (m_CurrentAnimIndexs[iAnimLayerIndex] != iAnimIndex)
		{
			if (-1 != m_CurrentAnimIndexs[iAnimLayerIndex])
			{
				m_Animations[m_CurrentAnimIndexs[iAnimLayerIndex]]->GetUseBoneIndex(m_PreAnimBones);
				m_PreFrameTrackPos = m_Animations[m_CurrentAnimIndexs[iAnimLayerIndex]]->GetPreFrameKey();
				if (-1 != m_PreFrameTrackPos.x)
					m_bIsLerpAnimation = true;
			}

			m_CurrentAnimIndexs[iAnimLayerIndex] = iAnimIndex;
			m_Animations[m_CurrentAnimIndexs[iAnimLayerIndex]]->GetUseBoneIndex(m_CurAnimBones);
			for (_uInt i = 0; i < m_iNumBones; ++i)
			{
				if (false == (m_PreAnimBones[i] & m_CurAnimBones[i]))
				{
					m_Bones[i]->InitTransformationMatrix();
					m_PreAnimBones[i] = m_CurAnimBones[i];
				}
			}
		}
	}
}

_bool CModel::LerpAnimation(_uInt iAnimLayerIndex, _float fDeletaTime, _float AnimSpeed, _int2 UpdateBoneIdx)
{
	if (m_Animations[m_CurrentAnimIndexs[iAnimLayerIndex]]->UpdateTransformationMatrices(m_Bones, fDeletaTime, UpdateBoneIdx, &m_PreFrameTrackPos, 0.5f, AnimSpeed))
	{
		m_bIsLerpAnimation = false;
		return true;
	}

	return false;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODEL_TYPE eType, const _char* pModelFilePath, _matrix PreModelMat, const char* RetargetFile, _uInt iLayerCount)
{
	CModel* pModel = new CModel(pDevice, pContext);
	if (FAILED(pModel->Initialize_Prototype(eType, pModelFilePath, PreModelMat, RetargetFile, iLayerCount)))
	{
		Safe_Release(pModel);
		MSG_BOX("CREATE FAIL : MODEL");
	}

	return pModel;
}


CComponent* CModel::Clone(void* pArg)
{
	CModel* pModel = new CModel(*this);
	if (FAILED(pModel->Initialize(pArg)))
	{
		Safe_Release(pModel);
		MSG_BOX("CLONE FAIL : MODEL");
	}

	return pModel;
}

void CModel::Free()
{
	__super::Free();

	for (auto& iter : m_Meshes)
		Safe_Release(iter);

	for (auto& iter : m_Materials)
		Safe_Release(iter);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	m_Importer.FreeScene();
}
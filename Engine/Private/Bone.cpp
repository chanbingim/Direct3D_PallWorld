#include "Bone.h"

#include "PaseDataHeader.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _Int iParentIndex)
{
    // 뼈의 이름을 받아와서 복사
    // 뼈의 개수가 0인 녀석의 경우에는 매시의 이름을 그대로 파생하는거같음
    strcpy_s(m_szName, pAINode->mName.data);

    // 뼈는 계층구조이기떄문에 Root노드가 아닐경우 무조건 부모를 가진다.
    m_iParentBoneIndex = iParentIndex;

    // 정보를 읽어서 뼈의 위치 메트릭스를 통해서 갱신한다.
    memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));
    XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));
    m_InitTransformationMatrix = m_TransformationMatrix;

    // 나중을 위한 정보
    XMStoreFloat4x4(&m_TransformationOffsetMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
    return S_OK;
}

HRESULT CBone::Initialize(void* pArg)
{
    SAVE_BONE_DESC* BoneDesc =  static_cast<SAVE_BONE_DESC*>(pArg);

    // 뼈의 이름을 받아와서 복사
    // 뼈의 개수가 0인 녀석의 경우에는 매시의 이름을 그대로 파생하는거같음
    strcpy_s(m_szName, BoneDesc->szName);

    // 뼈는 계층구조이기떄문에 Root노드가 아닐경우 무조건 부모를 가진다.
    m_iParentBoneIndex = BoneDesc->iParentBoneIndex;

    // 정보를 읽어서 뼈의 위치 메트릭스를 통해서 갱신한다.
    m_TransformationMatrix = BoneDesc->PreTransformMat;
    m_iChildCnt = BoneDesc->iChildCnt;
    m_InitTransformationMatrix = m_TransformationMatrix;

    // 나중을 위한 정보
    XMStoreFloat4x4(&m_TransformationOffsetMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
    return S_OK;
}

void CBone::UpdateCombinedTransformationMatrix(const vector<CBone*>& Bones, _matrix PreTransformMatrix)
{
    if (-1 == m_iParentBoneIndex)
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
    else
        XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * Bones[m_iParentBoneIndex]->GetCombinedTransformationMatrix());
}

_matrix CBone::GetCombinedTransformationMatrix()
{
    return XMLoadFloat4x4(&m_CombinedTransformationMatrix);
}

const _float4x4* CBone::GetCombinedTransformationMatrixPtr()
{
    return &m_CombinedTransformationMatrix;
}

_float4x4* CBone::GetTransformationOffsetMatrixPtr()
{
    return &m_TransformationOffsetMatrix;
}

void CBone::SetBoneTransformMatrix(_matrix TransformMat)
{
    XMStoreFloat4x4(&m_TransformationMatrix, TransformMat * XMLoadFloat4x4(&m_TransformationOffsetMatrix));
}

_matrix CBone::GetBoneTransformMatrix()
{
    return XMLoadFloat4x4(&m_TransformationMatrix);
}

_bool CBone::CompareName(const _char* pBoneName)
{
    return !strcmp(pBoneName, m_szName);
}

void CBone::Export(void* pBoneDsec)
{
    SAVE_BONE_DESC* Desc = static_cast<SAVE_BONE_DESC *>(pBoneDsec);

    strcpy_s(Desc->szName, m_szName);
    Desc->PreTransformMat = m_TransformationMatrix;
    Desc->iParentBoneIndex = m_iParentBoneIndex;
    Desc->iChildCnt = m_iChildCnt;
}

void CBone::InitTransformationMatrix()
{
    m_TransformationMatrix = m_InitTransformationMatrix;
}

void CBone::Set_ChildCount(_uInt iCount)
{
    m_iChildCnt = iCount;
}

CBone* CBone::Create(const aiNode* pAINode, _Int iParentIndex)
{
    CBone* pBone = new CBone();
    if (FAILED(pBone->Initialize(pAINode, iParentIndex)))
    {
        Safe_Release(pBone);
        MSG_BOX("CREATE FAIL : BONE");
    }
    return pBone;
}

CBone* CBone::Create(void* pArg)
{
    CBone* pBone = new CBone();
    if (FAILED(pBone->Initialize(pArg)))
    {
        Safe_Release(pBone);
        MSG_BOX("CREATE FAIL : BONE");
    }
    return pBone;
}

CBone* CBone::Clone()
{
    return new CBone(*this);
}

void CBone::Free()
{
    __super::Free();
}

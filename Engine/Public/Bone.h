#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	HRESULT						Initialize(const aiNode* pAINode, _Int iParentIndex);
	void						UpdateCombinedTransformationMatrix(const vector<CBone*>& Bones, _matrix PreTransformMatrix);

	_matrix						GetCombinedTransformationMatrix();
	void						SetBoneTransformMatrix(_matrix TransformMat);
	_bool						CompareName(const _char* pBoneName);

	void						Export(void* pBoneDsec);

private:
	_char						m_szName[MAX_PATH] = {};
	_float4x4					m_TransformationMatrix = {}; /* 이 뼈만의 상태변환행렬 */
	_float4x4					m_CombinedTransformationMatrix = {}; /* m_TransformatinoMatrix * Parent`s m_CombinedTransformationMatrix */
	_Int						m_iParentBoneIndex = { -1 };

public:
	static		CBone*			Create(const aiNode* pAINode, _Int iParentIndex);
	virtual		void			Free() override;

};
NS_END
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
	HRESULT						Initialize(void* pArg);
	void						UpdateCombinedTransformationMatrix(const vector<CBone*>& Bones, _matrix PreTransformMatrix);

	_matrix						GetCombinedTransformationMatrix();
	void						SetBoneTransformMatrix(_matrix TransformMat);
	_matrix						GetBoneTransformMatrix();

	_bool						CompareName(const _char* pBoneName);

	void						Export(void* pBoneDsec);
	void						InitTransformationMatrix();

	void						Set_ChildCount(_uInt iCount);
	_Int						GetChildCount() { return m_iChildCnt; }

private:
	_char						m_szName[MAX_PATH] = {};

	_float4x4					m_InitTransformationMatrix = {};
	_float4x4					m_TransformationMatrix = {}; /* 이 뼈만의 상태변환행렬 */
	_float4x4					m_CombinedTransformationMatrix = {}; /* m_TransformatinoMatrix * Parent`s m_CombinedTransformationMatrix */
	
	_Int						m_iParentBoneIndex = { -1 };
	_Int						m_iChildCnt = {};
	
public:
	static		CBone*			Create(const aiNode* pAINode, _Int iParentIndex);
	static		CBone*			Create(void* pArg);

	CBone*						Clone();
	virtual		void			Free() override;

};
NS_END
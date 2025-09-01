#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CBone;
class CModel;

class CChannel : public CBase
{
private :
	CChannel();
	virtual ~CChannel() = default;
public:
	HRESULT					Initialize(const class CModel* pModel, const aiNodeAnim* pAIChannel);
	HRESULT					Initialize(void* pArg);
	
	void					Update_TransformationMatrix(vector<CBone*>& Bones, _float fCurrentTrackPosition, _uInt* iKeyIndex);
	void					Update_TransformationMatrix(vector<CBone*>& Bones, _float fRatio);
	void					Export(void* pChannelDesc);

	_float					GetPreKeyFrameTrackPos(_uInt iIndex);
	_Int					GetBoneIndex() { return m_iBoneIndex; }

private:
	_char					m_szName[MAX_PATH] = {};
	_Int					m_iBoneIndex = { -1 };

	_uInt					m_iNumKeyFrames = {};

	// 새로운 애니메이션을 재생할때 필요한 이전 뼈의 정보
	_bool					m_bIsBoneLerp = false;
	_float4x4				m_PreBoneMatrix = {};
	vector<KEYFRAME>		m_KeyFrames;

public:
	static		CChannel*	Create(const class CModel* pModel, const aiNodeAnim* pAIChannel);
	static		CChannel*	Create(void* pArg);
	virtual		void		Free() override;

};
NS_END
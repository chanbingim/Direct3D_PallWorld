#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CChannel;
class CModel;
class CBone;

class CAnimation final : public CBase
{
private :
	CAnimation();
	CAnimation(const CAnimation& rhs);
	virtual ~CAnimation() = default;
		
public :
	HRESULT			Initialize(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop = false);
	HRESULT			Initialize(void* pArg);

	_bool			UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta, _int2 UpdateBoneIdx, _bool bIsLoop);
	_bool			UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta, _int2 UpdateBoneIdx, _float2* LastAnimTrackPos, _float fLength, _float LerpSpeed);

	_bool			CompareAnimName(const char* szName);
	_float2			GetPreFrameKey();
	const	char*	GetAnimationName() { return m_szAnimName; }

	void			Export(void* pAnimationDesc);

	void			GetUseBoneIndex(vector<_bool>& BoneList);
	void			ResetAnimation();

	void			Bind_KeyFrameFunction(_uInt iKeyFrame, function<void()> function);

private :
	/*  애니 메이션에 대한 정보를 저장한다. */
	_char						m_szAnimName[MAX_PATH] = {};

	/* 현재 애니메이션의 재생 위치 */
	_float						m_fCurrentTrackPosition = {};

	/* 애니메이션 재생을 위한 전체 길이 */
	_float						m_fLength = {};

	/* 초당 얼마나 재생되어야하는지 : 재생 속도 */
	_float						m_fTickPerSecond = {};
	_uInt						m_iNumChannels = {};
	_Int						m_iLastCallFunctionIndex = { -1 };

	vector<CChannel*>							m_Channels;
	vector<_uInt>								m_iChannelIndex;


	unordered_map<_uInt, function<void()>>		m_KeyFrameCallFunction = {};
public :
	static		CAnimation*					Create(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop = false);
	static		CAnimation*					Create(void* pArg);
	CAnimation*								Clone();

	virtual		void						Free() override;

};
NS_END
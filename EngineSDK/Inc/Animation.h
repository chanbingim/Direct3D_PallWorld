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
	virtual ~CAnimation() = default;
		
public :
	HRESULT		Initialize(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop = false);
	void		UpdateTransformationMatrices(vector<CBone*>& Bones, _float fTimeDelta);

	_bool		CompareAnimName(const char* szName);

private :
	/*  애니 메이션에 대한 정보를 저장한다. */
	_char						m_szAnimName[MAX_PATH] = {};

	/* 현재 애니메이션의 재생 위치 */
	_float						m_fCurrentTrackPosition = {};

	/* 애니메이션 재생을 위한 전체 길이 */
	_float						m_fLength = {};

	/* 초당 얼마나 재생되어야하는지 : 재생 속도 */
	_float						m_fTickPerSecond = {};

	_bool						m_bIsLoop = false;

	_uInt						m_iNumChannels = {};
	vector<CChannel*>			m_Channels;

public :
	static		CAnimation*					Create(const CModel* pModel, const aiAnimation* pAIAnimation, _bool bIsLoop = false);
	virtual		void						Free() override;

};
NS_END
#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CChannel;

class CAnimation final : public CBase
{
private :
	CAnimation();
	virtual ~CAnimation() = default;
		
public :
	HRESULT		Initialize(const aiAnimation* pAIAnimation);
	void		UpdateTransformationMatrices(_float fTimeDelta);

private :
	_float						m_fCurrentTrackPosition = {};

	/* 애니메이션 재생을 위한 전체 길이 */
	_float						m_fLength = {};

	/* 초당 얼마나 재생되어야하는지 : 재생 속도 */
	_float						m_fTickPerSecond = {};

	_uInt						m_iNumChannels = {};
	vector<CChannel*>			m_Channels;

public :
	static		CAnimation*					Create(const aiAnimation* pAIAnimation);
	virtual		void						Free() override;

};
NS_END
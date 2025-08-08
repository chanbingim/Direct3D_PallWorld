#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class ENGINE_DLL CAnimEntity : public CBase
{
public :
	typedef struct Anim_Desc
	{
		_float								fFrame;
		_float								fRateTime;
		_bool								bIsLoop;
	}ANIM_DESC;

protected :
	CAnimEntity() = default;
	virtual ~CAnimEntity() = default;

public:
	//초기화
	virtual		HRESULT						Initialize(void* pArg);

public :
	//업데이트	
	virtual		void						Priority_Update();
	virtual		void						Update();
	virtual		void						Late_Update();

	
	HRESULT									BindKeyFrame(_uInt Frame, function<void()> Funtor);

	void									ReplayAnimation();
	_uInt									GetCurFrame();
	const _bool&							IsPlay();

	// LoopFlag = false Is true
	const _bool&							IsFinished();

protected :
	_float									m_fCurFrame = {};
	_float									m_fFrame = {};

	_bool									m_bIsPlay = false;
	_bool									m_bIsFinished = false;

private :
	_float									m_fIncTime = {};
	_float									m_fRateTime = {};

	_bool									m_bIsLoop = false;
	vector<pair<_uInt, function<void()>>>	m_FrameKey = {};

public:
	virtual			void					Free() override;
};
NS_END
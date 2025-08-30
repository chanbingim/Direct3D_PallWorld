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
	
	void					ResetChannel();
	void					Update_TransformationMatrix(vector<CBone*>& Bones, _float fCurrentTrackPosition);
	
	void					Export(void* pChannelDesc);

private:
	_char					m_szName[MAX_PATH] = {};
	_Int					m_iBoneIndex = { -1 };

	_uInt					m_iNumKeyFrames = {};
	vector<KEYFRAME>		m_KeyFrames;

	_uInt					m_iCurrentKeyFrameIndex = { 0 };

public:
	static		CChannel*	Create(const class CModel* pModel, const aiNodeAnim* pAIChannel);
	static		CChannel*	Create(void* pArg);
	virtual		void		Free() override;

};
NS_END
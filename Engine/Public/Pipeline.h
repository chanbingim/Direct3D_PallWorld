#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CPipeline final : public CBase
{
private :
	CPipeline();
	virtual ~CPipeline() = default;

public :
	//초기화
	HRESULT							Initalize();

	//업데이트	
	virtual		void				Update(_float fDeletaTime);
	void							SetMatrix(MAT_STATE eState, _float4x4 Matrix);

	const _float4x4&				GetMatrix(MAT_STATE eState);
	const _float4x4&				GetInvMatrix(INV_MAT_STATE eState);
	const _float4x4&				GetIndentityMatrix();

private :
	//행렬 모음
	_float4x4						m_PipeLineMat[ENUM_CLASS(MAT_STATE::END)];
	_float4x4						m_PipeLineInvMat[ENUM_CLASS(INV_MAT_STATE::END)];

	_float4x4						m_IdentityMat = {};

public :
	static		CPipeline*			Create();
	virtual		void				Free() override;

};
NS_END
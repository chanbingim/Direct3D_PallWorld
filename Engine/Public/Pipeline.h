#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

class CPipeline final : public CBase
{
private :
	CPipeline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPipeline() = default;

public :
	//초기화
	HRESULT							Initalize();

	//업데이트	
	virtual		void				Update(_float fDeletaTime);
	void							SetMatrix(MAT_STATE eState, _float4x4 Matrix);

	const _float4x4&				GetMatrix(MAT_STATE eState);
	const _float4x4&				GetInvMatrix(MAT_STATE eState);
	const _float4x4&				GetIndentityMatrix();

	void							SetPostBuffer(_uInt iIndex, ID3D11ShaderResourceView* pTex);
	void							SetPlayerWorldMatrix(_float4x4 Matrix);

	//카메라 Enum값 그대로 넣으면됨
	void							SetCameraInfo(const _float4& pCmaeraInfo);
	ID3D11ShaderResourceView*		GetPostBuffer(_uInt iIndex);

	_vector							GetCameraState(WORLDSTATE eType);
	const _float4&					GetCameraINFO() { return m_CameraInfo; }

	_vector							GetPlayerState(WORLDSTATE eType);

private :
	CGameInstance*					m_pGameInstance = nullptr;
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;

	_float4							m_CameraInfo;

	//행렬 모음
	_float4x4						m_PipeLineMat[ENUM_CLASS(MAT_STATE::END)];
	_float4x4						m_PipeLineInvMat[ENUM_CLASS(MAT_STATE::END)];
	
	_float4x4						m_PlayerWorldMat = {};
	_float4x4						m_IdentityMat = {};

	/* Textures Var */
	D3D11_TEXTURE2D_DESC				m_PostDesc = {};
	vector<ID3D11ShaderResourceView*>	m_PipeTextures = {};
	
private :
	HRESULT							SettingPipeTextures();

public :
	static		CPipeline*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		void				Free() override;

};
NS_END
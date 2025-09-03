#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CBaseCamera abstract : public CGameObject
{
public :
	typedef struct Camera_Desc : public CGameObject::GAMEOBJECT_DESC
	{
		_float3	 vEye{}, vAt{};
		_float	 fFov{}, fFar{}, fNear{};
	}CAMERA_DESC;

protected:
	CBaseCamera(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CBaseCamera(const CBaseCamera& rhs);
	virtual ~CBaseCamera() = default;

public:
	//초기화
	virtual			HRESULT						Initalize_Prototype();
	virtual			HRESULT						Initialize(void* pArg);

	//업데이트		
	virtual			void						Priority_Update(_float fDeletaTime);
	virtual			void						Update(_float fDeletaTime);
	virtual			void						Late_Update(_float fDeletaTime);

	// 랜더
	virtual			HRESULT						Render();

	
	void										SetAspect(_float fWidth, _float fHeight);
	void										CameraLookAt(_vector LookAt);
	void										SetFov(_float fAngle);
	_bool										IsInPoint(_float3& vPos, _float offset);
	 
#ifdef _DEBUG
	virtual void								ExportData(void* pArg);
#endif // _DEBUG

protected :
	//카메라 정보
	_float										m_fFov = {};
	_float										m_fAspect = {};
	_float										m_fFar = {};
	_float										m_fNear = {};

	_float4x4									m_ProjMat = {};

	vector<_float3>								m_FustomPointVec;
	vector<_vector>								m_FustomWorldPointVec;
	_float4										m_FustomPlane[6] = {};

protected:
	void										Compute_FustomPlane();

public:
	virtual			CGameObject*				Clone(void* pArg);
	virtual			void						Free() override;
};
NS_END
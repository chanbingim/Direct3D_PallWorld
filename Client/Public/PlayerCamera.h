#pragma once
#include "Client_Define.h"
#include "BaseCamera.h"

NS_BEGIN(Client)
class CPlayerCamera : public CBaseCamera
{
public :
	enum class CAMERA_MODE
	{
		NONE_AIMMING = 0b00000001,
		AIMING = 0b00000010,
		NONE
	};

	typedef struct PlayerCmaeraDesc : public CAMERA_DESC
	{
		const _float4x4*					pSocketMatrix;
	}PLAYER_CAMERA_DESC;

private:
	CPlayerCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerCamera(const CPlayerCamera& rhs);
	virtual ~CPlayerCamera() = default;

public:
	//초기화
	virtual		HRESULT				Initalize_Prototype() override;
	virtual		HRESULT				Initialize(void* pArg) override;

	virtual			void			Priority_Update(_float fDeletaTime)override;
	virtual			void			Update(_float fDeletaTime)override;
	virtual			void			Late_Update(_float fDeletaTime)override;

	// 랜더
	virtual			HRESULT			Render()override;
	void							AttachCamera(_vector BonesTransformation);

	void							ADDRevolutionRotation(_float Angle, _float DeletaTime);
	void							ADDPitchRotation(_float Angle, _float DeletaTime);

	void							SetChangeCameraMode(CAMERA_MODE eMode);

private:
	CAMERA_MODE						m_CameraModel = {};

	const _float4x4*				m_SocketMatrix = nullptr;
	_float							m_AccYawAngle = {};

	_float4x4						m_CombinedMatrix = {};
	_float4x4						m_InvCombinedMatrix = {};

	_float							m_RevolutionAngle = {};

public:
	static	CPlayerCamera*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};
NS_END
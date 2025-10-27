#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Client)
class CPlayer; 
class CSunLight;

class CShadowCamera : public CGameObject
{
public :
	typedef struct ShadowCameraDesc
	{
		_float			fNear;
		_float			fFar;
		_float			fFov;
	}SHADOW_CAMERA_DESC;

protected:
	CShadowCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShadowCamera(const CShadowCamera& rhs);
	virtual ~CShadowCamera() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

private :
	const			CPlayer*				m_pPlayer = nullptr;
	const			CSunLight*				m_pSunLight = nullptr;
	SHADOW_LIGHT_DESC						m_pShadowLightDesc = {};

public:
	static			CShadowCamera*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
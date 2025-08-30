#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;

class CPicking : public CBase
{
private :
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public :
	void							Update();

	void							SetEditor_MousePos(const _float3& vPos);
	void							SetEditor_Frame(const _float2& vSize);
	void							Change_Mode(GAMEMODE eType);
	const GAMEMODE&					GetGameMode();

	_bool							RayPicking(_vector vRayOrizin, _vector vRayDir, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);

	_bool							Picking_InWorld(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool							Picking_InLocal(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);


	void							Compute_LocalRay(const _matrix* InvWorldMatrix);

	const _float3&					GetRayPos(RAY eType);
	const _float3&					GetRayDir(RAY eType);

	void							SetHitPoint(_float3 vHitPoint);
private :
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pContext = nullptr;
	CGameInstance*					m_pGameInstance = nullptr;

	GAMEMODE						m_eMode = GAMEMODE::END;

	_float3							m_EditorMousePos = {};
	_float2							m_EditorSize = {};

	_float3							m_RayPos[ENUM_CLASS(RAY::END)] = {};
	_float3							m_RayDir[ENUM_CLASS(RAY::END)] = {};

	_bool							m_bIsHit = false;
	_float3							m_LastPickPoint = {};

public :
	static	CPicking*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual	void					Free() override;

};
NS_END
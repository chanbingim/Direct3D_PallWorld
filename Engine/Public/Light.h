#pragma once

#include "Base.h"

NS_BEGIN(Engine)
class CLight : public CBase
{
public :
	typedef struct LightDesc
	{
		LIGHT			eType;
		_float3			vDiffuse;
		_float3			vAmbient;
		_float3			vSpecular;

		_float3			vPosition;
		_float3			vDirection;        /* 광원의 빛의 방향 */
		_float			fRange;            /* 빛의 최대 진행 거리 방향성 광원의 경우 의이없음 */
		/* 루트 FLT_MAX의 값을 넘어갈수 없다.  */

		_float          Falloff;          /* 스포트 광원에서 사용 보통 1.0f 설정하며     */
		/* 안쪽 원뿔과 바깥쪽 원뿔간의 빛의 세기 차이  */

		_float           Attenuation0;     /* 상수 감쇠 계수 */
		_float           Attenuation1;     /* 선형 감쇠 계수 */
		_float           Attenuation2;     /* 이차 감쇠 계수 */
		_float           Theta;            /* 스포트 광원의 안쪽 원뿔의 각도 */
		_float           Phi;              /* 스포트 광원의 바깥쪽 원뿔의 각도 */
	}LIGHT_DESC;

private :
	CLight(LIGHT_DESC LightInfo);
	virtual ~CLight() = default;

public :
	const LIGHT_DESC&		GetLightInfo() { return m_LightInfo; }
	HRESULT					Render(class CShader* pShader, class CVIBuffer* pVIBuffer);

private :
	LIGHT_DESC				m_LightInfo = {};

public :
	static		CLight*		Create(const LIGHT_DESC& Desc);
	virtual		void		Free() override;

};
NS_END
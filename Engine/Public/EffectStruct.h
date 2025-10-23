#pragma once
#include "Engine_Defines.h"

namespace Engine
{
	enum class EFFECT_TYPE				{ SPRITE, MESH, END };
	enum class EFFECT_BELND_MODE		{ DEPTHTEST, APLPHA_BLEND, END };

	enum class EFFECT_DISTOTION_TYPE	{ LERP, POLAR, END };
	enum class EFFECT_MASK_TYPE			{ ADDTIVE, MULTIPLY, DIVIDE, END };

	typedef struct Effect_Network_Desc
	{
		// 이펙트 타입도 하나 주자
		// 어떤 이펙트가 어디에 생성되어야하는지는 PartObject일때 저장해주면 되는것
		// 매시형태라면 
		// 특정 이펙트의 정보를 데이터 파일화한다.
		WCHAR						EffectTag[MAX_PATH]; // 이펙트를 전부 리소스화 시켜서 만들어서 관리할것
		_float3						vPosition;
		_float3						vRotation;
		_float3						vScale;

		_bool						bIsLerp;
		_float3						vEndPosition;
		_float3						vEndRotation;
		_float3						vEndScale;

		_bool						bIsReverse;
		EFFECT_TYPE					eType;
		EFFECT_BELND_MODE			eBlend_Mode;

		_float						fLifeTime; //이펙트 생명시간
		_float						fRateTime; //이펙트 진행 시간

		WCHAR						DiffuseTexturePath[MAX_PATH];   // 이펙트 디퓨즈 텍스처 프로토타입 이름
		WCHAR						NormalTexturePath[MAX_PATH];    // 이펙트 노말 텍스처 프로토타입 이름;

		// 노이즈맵이 있을때 사용
		_float2						fUvRateTime;                   // 이펙트에서 UV를 이동하여 표현해야할때 사용
		_float						fNoiseStength;
		EFFECT_DISTOTION_TYPE		eDistotionType;					// 노이즈 맵등을 이용하여 텍스처를 변경할때 UV의 흐름을 결정할 타입
		WCHAR						DistotionTexturePath[MAX_PATH]; // 이펙트 디스토션 텍스처 프로토타입 이름;

		// 마스킹을 위한 텍스처
		EFFECT_MASK_TYPE			eMaskType;						// 이펙트 마스킹 타입
		WCHAR						MaskTexturePath[MAX_PATH];      // 이펙트 마스킹 텍스처 프로토타입 이름

		WCHAR						ShaderTag[MAX_PATH];      // 이펙트 마스킹 텍스처 프로토타입 이름
		WCHAR						ModelTag[MAX_PATH];      // 이펙트 마스킹 텍스처 프로토타입 이름
		
		_float4						vColor;    // 이펙트 색상 정보;
		_bool						bIsLoop;   // 반복재생 유무
	}EFFECT_NETWORK_DESC;

}
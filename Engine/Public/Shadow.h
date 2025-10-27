#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CShadow : public CBase
{
private:
	CShadow();
	virtual ~CShadow() = default;

public:
	HRESULT Ready_Shadow_Light(const SHADOW_LIGHT_DESC& Desc);
	HRESULT Bind_Shader_Resource(class CShader* pShader, const _char* pConstantName, MAT_STATE eType);

	//0 : Far,  1 : Near, 2 : FOV
	HRESULT Bind_Shader_RawResource(class CShader* pShader, const _char* pConstantName, _uInt iType);

private:
	SHADOW_LIGHT_DESC			m_CameraDesc = {};
	_float4x4					m_TransformationMatrices[ENUM_CLASS(MAT_STATE::END)] = {};

public:
	static CShadow*				Create();
	virtual void				Free() override;
};
NS_END
#pragma once
#include "Client_Define.h"
#include "GameObject.h"
#include "Light.h"

NS_BEGIN(Engine)
class CVIBuffer_Point;
class CTexture;
class CShader;
class CLight;
NS_END

NS_BEGIN(Client)

class CSunLight : public CGameObject
{
public :
	typedef struct SunLightDesc
	{
		_float			fRadius;

	}SUN_LIGHT_DESC;

	enum class	DAY_TYPE { MORNING, LUNCH, NIGHT, DAWN, END};
	static	_float3		vSunPositions[ENUM_CLASS(DAY_TYPE::END)];

protected:
	CSunLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSunLight(const CSunLight& rhs);
	virtual ~CSunLight() = default;

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
	const		_float4						GetDireaction() const; 

protected:
	virtual		HRESULT						Apply_ConstantShaderResources() override;

private :
	CVIBuffer_Point*						m_pVIBufferCom = nullptr;
	CTexture*								m_pTextureCom = nullptr;
	CShader*								m_pShaderCom = nullptr;

	CLight*									m_pDireactionLight = nullptr;
	_float									m_fAccTime = {};

	CLight::LIGHT_DESC 						m_LightDesc[ENUM_CLASS(DAY_TYPE::END)] = {};

	_float									DayLengthTimes[ENUM_CLASS(DAY_TYPE::END)] = {};
	_float3									DayAngle = {};

	DAY_TYPE								m_eDayType = {};
	_float									m_fRadius = {};

private:
	HRESULT									ADD_Components();
	void									Compute_Time(_float fDeltaTime);

public:
	static			CSunLight*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
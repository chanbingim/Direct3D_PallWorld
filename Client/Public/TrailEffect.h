#pragma once
#include "SpriteEffect.h"

NS_BEGIN(Client)

typedef struct TrailData
{
	_float4x4		WorldMat;

	_float3			vLifeTime;
};

class CTrailEffect : public CSpriteEffect
{
protected:
	CTrailEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrailEffect(const CTrailEffect& rhs);
	virtual ~CTrailEffect() = default;

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
	ID3D11Buffer				


public:
	static			CTrailEffect*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
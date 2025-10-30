#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CTrailComponent : public CComponent
{
public:
	typedef struct tagTrailHighLow {
		_float4		vHigh;
		_float4		vLow;
	}TRAILHIGHLOW;

protected:
	CTrailComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrailComponent(const CTrailComponent& rhs);
	virtual ~CTrailComponent() = default;

private:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(void* pArg) override;

public:
	void									Initialize_Trail();
	void									Update_Trail(_matrix matCurrentWorld, _bool bMakeTrail = false);

	virtual HRESULT							Render() override;
	_bool									IsRenderable();

private:
	_float4									m_vHigh = { 0.f, 1.f, 0.f, 1.f };
	_float4									m_vLow = { 0.f, 0.f, 0.f, 1.f };
	ID3D11Buffer*							m_pVB = { nullptr };
	ID3D11Buffer*							m_pIB = { nullptr };
	VTX_TEX*								m_pVTXPOSTEXs = {};
	_uInt									m_iNumPresent = {};
	_uInt									m_iEndIndex = {};
	_uInt									m_iNumVertices = {};
	_uInt									m_iNumIndices = {};

	_float4									m_vPreHighPositions[3]{};
	_float4									m_vPreLowPositions[3]{};


public:
	static	CTrailComponent*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;

};
NS_END
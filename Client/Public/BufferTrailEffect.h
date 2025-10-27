#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CTrail final :	public CComponent
{
public:
	typedef struct tagTrailHighLow {
		_float4		vHigh;
		_float4		vLow;
	}TRAILHIGHLOW;
protected:
	CTrail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrail(const CTrail& rhs);
	virtual ~CTrail() = default;

public:
	void Initialize_Trail();
	void Update_Trail(_fmatrix matCurrentWorld, _bool bMakeTrail = false);
	virtual HRESULT Render() override;
	_bool IsRenderable();

private:
	_float4			m_vHigh = { 0.f, -1.f, 0.f, 1.f };
	_float4			m_vLow = { 0.f, 0.f, 0.f, 1.f };
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };
	VTXPOSTEX* m_pVTXPOSTEXs = {};
	_uint			m_iNumPresent = {};
	_uint			m_iEndIndex = {};
	_uint			m_iNumVertices = {};
	_uint			m_iNumIndices = {};

	_float4			m_vPreHighPositions[3]{};
	_float4			m_vPreLowPositions[3]{};
private:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CTrail*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CTrail*				Clone(void* pArg, class CGameObject* pOwner = nullptr) override;
	virtual void				Free() override;
	virtual void				Describe_Entity() override;
};

NS_END
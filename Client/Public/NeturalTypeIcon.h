#pragma once

#include "TypeIcon.h"

NS_BEGIN(Client)
class CNeturalTypeIcon : public CTypeIcon
{
protected:
	CNeturalTypeIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNeturalTypeIcon(const CNeturalTypeIcon& rhs);
	virtual ~CNeturalTypeIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

protected:
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private:
	_float4x4								m_CombinedMat = {};

public:
	static		CNeturalTypeIcon*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
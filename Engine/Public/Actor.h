#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CActor abstract : public CGameObject
{
protected :
	CActor(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CActor(const CActor& rhs);
	virtual ~CActor() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype();
	virtual		HRESULT						Initialize(void* pArg);

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime);
	virtual		void						Update(_float fDeletaTime);
	virtual		void						Late_Update(_float fDeletaTime);

	// 랜더
	virtual		HRESULT						Render();

public:
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
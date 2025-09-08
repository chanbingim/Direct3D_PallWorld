#pragma once

#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)
class CPalSpher : public CPartObject
{
private :
	CPalSpher(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalSpher(const CPalSpher& rhs);
	virtual ~CPalSpher() = default;
	
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
	_bool									m_bIsPlayAnim = false;

private :
	HRESULT									ADD_Components();

public :
	static		CPalSpher*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;
};
NS_END
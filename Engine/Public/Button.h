#pragma once
#include "UserInterface.h"

NS_BEGIN(Engine)
class ENGINE_DLL CButton : public CUserInterface
{
protected :
	CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CButton(const CButton& rhs);
	virtual ~CButton() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

public:
	static		CButton*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;		
	virtual		void						Free() override;

};
NS_END
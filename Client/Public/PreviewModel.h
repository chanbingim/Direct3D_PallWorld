#pragma once
#include "Client_Define.h"
#include "ContainerObject.h"

NS_BEGIN(Client)
class CPreivewModelBody;

class CPreviewModel : public CContainerObject
{
protected:
	CPreviewModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPreviewModel(const CPreviewModel& rhs);
	virtual ~CPreviewModel() = default;

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
	CPreivewModelBody*						m_pAnimator = nullptr;
	_string									m_szAnimName = {};

private :
	HRESULT									ADD_PartObjects();
	const char*								GetWeaponName();

public:
	static			CPreviewModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END;
#pragma once
#include "Client_Define.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollision;
NS_END

NS_BEGIN(Client)
class CDebugBoundBox : public CGameObject
{
protected:
	CDebugBoundBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDebugBoundBox(const CDebugBoundBox& rhs);
	virtual ~CDebugBoundBox() = default;

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

protected:
	CCollision*								m_pCollision = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static			CDebugBoundBox*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;
};
NS_END
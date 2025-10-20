#pragma once
#include "NoneAnimMesh.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CBossMap : public CNoneAnimMesh
{
protected:
	CBossMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossMap(const CBossMap& rhs);
	virtual ~CBossMap() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render();

protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex) override;

private:
	HRESULT									ADD_Components();

public:
	static			CBossMap*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END
#pragma once
#include "Client_Define.h"
#include "NoneAnimMesh.h"

NS_BEGIN(Client)
class CSkyBox : public  CNoneAnimMesh
{
protected:
	CSkyBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox() = default;

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

protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex) override;

private :
	HRESULT									ADD_Components();

public:
	static			CSkyBox*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
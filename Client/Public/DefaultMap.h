#pragma once
#include "NoneAnimMesh.h"

#ifdef _DEBUG
#include "NavigationStruct.h"
#endif // _DEBUG


NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CDefaultMap : public CNoneAnimMesh
{
protected:
	CDefaultMap(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CDefaultMap(const CDefaultMap& rhs);
	virtual ~CDefaultMap() = default;

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
	static			CDefaultMap*			Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
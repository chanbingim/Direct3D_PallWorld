#pragma once
#include "NoneAnimMesh.h"

#ifdef _DEBUG
#include "NavigationStruct.h"
#endif // _DEBUG


NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Client)
class CBossMap : public CNoneAnimMesh
{
protected:
	CBossMap(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
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
#ifdef _DEBUG
	void									GetAllNaviMeshTriangle(list<NAVI_TRIANGLE>* pOut);
	_bool									m_bViewMesh = true;
	_uInt									m_iDrawTriCount = 0;
	_float3									m_CrateTriangle[3] = {};
#endif // _DEBUG

protected:
	virtual		HRESULT						Bind_ShaderResources() override;
	virtual		HRESULT						Apply_ConstantShaderResources(_uInt iMeshIndex) override;

private:
	_bool									m_bIsPicking = false;
	_uInt									m_iTerrainCnt = {};

	list<_uInt>								m_MapRenderIndex = {};
	CNavigation*							m_pNavigationCom = { };

private:
	HRESULT									ADD_Components();

#ifdef _DEBUG
	void									PickingNavimesh();
	void									PickingPoint();
#endif // _DEBUG

public:
	static			CBossMap*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
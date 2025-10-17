#pragma once
#include "NoneAnimMesh.h"

NS_BEGIN(Client)
class CEnviormnent : public CNoneAnimMesh
{
public :
	enum class ENVIORNMENT_TYPE {TREE, ROCK, PELLSTONE, END};
	
	typedef struct Enviornment_Dsec : GAMEOBJECT_DESC
	{
		_uInt			iModelIndex;

	}ENVIORMNENT_DESC;

protected :
	CEnviormnent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEnviormnent(const CEnviormnent& rhs);
	virtual ~CEnviormnent() = default;

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

protected :
	

public:
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
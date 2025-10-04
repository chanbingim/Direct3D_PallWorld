#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Client)
class CIngredientUI;

class CIngredientList : public CUserInterface
{
public :
	typedef struct IngredientListDesc : public GAMEOBJECT_DESC
	{
		_uInt			iIngredientCount;
	}INGREDIENT_LIST_DESC;

protected:
	CIngredientList(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CIngredientList(const CIngredientList& rhs);
	virtual ~CIngredientList() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private:
	_uInt									m_iNumIngredient = {};
	vector<CIngredientUI*>					m_Ingredients = {};

private:
	HRESULT									ADD_Components();

public:
	static			CIngredientList*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;
};
NS_END
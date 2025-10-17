#pragma once

#include "ImgUIBase.h"

NS_BEGIN(Engine)
class CGameObject;
NS_END

NS_BEGIN(Client)
class CImgManager;

class CIMG_Hierarchy : public CImgUIBase
{
private :
	CIMG_Hierarchy(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CIMG_Hierarchy() = default;

public :
	virtual HRESULT					Prototype_Initialize() override;
	virtual HRESULT					Initialize(void* pArg) override;

	virtual void					Update(_float fDeletaTime) override;

private :
	CImgManager*					m_pImgManager = nullptr;
	list<CGameObject*>				m_pParentNodes = {};
	ImGuiTreeNodeFlags				m_TreeNodeFlag = {};

	_bool							m_bIsMovePickingPos = {};
	_char							m_szName[MAX_PATH] = {};

private :
	void							Update_ParentNodes();
	HRESULT							Draw_Hierarchy(CGameObject* pObject);

public:
	static	CIMG_Hierarchy*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CImgUIBase*				Clone(void* pArg);
	virtual void					Free() override;

};
NS_END
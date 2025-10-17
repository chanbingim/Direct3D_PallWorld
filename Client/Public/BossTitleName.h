#pragma once
#include "Client_Define.h"
#include "UserInterface.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTypeIcon;

class CBossTitleName : public CUserInterface
{
protected:
	CBossTitleName(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBossTitleName(const CBossTitleName& rhs);
	virtual ~CBossTitleName() = default;

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
	CTypeIcon*								m_pTypeIcon = nullptr;
	CFontComponent*							m_pFontCom = nullptr;

	_float2									m_TextFontPos = {};
	_wstring								m_szText = {};

private:
	HRESULT									ADD_Components();

public:
	static			CBossTitleName*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
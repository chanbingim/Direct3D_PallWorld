#pragma once
#include "BackGround.h"
#include "TechStruct.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CTechTabIcon;

class CTechCategoryUI : public CBackGround
{
public :
	typedef struct TechCategoryDesc : public GAMEOBJECT_DESC
	{
		TECH_TYPE		CategoryType;
	}TECH_CATEGORY_DESC;

private:
	CTechCategoryUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTechCategoryUI(const CTechCategoryUI& rhs);
	virtual ~CTechCategoryUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetSelect(_bool bFlag) { m_bIsSelect = bFlag; }

	void									Bind_EventFunction(function<void(_uInt)> ClieckEvent);
	const	TECH_TYPE&						GetTechType() { return m_eTechButtonType; }

protected:
	virtual		void						MouseHoverEnter();
	virtual		void						MouseHoverExit();

	virtual		void						MouseButtonDwon();

private:
	//제작 UI
	TECH_TYPE								m_eTechButtonType = {};
	function<void(_uInt)>					m_ClickEvent = nullptr;

	CFontComponent*							m_pFontCom = nullptr;
	_wstring								m_pTpyeName = {};
	_float2									m_fFontPoint = {};

	CTechTabIcon*							m_pTechTabIcon = nullptr;
	_bool									m_bIsSelect = false;

private:
	HRESULT									ADD_Components();

public:
	static		CTechCategoryUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg);
	virtual		void						Free() override;

};
NS_END
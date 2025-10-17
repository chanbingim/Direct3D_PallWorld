#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
NS_END

NS_BEGIN(Client)
class CEventButton;

class CSelectUI : public CBackGround
{
private:
	CSelectUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSelectUI(const CSelectUI& rhs);
	virtual ~CSelectUI() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									BindEvent(_uInt iButtonType, function<void()> bindEvent) const;
	
	void									SetText(_uInt iButtonType, const WCHAR* szText) const;
	void									SetPosition(const _float3& vPos);

private:
	/* 구성 요소 */
	CEventButton*							m_pCancelButton = nullptr;
	CEventButton*							m_pOKButton = nullptr;

private:
	HRESULT									ADD_Childs();

public:
	static		CSelectUI*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
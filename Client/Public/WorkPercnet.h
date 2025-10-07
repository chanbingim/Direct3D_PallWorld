#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CFontComponent;
class CActor;
NS_END

NS_BEGIN(Client)
class CWorkPercentBar;

class CWorkPercnet : public CBackGround
{
private:
	CWorkPercnet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkPercnet(const CWorkPercnet& rhs);
	virtual ~CWorkPercnet() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

private :
	CWorkPercentBar*						m_pWorPercentBar = nullptr;

private:
	HRESULT									ADD_Components();
	HRESULT									ADD_Childs();

public:
	static		CWorkPercnet*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
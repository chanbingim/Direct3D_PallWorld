#pragma once

#include "BackGround.h"

NS_BEGIN(Client)
class CPellInfoFrame : public CBackGround
{
protected:
	CPellInfoFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPellInfoFrame(const CPellInfoFrame& rhs);
	virtual ~CPellInfoFrame() = default;

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
	HRESULT									ADD_Components();

public:
	static		CPellInfoFrame*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
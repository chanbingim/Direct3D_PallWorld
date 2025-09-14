#pragma once

#include "BackGround.h"

NS_BEGIN(Engine)
class CTexture;
NS_END;

NS_BEGIN(Client)
class CTypeIcon : public CBackGround
{
protected:
	CTypeIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTypeIcon(const CTypeIcon& rhs);
	virtual ~CTypeIcon() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetData(_uInt iID);

protected :
	CTexture*								m_pBackGroundTex = nullptr;
	_uInt									m_iTypeID = {};

private:
	HRESULT									ADD_Components();

public:
	static		CTypeIcon*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
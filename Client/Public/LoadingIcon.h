#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
NS_END

NS_BEGIN(Client)
class CLoadingIcon : public CBackGround
{
protected:
	CLoadingIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoadingIcon(const CLoadingIcon& rhs);
	virtual ~CLoadingIcon() = default;

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
	CTexture*								m_pFrontTexture = nullptr;
	CTransform*								m_pFrontTransform = nullptr;

	_bool									m_bIsRotate = false;
	_float									m_fRadius = {};
	_float									m_fSpeed = 35.f;

private:
	HRESULT									ADD_Components();
	HRESULT									Aplly_FrontObejctShaderResources();

public:
	static		CLoadingIcon*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
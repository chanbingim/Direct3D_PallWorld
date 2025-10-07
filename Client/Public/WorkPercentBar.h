#pragma once
#include "Client_Define.h"
#include "ProgressBar.h"

NS_BEGIN(Client)
class CWorkPercentBar final : public CProgressBar
{
private:
	CWorkPercentBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkPercentBar(const CWorkPercentBar& rhs);
	virtual ~CWorkPercentBar() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	void									SetPosition(_float3 vPosition);

protected:
	virtual     HRESULT						Apply_ConstantShaderResources() override;

private:
	HRESULT									ADD_Components();

public:
	static		CWorkPercentBar*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CGameObject*				Clone(void* pArg) override;
	virtual		void						Free() override;
};
NS_END
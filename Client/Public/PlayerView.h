#pragma once
#include "Client_Define.h"
#include "Viewer.h"

NS_BEGIN(Client)
class CPlayerView : public CViewer
{
private :
	CPlayerView(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayerView(const CPlayerView& rhs);
	virtual ~CPlayerView() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources();

private :
	HRESULT									ADD_Components();

public :
	static		CPlayerView*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual		CUserInterface*				Clone(void* pArg) override;
	virtual		void						Free() override;

};
NS_END
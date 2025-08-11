#pragma once

#include "Client_Define.h"
#include "Client_ImgDefines.h"
#include "Base.h"

NS_BEGIN(Client)
class CImgManager final : public CBase
{
private :
	CImgManager() = default;
	virtual ~CImgManager() = default;

public :
	HRESULT			Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void			Update(_float fDeletaTime);
	void			Render();

private :
	// Img gui를 여기다가 만들어서 보관하고 그걸 통해서 제어

public :
	static			CImgManager*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			void			Free() override;

};
NS_END
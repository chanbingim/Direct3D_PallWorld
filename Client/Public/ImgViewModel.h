#pragma once
#include "Client_Define.h"
#include "PartObject.h"

NS_BEGIN(Client)

class CImgViewModel : public CPartObject
{
protected:
	CImgViewModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CImgViewModel(const CImgViewModel& rhs);
	virtual ~CImgViewModel() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

	void									Bind_Model(CModel* pModel);
	void									SetAnimationIndex(_uInt iIndex);

private:
	HRESULT									ADD_Components();

public:
	static			CImgViewModel*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			void					Free() override;
};
NS_END
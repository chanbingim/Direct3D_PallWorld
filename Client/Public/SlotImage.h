#pragma once
#include "BackGround.h"

NS_BEGIN(Engine)
class CTexture;
NS_END

NS_BEGIN(Client)
class CSlotImage : public CBackGround
{
private:
	CSlotImage(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CSlotImage(const CSlotImage& rhs);
	virtual ~CSlotImage() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;
	//업데이트	
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;
	// 랜더
	virtual		HRESULT						Render() override;

	//텍스처의 정보를 얻어와서 세팅
	//텍스처의 정보가 바뀌면 호출해서 텍스처 정보를 반환
	void									ChangeTextureData(CTexture* pTexture);

private :
	HRESULT									ADD_Components();

public:
	static			CSlotImage*				Create(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;


};
NS_END
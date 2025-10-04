#pragma
#include "BackGround.h"

NS_BEGIN(Client)
class CTitleUI;
class CCreateToolTipUI;

class CWorkBenchCreateUI : public CBackGround	
{
protected:
	CWorkBenchCreateUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorkBenchCreateUI(const CWorkBenchCreateUI& rhs);
	virtual ~CWorkBenchCreateUI() = default;

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
	CTitleUI*								m_pTitle = nullptr;
	CCreateToolTipUI*						m_pCreateToolTip = nullptr;

private:
	HRESULT									ADD_Components();

public:
	static			CWorkBenchCreateUI*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
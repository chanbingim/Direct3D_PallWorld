#pragma once
#include "Architecture.h"

NS_BEGIN(Client)
class CPalBox : public CArchitecture
{
public :
	typedef struct		PalBoxDesc
	{
		_float			fAreaRadius;
	}PALBOX_DESC;

protected:
	CPalBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPalBox(const CPalBox& rhs);
	virtual ~CPalBox() = default;

public:
	//초기화
	virtual		HRESULT						Initalize_Prototype() override;
	virtual		HRESULT						Initialize(void* pArg) override;

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime) override;
	virtual		void						Update(_float fDeletaTime) override;
	virtual		void						Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT						Render() override;

protected :
	virtual void							HitOverlapFunction(_float3 vDir, CGameObject* pHitActor) override;

private :
	_float									m_fAreaRadius = {};

private :
#ifdef _DEBUG
	CCollision*								m_pDebugSphere = nullptr;
#endif // _DEBUG

private :
	HRESULT									ADD_Components();

public:
	static			CPalBox*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual			CGameObject*			Clone(void* pArg) override;
	virtual			void					Free() override;

};
NS_END
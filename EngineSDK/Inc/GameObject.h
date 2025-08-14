#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CTransform;
class CGameInstance;
class CComponent;

class ENGINE_DLL CGameObject abstract : public CBase
{
public :
	enum class OBJECT_TYPE { STATIC, DYNAMIC };

	typedef struct GameObject_Desc
	{
		CGameObject*	pParent;

		_float3			vPosition;
		_float3			vRotation;
		_float3			vScale;

	}GAMEOBJECT_DESC;

protected :
	CGameObject(ID3D11Device* pGraphic_Device, ID3D11DeviceContext* pDeviceContext);
	CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public :
	//초기화
	virtual		HRESULT						Initalize_Prototype();
	virtual		HRESULT						Initialize(void* pArg);

	//업데이트	
	virtual		void						Priority_Update(_float fDeletaTime);
	virtual		void						Update(_float fDeletaTime);
	virtual		void						Late_Update(_float fDeletaTime);

	// 랜더
	virtual		HRESULT						Render();

	CComponent*								Find_Component(const _wstring& ComponentTag);

	_bool									IsDead() { return m_IsDead; }
	void									SetDead(_bool bflag);

	CTransform*								GetTransform() { return m_pTransformCom; }

	CGameObject*							GetParent() { return m_pParent; }
	void									SetParent(CGameObject* pParent);

	void									ADD_Child(CGameObject* pChild);
	void									Remove_Child(CGameObject* pChild);
	const list<CGameObject*>*				GetChildObject();

	void									SetLocation(_float3 vPosition);
	void									SetRotation(_float3 vRotation);
	void									SetScale(_float3 vScale);

	void									ADDPosition(_vector vAddPos);
	void									ADDRotation(_vector vAxis, _float fTurnSpeed, _float fTimeDeleta);

	void									SetObjectTag(const WCHAR* szTag);
	const _wstring&							GetObjectTag();

protected:
	_wstring								m_ObejctTag = {};

	ID3D11Device*							m_pGraphic_Device = nullptr;
	ID3D11DeviceContext*					m_pDeviceContext = nullptr;
	CGameInstance*							m_pGameInstance = nullptr;

	//Obejct & heirarchy
	CGameObject*							m_pParent = nullptr;
	list<CGameObject*>						m_pChildList;

	_float4x4								m_AccParentPos = {};

	//Compoent
	CTransform*								m_pTransformCom = nullptr;

	VISIBILITY								m_eVisible = VISIBILITY::VISIBLE;

	unordered_map<_wstring, CComponent*>	m_pComponentMap;
	_bool									m_IsDead = false;
	OBJECT_TYPE								m_eType = OBJECT_TYPE::DYNAMIC;

#pragma region  Default Shader Resources
	ID3DX11EffectMatrixVariable*			m_pEMVWorldMat = nullptr;
	ID3DX11EffectMatrixVariable*			m_pEMVViewMat = nullptr;
	ID3DX11EffectMatrixVariable*			m_pEMVProjMat = nullptr;
#pragma endregion

protected :
	virtual     HRESULT						Bind_ShaderResources();
	virtual     HRESULT						Apply_ConstantShaderResources();

	//컴포넌트 추가 및 찾기
	HRESULT									Add_Component(_uInt iLevelIndex, const _wstring& _PrototypeTag, const _wstring& ComponentTag, CComponent** ppOut, void* pArg = nullptr);
	void									Update_ParentMatrix();

private :
	_bool									m_IsMoveParentTransform = false;

public :
	virtual			CGameObject*			Clone(void* pArg);
	virtual			void					Free() override;

};
NS_END
#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CEffectContatiner : public CGameObject
{
public :
#ifdef _DEBUG
	typedef struct EffectSave_Desc
	{
		const char*					szFilePath;
	}EFFECT_SAVE_SESC;
#endif // _DEBUG

protected:
	CEffectContatiner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffectContatiner(const CEffectContatiner& rhs);
	virtual ~CEffectContatiner() = default;

public:
	//초기화
	virtual		HRESULT										Initalize_Prototype() override;
	virtual		HRESULT										Initalize_Prototype(const char* pFilePath);

	virtual		HRESULT										Initialize(void* pArg) override;

	//업데이트	
	virtual		void										Priority_Update(_float fDeletaTime) override;
	virtual		void										Update(_float fDeletaTime) override;
	virtual		void										Late_Update(_float fDeletaTime) override;

	// 랜더
	virtual		HRESULT										Render() override;

#ifdef _DEBUG
	void													ADD_EffectPartObject(const wstring& EffectPartTag, void* pArg);
	void													Remove_EffectPartObject(const WCHAR* EffectPartTag);
	virtual void											ExportData(void* pArg) override;
#endif // _DEBUG
	CGameObject*											FindPartObject(const WCHAR* szTag);
	const 	unordered_map<_wstring, CGameObject*>*			GetAllPartObejcts() { return &m_PartObjects; }

protected:
	const _float4x4*										m_SocketMatrix = nullptr;
	unordered_map<_wstring, CGameObject*>					m_PartObjects;

protected :
	HRESULT													ReadFileData(const char* szFilePath);
	HRESULT													Bind_PartObject();

public:
	static			CEffectContatiner*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static			CEffectContatiner*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const char* pFilePath);

	virtual			CGameObject*							Clone(void* pArg) override;
	virtual			void									Free() override;

};
NS_END
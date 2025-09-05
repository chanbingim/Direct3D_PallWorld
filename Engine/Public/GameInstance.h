#pragma once
#include "Prototype_Manager.h"

NS_BEGIN(Engine)
class CGraphic_Device;
class CMouse;
class CLevel_Manager;
class CObject_Manager;
class CLevel;
class CPicking;
class CLayer;
class CGameObject;
class CPipeline;
class CUserInterface;
class CHeadUpDisplay;
class CTimer_Manager;
class CRenderer;
class CPrototype_Manager;
class CHeadUpDisplay;
class CInputManager;
class CSound_Manager;
class CLightManager;
class CLight;

class ENGINE_DLL CGameInstance : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

	typedef struct GameInstance_Desc
	{
		ENGINE_DESC				EngineSetting;
		ID3D11Device**			ppDevice;
		ID3D11DeviceContext**	ppContext;

	}GAMEINSTANCE_DESC;

private :
	CGameInstance() = default;
	virtual ~CGameInstance() = default;

public :
	HRESULT		Initialize_Engine(void* pArg);
	void		Update_Engine(_float fTimeDelta);

	// DefaultRender
	HRESULT		Draw();

	// Only Renderer Draw
	HRESULT		DrawRender();

	// GrpahicDevice
#pragma region Graphic_Device
	HRESULT		ADD_Window(const ENGINE_DESC& Win_Desc);
	void		Set_RenderResource(_uInt iIndex);
	HRESULT		GetBackBuffer(ID3D11Texture2D** pOut);
	_uInt		GetRenderTargetNum();

	void		Render_Begin(_float* Color);
	void		Render_End();
#pragma endregion

#pragma region Level_Manager
	HRESULT						Change_Level(class CLevel* pNewLevel);
	CLevel*						GetCurrentLevel();
	CHeadUpDisplay*				GetCurrentHUD();
#pragma endregion

#pragma region Object_Manager
	HRESULT									Add_GameObject_ToLayer(_uInt iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uInt iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	const list<CGameObject*>*				GetAllObejctToLayer(_uInt iLayerIndex, const _wstring& LayerTag);

	
	void									Clear_DeadObject();
	const unordered_map<_wstring, CLayer*>*	GetCurLevelLayer();

#pragma endregion

#pragma region Prototype_Manager
	HRESULT						Add_Prototype(_uInt iLevelNum, const _wstring& PrototypeTag, CBase* pPrototype);
	void						Clear_Resource(_uInt iLevelIndex);

	template<typename T>
	T* Clone_Prototype(_uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg);

	template<typename T>
	void						GetPrototypeList(_uInt iLevelIndex, list<pair<_wstring, T*>>* pOutList);

	void						GetPrototypeName(const char* classTypeName, _string& OutName);
	_uInt						GetPrototypeLevel(const _wstring& PrototpyeName);
#pragma endregion

#pragma region Timer_Manager
	HRESULT						Add_Timer(const _wstring& strTimerTag);
	_float						Get_TimeDelta(const _wstring& strTimerTag, _bool bFlag = true);
#pragma endregion

#pragma region Renderer
	HRESULT						Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region Input Manager
	//마우스 입력 0 : 왼쪽 , 1 : 오른쪽 , 2 가운데 버튼
	_bool		KeyDown(KEY_INPUT eType, _uInt KeyState);

	//마우스 입력 0 : 왼쪽 , 1 : 오른쪽 , 2 가운데 버튼
	_bool		KeyPressed(KEY_INPUT eType, _uInt KeyState);

	//마우스 입력 0 : 왼쪽 , 1 : 오른쪽 , 2 가운데 버튼
	_bool		KeyUp(KEY_INPUT eType, _uInt KeyState);

	// 0 : X 축    1  : Y 축    2 : Z축
	LONG		GetMouseAxis(_uInt iAxis);
#pragma endregion

#pragma region Sound Manager
	void Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void Manager_StopSound(CHANNELID eID);
	void Manager_StopAll();
	void Manager_SetChannelVolume(CHANNELID eID, float fVolume);
#pragma endregion

#pragma region Mouse
	HRESULT						SetMouseTexture(_uInt iLevelIndex, const WCHAR* Proto_TexTag, const WCHAR* ComTex_Tag, void* pTexArg,
												const WCHAR* Proto_ShaderTag, const WCHAR* ComShader_Tag, void* pShaderArg,
												const WCHAR* Proto_BufferTag, const WCHAR* ComBuffer_Tag, void* pBufferArg);

	POINT&						GetMousePoint();

	void						SetDrag(_bool flag);
	BOOL						IsMouseDrag();

	void						SetMouseFocus(CUserInterface* Widget);
	BOOL						IsMouseFocus(CUserInterface* Widget);

	_float3&					GetRayPos();
	_float3						GetMouseWorldPos();

	void						ResetMouseData();
#pragma endregion

#pragma region PipeLine
	void						SetMatrix(MAT_STATE eState, _float4x4 Matrix);

	const _float4x4&			GetMatrix(MAT_STATE eState);
	const _float4x4&			GetInvMatrix(MAT_STATE eState);
	const _float4x4&			GetIndentityMatrix();

	void						SetPostBuffer(_uInt iIndex, ID3D11ShaderResourceView* pTex);
	ID3D11ShaderResourceView*	GetPostBuffer(_uInt iIndex);
#pragma endregion

#pragma region Editor_Picking
	void						SetEditor_MousePos(_float3 vPos);
	void						SetEditor_Frame(const _float2& vSize);
	void						Change_Mode(GAMEMODE eType);
	const	GAMEMODE&			GetGameMode();

	const _float3&				GetPickingRayPos(RAY eType);
	const _float3&				GetPickingRayDir(RAY eType);
	void						Compute_LocalRay(const _matrix* InvWorldMatrix);

	_bool						RayPicking(_vector vRayOrizin, _vector vRayDir, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool						Picking_InWorld(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool						Picking_InLocal(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
#pragma endregion

#pragma region Game_Instance
	_float						GetPipeLineLoopTime(const TCHAR* Str);
	const _float2&				GetScreenSize();
#pragma endregion

#pragma region LIGHT_MANAGER
	void						ADDLight(CLight* pLight);
	const CLight*				GetLight(_uInt iIndex);
#pragma endregion


private :
	CGraphic_Device*			m_pGraphic_Device = nullptr;
	CLevel_Manager*				m_pLevel_Manager = nullptr;
	CObject_Manager*			m_pObject_Manager = nullptr;
	CPrototype_Manager*			m_pPrototype_Manager = nullptr;
	CTimer_Manager*				m_pTimer_Manager = nullptr;
	CRenderer*					m_pRenderer = nullptr;
	CInputManager*				m_pInput_Manager = nullptr;
	CSound_Manager*				m_pSound_Manager = nullptr;
	CMouse*						m_pMouse = nullptr;
	CPipeline*					m_pPipeline = nullptr;
	CPicking*					m_pPicking = nullptr;
	CLightManager*				m_pLightManager = nullptr;

	_float2						m_ScreenSize = {};

public :
	void						Release_Engine();
	virtual		void			Free() override;

};
NS_END

template<typename T>
inline T* CGameInstance::Clone_Prototype(_uInt iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype<T>(iLevelIndex, strPrototypeTag, pArg);
}

template<typename T>
inline void CGameInstance::GetPrototypeList(_uInt iLevelIndex, list<pair<_wstring, T*>>* pOutList)
{
	m_pPrototype_Manager->GetPrototypeList<T>(iLevelIndex, pOutList);
}

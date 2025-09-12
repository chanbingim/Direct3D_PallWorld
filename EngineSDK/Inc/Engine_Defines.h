#pragma once
#ifndef Engine_Define_h__
#define Engine_Define_h__

//WIN
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <wincodec.h>

//FILE Input
#include <fstream>

//STL
#include <set>
#include <stack>
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include <queue>
#include <functional>
#include <string>
#include <bitset>

using namespace std;

//FMode
#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
#include <io.h>

//Dx11
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <d3dcompiler.h>

//Dx11 Texture Headers
#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/ScreenGrab.h"

//Dx11 Shader
#include "FX11/d3dx11effect.h"

//Assimp
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

using namespace Assimp;
using namespace DirectX;

#pragma warning(disable : 4251)

#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Math.h"

#include <dinput.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace Engine;
#endif // Engine_Define_h__
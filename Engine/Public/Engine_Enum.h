#pragma once

namespace Engine
{
	enum class WINMODE		{ FULL, WIN, END };
	enum class GAMEMODE		{ EDITOR, GAME, END };
	enum class KEY_INPUT    { KEYBOARD, MOUSE, END };
	enum class MODEL_TYPE	{ ANIM, NONANIM, END };
	enum class VISIBILITY	{ VISIBLE, HIDDEN, END };
	enum class WORLDSTATE	{ RIGHT, UP, LOOK, POSITION, END };
	enum class OBJECT_ID	{ GAMEOBJECT, COMPONENT, END };

	enum class RENDER {
		PRIORITY,
		NONBLEND,
		BLEND,
		WORLD_UI, SCREEN_UI, END
	};

	enum class TEXTURE			{ PLANE, CUBE, END };
	enum class RAY				{ LOCAL, WORLD, END };
	enum class COLLISION_TYPE	{ BOX, SPHERE, END };
	enum class LIGHT_TYPE		{ POINT = 1, SPOT, DIRECATION, END };

	enum class MAT_STATE		{ VIEW, PROJECTION, END };
	enum class AXIS				{ HORIZONTAL, VERTICAL, END };
	enum class LIGHT			{ DIRECTION, POINT, SOPT, END };

	enum class NAVI_POINT		{ A, B, C, END };
	enum class NAVI_LINE		{ AB, BC, CA, END };

	enum CHANNELID { BGM, EFFECT, END };
}

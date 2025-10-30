#pragma once

namespace Engine
{
	enum class WINMODE		{ FULL, WIN, END };
	enum class GAMEMODE		{ EDITOR, GAME, END };
	enum class KEY_INPUT    { KEYBOARD, MOUSE, END };
	enum class MODEL_TYPE	{ ANIM, NONANIM, END };
	enum class VISIBILITY	{ VISIBLE, HIDDEN, END };
	enum class WORLDSTATE	{ RIGHT, UP, LOOK, POSITION, END };
	enum class CAMERA_INFO	{ CAMERA_FOV, CAMERA_FAR, CAMERA_NEAR, END};
	enum class OBJECT_ID	{ GAMEOBJECT, COMPONENT, END };

	enum class TEXTURE_TYPE {
		NONE = 0,
		DIFFUSE = 1,
		SPECULAR = 2,
		AMBIENT = 3,
		EMISSIVE = 4,
		HEIGHT = 5,
		NORMAL = 6,
		SHININESS = 7,
		OPACITY = 8,
		LIGHTMAP = 10,
		REFLECTION = 11,
		BASE_COLOR = 12,
		NORMAL_CAMERA = 13,
		EMISSION_COLOR = 14,
		METALNESS = 15,
		ROUGHNESS = 16,
		OCCLUSION = 17,
	};

	enum class EFFECT_TEXTURE_TYPE {
		DIFFUSE = 0,
		NORMAL = 1,
		NOISE = 2,
		MASK = 3,
		DISSOLVE = 4,
		END
	};


	enum class RENDER {
		PRIORITY,
		SHADOW,
		NONBLEND,
		NONLIGHT,
		BLUR,
		BLEND,
		WORLD_UI, SCREEN_UI, END
	};

	enum class TEXTURE			{ PLANE, CUBE, END };
	enum class RAY				{ LOCAL, WORLD, END };
	enum class COLLISION_TYPE	{ BOX, SPHERE, OBB, END };
	enum class LIGHT_TYPE		{ POINT = 1, SPOT, DIRECATION, END };

	enum class MAT_STATE		{ VIEW, PROJECTION, ORTHROGRAPHIC,END };
	enum class AXIS				{ HORIZONTAL, VERTICAL, END };
	enum class LIGHT			{ DIRECTION, POINT, SOPT, END };

	enum class NAVI_POINT		{ A, B, C, END };
	enum class NAVI_LINE		{ AB, BC, CA, END };

	enum CHANNELID { BGM, EFFECT, EFFECT2, END };
}

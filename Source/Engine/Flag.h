#pragma once

enum class ENetwork_Type
{
	Offline,
	Online
};

enum class CBV_REGISTER : unsigned char
{
	b0,
	b1,
	b2,
	b3,
	b4,

	END
};

enum class SRV_REGISTER : unsigned char
{
	t0 = static_cast<unsigned char>(CBV_REGISTER::END),
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	t7,
	t8,
	t9,

	END
};

enum class UAV_REGISTER : unsigned char
{
	u0 = static_cast<unsigned char>(SRV_REGISTER::END),
	u1,
	u2,
	u3,
	u4,

	END
};

enum class EComponent_Type
{
	Transform,
	Camera,
	Collider,
	Light,
	Animator,
	Rigidbody,
	UI,
	UIButton,
	MonsterAI,
	
	// Rendering Component
	MeshRenderer,
	ParticleSystem,
	Skybox,
	END,
	Script
};

enum class EWidgetComponent_Type
{
	Transform,
	MeshRenderer,
	END,
};

enum class EWIDGETWINDOW_TYPE
{
	START_WINDOW,

	GAME_WINDOW,
	MAP_WINDOW,

	STORE_WINDOW,
	INVENTORY_WINDOW,	

	END
};

enum class EAsset_Type
{
	Mesh,
	Texture,
	Material,
	GraphicShader,
	ComputeShader,
	FBX,
	JHDMeshData,

	END
};

enum class EProjection_Type
{
	Perspective,
	Orthographic,
};

enum class ECamera_Type
{
	Free,
	Fixed,
	Inventory_Interaction_Start,
	Interaction_Start,
	Interaction,
	Interaction_End,
};

// ����
enum class EDir
{
	Right,
	Up,
	Front,

	END
};

enum class ECollision_Channel
{
	Default,
	Player,
	Wall,
	Max
};

enum class ECollision_Interaction
{
	Ignore,
	Collision
};

enum class EState_Type
{
	Idle,
	Run,
	Dash,
	Attack,
	Attack_Q,
	Attack_R,
	Attack_E,
	Attack_LButton,
	Attack_RButton,
	Chase,
	Hit,
	Death,
	Knockback,
	Damaged,
	Spawn,
	Talk,
	Buy,
	Dissolve,
	Falling,
	END
};

enum class EPlayerAttribute
{
	Fire,  
	Ice,  
	Electric 
};

enum class EMonsterType
{
	Crab,
	Adc,


	END
};

enum ESkillSlot
{
	LButton,
	RButton,
	Q,
	E,
	R,
	END
};
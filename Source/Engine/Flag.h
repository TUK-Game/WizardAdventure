#pragma once

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
	
	// Rendering Component
	MeshRenderer,
	ParticleSystem,
	Skybox,

	END,

	Script
};

enum class EAsset_Type
{
	Mesh,
	Texture,
	Material,
	GraphicShader,
	ComputeShader,
	FBX,


	END
};

enum class EProjection_Type
{
	Perspective,
	Orthographic,
};

// πÊ«‚
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
	Max
};

enum class ECollision_Interaction
{
	Ignore,
	Collision
};
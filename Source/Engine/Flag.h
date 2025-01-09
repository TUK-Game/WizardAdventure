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

	END
};

enum class EComponent_Type
{
	Transform,
	Camera,
	Collider,
	
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
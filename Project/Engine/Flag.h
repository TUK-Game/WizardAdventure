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


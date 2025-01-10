#pragma once

enum class EKeyState
{
	None,
	Down,
	Push,
	Up,
};

enum class EKey
{
	Q, W, E, R, T, Y,
	A, S, D, F, G, H,
	Z, X, C, V, B, N, M,

	Enter,
	Esc,
	LShift,
	Alt,
	Ctrl,
	Space,

	Up, Down, Left, Right,

	Num0,
	Num1,
	Num2,
	Num3,
	Num4,
	Num5,
	Num6,
	Num7,
	Num8,
	Num9,

	//NumPad0,
	//NumPad1,
	//NumPad2,
	//NumPad3,
	//NumPad4,
	//NumPad5,
	//NumPad6,
	//NumPad7,
	//NumPad8,
	//NumPad9,

	LButton,
	RButton,

	Max,
};


struct KeyInfo
{
	EKeyState	State;			// 현재 상태
	bool		bPrevPressed;	// 이전 프레임에 눌렸는지
};


class CInputManager :
	public CSingleton<CInputManager>
{
	DECLARE_SINGLETON(CInputManager)

public:
	EKeyState GetKeyState(EKey key) const	{ return m_vecKeyInfo[(int)key].State; }
	Vec2 GetMousePosition() const			{ return m_MousePosition; }
	Vec2 GetMousePrevPosition() const		{ return m_MousePrevPosition; }
	Vec2 GetDragDir() const					{ return m_DragDir; }

	bool IsMouseOnClient();

public:
	int Init();
	void Update();

private:
	std::array<UINT, (int)EKey::Max>	m_arrKeyValue;
	std::vector<KeyInfo>				m_vecKeyInfo;
	Vec2								m_MousePosition;
	Vec2								m_MousePrevPosition;
	Vec2								m_DragDir;
};


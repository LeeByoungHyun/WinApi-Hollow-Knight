#include "yaInput.h"
#include "yaApplication.h"

extern ya::Application application;

namespace ya
{
	int ASCII[(UINT)eKeyCode::END] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 
		'Z', 'X', 'C', 'V', 'B', 'N', 'M', 
		VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_SPACE,
		VK_NUMPAD7, VK_NUMPAD8, VK_NUMPAD9,
		VK_NUMPAD4, VK_NUMPAD5, VK_NUMPAD6,
		VK_NUMPAD1, VK_NUMPAD2, VK_NUMPAD3, 
		VK_NUMPAD0,
		VK_LBUTTON, VK_RBUTTON,
	};

	std::vector<Input::Key> Input::mKeys;
	Vector2 Input::mMousePos = Vector2::Zero;

	void Input::Initialize()
	{
		// 키보드 키 하나하나 매칭하여 mKeys vector에 추가
		for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
		{
			Key keyInfo;
			keyInfo.key = (eKeyCode)i;
			keyInfo.state = eKeyState::None;
			keyInfo.bePressed = false;

			mKeys.push_back(keyInfo);
		}
	}

	void Input::Update()
	{
		if (GetFocus())
		{

			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (GetAsyncKeyState(ASCII[i]) & 0x8000)
				{
					// 이전 프레임에도 눌려 있었다
					if (mKeys[i].bePressed)
						mKeys[i].state = eKeyState::Pressed;
					else
						mKeys[i].state = eKeyState::Down;

					mKeys[i].bePressed = true;
				}
				else // 현재 프레임에 키가 눌려있지 않다.
				{
					// 이전 프레임에 내키가 눌려있엇다.
					if (mKeys[i].bePressed)
						mKeys[i].state = eKeyState::Up;
					else
						mKeys[i].state = eKeyState::None;

					mKeys[i].bePressed = false;
				}
			}

			POINT mousePos = {};
			GetCursorPos(&mousePos);

			ScreenToClient(application.GetHwnd(), &mousePos);
			mMousePos.x = mousePos.x;
			mMousePos.y = mousePos.y;
		}
		else
		{
			for (UINT i = 0; i < (UINT)eKeyCode::END; i++)
			{
				if (eKeyState::Down == mKeys[i].state
					|| eKeyState::Pressed == mKeys[i].state)
				{
					mKeys[i].state = eKeyState::Up;
				}
				else if (eKeyState::Up == mKeys[i].state)
				{
					mKeys[i].state = eKeyState::None;
				}

				mKeys[i].bePressed = false;
			}
		}
	}

	void Input::Render(HDC hdc)
	{

	}
}
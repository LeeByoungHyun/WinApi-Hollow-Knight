#pragma once
#include "yaScene.h"
#include "yaCuphead.h"

namespace ya
{
	class CupHead;
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		~PlayScene();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void Enter() override;
		virtual void Exit() override;

	private:
		Cuphead* mCuphead;
	};
}



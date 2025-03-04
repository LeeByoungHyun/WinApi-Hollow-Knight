#pragma once
#include "yaGameObject.h"
#include "yaImage.h"

namespace ya
{
	class Animator;
	class PlayerSkull : public GameObject
	{
	public:

		PlayerSkull();
		~PlayerSkull();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		Image* mImage;
		Animator* mAnimator;
	};
}



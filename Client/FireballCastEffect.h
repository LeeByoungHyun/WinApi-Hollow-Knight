#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Animator;
	class FireballCastEffect : public GameObject
	{
	public:
		FireballCastEffect();
		~FireballCastEffect();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		void fireballCastEffectComplateEvent();

	private:
		Animator* mAnimator;
		class Transform* tr;
		class Player* player;

		bool endFlag = false;
	};
}

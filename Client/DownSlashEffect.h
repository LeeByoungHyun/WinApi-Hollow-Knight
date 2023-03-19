#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Animator;
	class DownSlashEffect : public GameObject
	{
	public:
		DownSlashEffect();
		~DownSlashEffect();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(class Collider* other);
		virtual void OnCollisionStay(class Collider* other);
		virtual void OnCollisionExit(class Collider* other);

	private:
		void downSlashEffectComplateEvent();

	private:
		Animator* mAnimator;
		class Transform* tr;
		class Player* player;

		bool hitFlag = false;
	};
}


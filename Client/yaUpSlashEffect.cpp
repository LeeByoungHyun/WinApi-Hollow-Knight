#include "yaUpSlashEffect.h"
#include "yaTransform.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaObject.h"
#include "yaPlayer.h"
#include "yaRigidBody.h"
#include "NailHitEffect.h"

namespace ya
{
	UpSlashEffect::UpSlashEffect()
	{

	}

	UpSlashEffect::~UpSlashEffect()
	{

	}

	void UpSlashEffect::Initialize()
	{
		mAnimator = AddComponent<Animator>();
		player = Player::GetInstance();
		Collider* mCollider = AddComponent<Collider>();
		tr = AddComponent<Transform>();
		tr->SetSize(Vector2(1.2f, 1.2f));

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_UpSlashEffect\\neutral", Vector2::Zero, 0.015f);

		mAnimator->GetCompleteEvent(L"Knight_UpSlashEffectneutral") = std::bind(&UpSlashEffect::upSlashEffectComplateEvent, this);

		mAnimator->Play(L"Knight_UpSlashEffectneutral", true);

		mCollider->SetCenter(Vector2(-60.0f, -180.0f));
		mCollider->SetSize(Vector2(160.0f, 150.0f));

		GameObject::Initialize();
	}

	void UpSlashEffect::Update()
	{
		Vector2 corPos = Vector2(-34.0f, -58.0f);
		tr->SetPos(player->GetPos() + corPos);

		GameObject::Update();
	}

	void UpSlashEffect::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void UpSlashEffect::Release()
	{
		GameObject::Release();

	}

	void UpSlashEffect::OnCollisionEnter(Collider* other)
	{
	}

	void UpSlashEffect::OnCollisionStay(Collider* other)
	{
		// 몬스터와 충돌시 1회에 한해 뒤로 넉백 + 충돌한 몬스터 체력 감소
		if ((other->GetOwner()->GetType() == eLayerType::Monster) && hitFlag == false)
		{
			Vector2 pos = player->GetPos();
			player->GetComponent<RigidBody>()->SetVelocity(Vector2(0.0f, 150.0f));

			object::Instantiate<NailHitEffect>(tr->GetPos(), eLayerType::Effect);
			hitFlag = true;
		}
		if ((other->GetOwner()->GetType() == eLayerType::Hornet) && hitFlag == false)
		{
			Vector2 pos = player->GetPos();
			player->GetComponent<RigidBody>()->SetVelocity(Vector2(0.0f, 150.0f));

			object::Instantiate<NailHitEffect>(tr->GetPos(), eLayerType::Effect);
			hitFlag = true;
		}
		if ((other->GetOwner()->GetType() == eLayerType::FalseKnight) && hitFlag == false)
		{
			Vector2 pos = player->GetPos();
			player->GetComponent<RigidBody>()->SetVelocity(Vector2(0.0f, 150.0f));

			object::Instantiate<NailHitEffect>(tr->GetPos(), eLayerType::Effect);
			hitFlag = true;
		}
	}

	void UpSlashEffect::OnCollisionExit(Collider* other)
	{
	}

	void UpSlashEffect::upSlashEffectComplateEvent()
	{
		object::Destroy(this);
	}
}

#include "yaPlayer.h"
#include "yaTime.h"
#include "yaInput.h"
#include "yaScene.h"
#include "yaTransform.h"
#include "yaAnimator.h"
#include "yaCollider.h"
#include "yaObject.h"
#include "yaResourceManager.h"
#include "yaSceneManager.h"
#include "mySlashEffectLeft.h"
#include "yaSlashEffectRight.h"
#include "yaSlashAltEffectLeft.h"
#include "yaSlashAltEffectRight.h"
#include "yaUpSlashEffect.h"
#include "yaDashEffectLeft.h"
#include "yaDashEffectRight.h"

namespace ya
{
	Player::Player()
	{

	}

	Player::~Player()
	{

	}

	void Player::Initialize()
	{
		Transform* tr = GetComponent<Transform>();
		tr->SetName(L"PlayerTransform");

		hp = 5;
		atk = 1;

		/*
		mSlashTime = 0.0f;
		mSlashAltTime = 0.0f;
		mUpSlashTime = 0.0f;
		*/

		/*
		slashFlag = false;
		slashAltFlag = false;
		upSlashFlag = false;
		dashFlag = false;
		*/

		deathFlag = false;
		invincibilityFlag = false;

		mAnimator = AddComponent<Animator>();

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Idle\\left", Vector2::Zero, 0.1f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Idle\\right", Vector2::Zero, 0.1f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Walk\\left", Vector2::Zero, 0.1f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Walk\\right", Vector2::Zero, 0.1f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Dash\\left", Vector2::Zero, 0.025f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Dash\\right", Vector2::Zero, 0.025f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Slash\\left", Vector2::Zero, 0.05f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Slash\\right", Vector2::Zero, 0.05f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_SlashAlt\\left", Vector2::Zero, 0.05f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_SlashAlt\\right", Vector2::Zero, 0.05f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_UpSlash\\neutral", Vector2::Zero, 0.05f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_FireballCast\\left", Vector2::Zero, 0.05f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_FireballCast\\right", Vector2::Zero, 0.05f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Recoil\\left", Vector2::Zero, 0.05f);
		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Recoil\\right", Vector2::Zero, 0.05f);

		mAnimator->CreateAnimations(L"..\\Resources\\Knight\\Knight_Death\\neutral", Vector2::Zero, 0.066f);

		mAnimator->GetCompleteEvent(L"Knight_Slashleft") = std::bind(&Player::SlashEndEvent, this);
		mAnimator->GetCompleteEvent(L"Knight_Slashright") = std::bind(&Player::SlashEndEvent, this);

		mAnimator->GetCompleteEvent(L"Knight_SlashAltleft") = std::bind(&Player::SlashAltEndEvent, this);
		mAnimator->GetCompleteEvent(L"Knight_SlashAltright") = std::bind(&Player::SlashAltEndEvent, this);

		mAnimator->GetCompleteEvent(L"Knight_UpSlashneutral") = std::bind(&Player::UpSlashEndEvent, this);

		mAnimator->GetCompleteEvent(L"Knight_Dashleft") = std::bind(&Player::DashEndEvent, this);
		mAnimator->GetCompleteEvent(L"Knight_Dashright") = std::bind(&Player::DashEndEvent, this);

		mAnimator->GetCompleteEvent(L"Knight_Recoilleft") = std::bind(&Player::RecoilEndEvent, this);
		mAnimator->GetCompleteEvent(L"Knight_Recoilright") = std::bind(&Player::RecoilEndEvent, this);

		mAnimator->GetCompleteEvent(L"Knight_Deathneutral") = std::bind(&Player::DeathEndEvent, this);


		mAnimator->Play(L"Knight_Idleright", true);

		Collider* mCollider = AddComponent<Collider>();
		mCollider->SetName(L"PlayerCollider");
		mCollider->SetCenter(Vector2(-30.0f, -120.0f));
		mCollider->SetSize(Vector2(60.0f, 120.0f));

		mState = ePlayerState::Idle;

		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();

		curScene = SceneManager::GetActiveScene();
		tr = GetComponent<Transform>();

		// HP가 0이하가 되면 죽음
		if (hp <= 0 && deathFlag == false)
		{
			mState = ePlayerState::Death;

			mAnimator->Play(L"Knight_Deathneutral", false);

			deathFlag = true;
		}

		switch (mState)
		{
		case ya::Player::ePlayerState::Idle:
			idle();
			break;

		case ya::Player::ePlayerState::Walk:
			walk();
			break;

		case ya::Player::ePlayerState::Slash:
			slash();
			break;

		case ya::Player::ePlayerState::SlashAlt:
			slashAlt();
			break;

		case ya::Player::ePlayerState::UpSlash:
			upSlash();
			break;

		case ya::Player::ePlayerState::Dash:
			dash();
			break;

		case ya::Player::ePlayerState::Jump:
			jump();
			break;

		case ya::Player::ePlayerState::Recoil:
			recoil();
			break;

		case ya::Player::ePlayerState::Death:
			death();
			break;

		default:
			break;
		}
	}

	void Player::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Player::Release()
	{
		GameObject::Release();
	}

	void Player::OnCollisionEnter(Collider* other)
	{
		if (mState != ePlayerState::Death)
		{
			// 몬스터 콜라이더와 접촉시 피격 애니메이션
			if (other->GetOwner()->GetType() == eLayerType::Monster && invincibilityFlag == false)
			{
				invincibilityFlag = true;

				mState = ePlayerState::Recoil;
				hp -= 1;

				if (mDirection == eDirection::Left)
				{
					mAnimator->Play(L"Knight_Recoilleft", true);
				}

				else if (mDirection == eDirection::Right)
				{
					mAnimator->Play(L"Knight_Recoilright", true);
				}

				return;
			}
		}

		GameObject::OnCollisionEnter(other);
		
	}

	void Player::OnCollisionStay(Collider* other)
	{
		GameObject::OnCollisionStay(other);

	}

	void Player::OnCollisionExit(Collider* other)
	{
		GameObject::OnCollisionExit(other);

	}

	void Player::idle()
	{
		if (Input::GetKey(eKeyCode::A))
			mDirection = eDirection::Left;
		if (Input::GetKey(eKeyCode::D))
			mDirection = eDirection::Right;

		mSlashTime = 0.0f;
		mSlashAltTime = 0.0f;

		// 좌우 이동키 입력시 Walk 상태로 변경
		if (Input::GetKey(eKeyCode::A) || Input::GetKey(eKeyCode::D))
		{
			mState = ePlayerState::Walk;

			if (mDirection == eDirection::Left)
				mAnimator->Play(L"Knight_Walkleft", true);
			else if (mDirection == eDirection::Right)
				mAnimator->Play(L"Knight_Walkright", true);

			return;
		}

		// 대시키 입력시 dash 상태로 변경
		if (Input::GetKeyDown(eKeyCode::L))
		{
			mState = ePlayerState::Dash;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_Dashleft", true);

				DashEffectLeft* dashEffectLeft =
					object::Instantiate<DashEffectLeft>(tr->GetPos() + Vector2(130.0f, 30.0f), eLayerType::Effect);
			}

			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_Dashright", true);

				DashEffectRight* dashEffectRight =
					object::Instantiate<DashEffectRight>(tr->GetPos() + Vector2(-130.0f, 30.0f), eLayerType::Effect);
			}

			return;
		}

		// W + 공격키 누르면 UpSlash
		if (Input::GetKeyDown(eKeyCode::K) && Input::GetKey(eKeyCode::W))
		{
			mState = ePlayerState::UpSlash;

			mAnimator->Play(L"Knight_UpSlashneutral", true);

			UpSlashEffect* upSlashEffect =
				object::Instantiate<UpSlashEffect>(tr->GetPos() + Vector2(0.0f, -0.0f), eLayerType::Effect);

			return;
		}

		// 공격 입력시 Slash 상태 변경
		if (Input::GetKeyDown(eKeyCode::K))
		{
			mState = ePlayerState::Slash;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_Slashleft", true);

				SlashEffectLeft* slashEffectLeft =
					object::Instantiate<SlashEffectLeft>(tr->GetPos() + Vector2(-60.0f, 0.0f), eLayerType::Effect);
			}

			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_Slashright", true);

				SlashEffectRight* slashEffectRight =
					object::Instantiate<SlashEffectRight>(tr->GetPos() + Vector2(60.0f, 0.0f), eLayerType::Effect);
			}

			return;
		}
	}

	void Player::walk()
	{
		if (Input::GetKey(eKeyCode::A))
			mDirection = eDirection::Left;
		else if (Input::GetKey(eKeyCode::D))
			mDirection = eDirection::Right;

		// 이동키에서 손을 땔 경우 Idle상태로 변경
		if (Input::GetKeyUp(eKeyCode::A) || Input::GetKeyUp(eKeyCode::D))
		{
			mState = ePlayerState::Idle;

			if (mDirection == eDirection::Left)
				mAnimator->Play(L"Knight_Idleleft", true);
			else if (mDirection == eDirection::Right)
				mAnimator->Play(L"Knight_Idleright", true);

			return;
		}

		// 대시키 입력시 dash 상태로 변경
		if (Input::GetKeyDown(eKeyCode::L))
		{
			mState = ePlayerState::Dash;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_Dashleft", true);

				DashEffectLeft* dashEffectLeft =
					object::Instantiate<DashEffectLeft>(tr->GetPos() + Vector2(130.0f, 30.0f), eLayerType::Effect);
			}

			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_Dashright", true);

				DashEffectRight* dashEffectRight =
					object::Instantiate<DashEffectRight>(tr->GetPos() + Vector2(-130.0f, 30.0f), eLayerType::Effect);
			}

			return;
		}

		// W + 공격키 누르면 UpSlash
		if (Input::GetKeyDown(eKeyCode::K) && Input::GetKey(eKeyCode::W))
		{
			mState = ePlayerState::UpSlash;

			mAnimator->Play(L"Knight_UpSlashneutral", true);

			UpSlashEffect* upSlashEffect =
				object::Instantiate<UpSlashEffect>(tr->GetPos() + Vector2(0.0f, -0.0f), eLayerType::Effect);

			return;
		}

		// 공격 입력시 Slash 상태 변경
		if (Input::GetKeyDown(eKeyCode::K))
		{
			mState = ePlayerState::Slash;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_Slashleft", true);

				SlashEffectLeft* slashEffectLeft =
					object::Instantiate<SlashEffectLeft>(tr->GetPos() + Vector2(-60.0f, 0.0f), eLayerType::Effect);
			}

			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_Slashright", true);

				SlashEffectRight* slashEffectRight =
					object::Instantiate<SlashEffectRight>(tr->GetPos() + Vector2(60.0f, 0.0f), eLayerType::Effect);
			}

			return;
		}

		Vector2 pos = tr->GetPos();

		if (Input::GetKey(eKeyCode::A))
			pos.x -= 200.0f * Time::DeltaTime();

		if (Input::GetKey(eKeyCode::D))
			pos.x += 200.0f * Time::DeltaTime();

		tr->SetPos(pos);
	}

	void Player::slash()
	{
		if (Input::GetKey(eKeyCode::A))
			mDirection = eDirection::Left;
		if (Input::GetKey(eKeyCode::D))
			mDirection = eDirection::Right;

		mSlashTime += Time::DeltaTime();
		//slashAltFlag = false;

		// W + 공격키 누르면 UpSlash
		if (Input::GetKeyDown(eKeyCode::K) && Input::GetKey(eKeyCode::W))
		{
			mState = ePlayerState::UpSlash;

			mAnimator->Play(L"Knight_UpSlashneutral", true);

			UpSlashEffect* upSlashEffect =
				object::Instantiate<UpSlashEffect>(tr->GetPos() + Vector2(0.0f, -0.0f), eLayerType::Effect);

			return;
		}

		// slash 상태에서 한번 더 공격시 slashAlt 상대로 변경
		if (Input::GetKeyDown(eKeyCode::K) && mSlashTime >= 0.3f)
		{
			mState = ePlayerState::SlashAlt;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_SlashAltleft", true);

				SlashAltEffectLeft* slashAltEffectLeft =
					object::Instantiate<SlashAltEffectLeft>(tr->GetPos() + Vector2(-40.0f, -10.0f), eLayerType::Effect);
			}
			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_SlashAltright", true);

				SlashAltEffectRight* slashAltEffectRight =
					object::Instantiate<SlashAltEffectRight>(tr->GetPos() + Vector2(40.0f, -10.0f), eLayerType::Effect);
			}

			mSlashTime = 0.0f;

			return;
		}
	}

	void Player::slashAlt()
	{
		Transform* tr = GetComponent<Transform>();

		if (Input::GetKeyDown(eKeyCode::A))
			mDirection = eDirection::Left;
		if (Input::GetKeyDown(eKeyCode::D))
			mDirection = eDirection::Right;

		mSlashAltTime += Time::DeltaTime();
		//slashFlag = false;

		// W + 공격키 누르면 UpSlash
		if (Input::GetKeyDown(eKeyCode::K) && Input::GetKey(eKeyCode::W))
		{
			mState = ePlayerState::UpSlash;

			mAnimator->Play(L"Knight_UpSlashneutral", true);

			UpSlashEffect* upSlashEffect =
				object::Instantiate<UpSlashEffect>(tr->GetPos() + Vector2(0.0f, -0.0f), eLayerType::Effect);

			return;
		}

		// slashAlt 상태에서 한번 더 공격시 slash 상태로 변경
		if (Input::GetKeyDown(eKeyCode::K) && mSlashAltTime >= 0.3f)
		{
			mState = ePlayerState::Slash;

			if (mDirection == eDirection::Left)
			{
				mAnimator->Play(L"Knight_Slashleft", true);

				SlashEffectLeft* slashEffectLeft =
					object::Instantiate<SlashEffectLeft>(tr->GetPos() + Vector2(-60.0f, 0.0f), eLayerType::Effect);
			}

			else if (mDirection == eDirection::Right)
			{
				mAnimator->Play(L"Knight_Slashright", true);

				SlashEffectRight* slashEffectRight =
					object::Instantiate<SlashEffectRight>(tr->GetPos() + Vector2(60.0f, 0.0f), eLayerType::Effect);
			}

			mSlashAltTime = 0.0f;

			return;
		}
	}

	void Player::upSlash()
	{
		if (Input::GetKey(eKeyCode::A))
			mDirection = eDirection::Left;
		if (Input::GetKey(eKeyCode::D))
			mDirection = eDirection::Right;

	}

	void Player::dash()
	{
		Vector2 pos = tr->GetPos();

		if (mDirection == eDirection::Left)
			pos.x -= 800.0f * Time::DeltaTime();

		else if (mDirection == eDirection::Right)
			pos.x += 800.0f * Time::DeltaTime();

		tr->SetPos(pos);
	}

	void Player::jump()
	{

	}

	void Player::fall()
	{

	}

	void Player::castFireball()
	{
	}

	void Player::recoil()
	{
		Vector2 pos = tr->GetPos();

		if (mDirection == eDirection::Left)
			pos.x += 100.0f * Time::DeltaTime();

		else if (mDirection == eDirection::Right)
			pos.x -= 100.0f * Time::DeltaTime();

		tr->SetPos(pos);
	}

	void Player::death()
	{
		
	}

	void Player::SlashEndEvent()
	{
		mState = ePlayerState::Idle;
		if (mDirection == eDirection::Left)
			mAnimator->Play(L"Knight_Idleleft", true);
		else if (mDirection == eDirection::Right)
			mAnimator->Play(L"Knight_Idleright", true);
	}

	void Player::SlashAltEndEvent()
	{
		mState = ePlayerState::Idle;
		if (mDirection == eDirection::Left)
			mAnimator->Play(L"Knight_Idleleft", true);
		else if (mDirection == eDirection::Right)
			mAnimator->Play(L"Knight_Idleright", true);
	}

	void Player::UpSlashEndEvent()
	{
		mState = ePlayerState::Idle;
		if (mDirection == eDirection::Left)
			mAnimator->Play(L"Knight_Idleleft", true);
		else if (mDirection == eDirection::Right)
			mAnimator->Play(L"Knight_Idleright", true);
	}

	void Player::RecoilEndEvent()
	{
		mState = ePlayerState::Idle;
		if (mDirection == eDirection::Left)
			mAnimator->Play(L"Knight_Idleleft", true);
		else if (mDirection == eDirection::Right)
			mAnimator->Play(L"Knight_Idleright", true);

		invincibilityFlag = false;
	}

	void Player::DashEndEvent()
	{
		mState = ePlayerState::Idle;
		if (mDirection == eDirection::Left)
			mAnimator->Play(L"Knight_Idleleft", true);
		else if (mDirection == eDirection::Right)
			mAnimator->Play(L"Knight_Idleright", true);

	}
	void Player::DeathEndEvent()
	{
		object::Destroy(this);
	}
}

#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Hornet : public GameObject
	{
	public:
		enum class eHornetState
		{
			Idle,
			Run,
			JumpAnticipate,
			Jump,
			Land,
			EvadeAnticipate,
			Evade,
			GDashAnticipate,
			GDash,
			GDashRecover,
			ADashAnticipate,
			ADash,
			ADashRecover,
			SphereAnticipateG,
			SphereAnticipateA,
			Sphere,
			SphereRecover,
			ThrowNeedleAnticipate,
			ThrowNeedle,
			ThrowNeedleRecover,
			CounterAnticipate,
			CounterStance,
			CounterEnd,
			CounterAttackAnticipate,
			CounterAttack,
			CounterAttackRecover,	// 이하 스프라이트 없음
			BarbThrowAnticipate,
			BarbThrow,
			BarbThrowRecover,
			StunAir,
			Stun,
			StunStance,
			Wounded,
			Flash,
			Wait
		};

		/*
		enum class eDirection
		{
			Left,
			Right
		};
		*/

		Hornet();
		~Hornet();

		// 싱글톤
		// 이미 인스턴스가 존재하면 인스턴스를 반환, 없다면 인스턴스 생성
		static Hornet* GetInstance()
		{
			if (instance == nullptr)
				instance = new Hornet();

			return instance;
		}

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		virtual void OnCollisionEnter(class Collider* other) override;
		virtual void OnCollisionStay(class Collider* other) override;
		virtual void OnCollisionExit(class Collider* other) override;

		eHornetState GetHornetState() { return mState; }
		eDirection GetHornetDirection() { return mDirection; }
		void SetHornetState(eHornetState state) { mState = state; }
		void Reset();

	private:
		void idle();
		void run();
		void jumpAnticipate();
		void jump();
		void land();
		void evadeAnticipate();
		void evade();
		void gDashAnticipate();
		void gDash();
		void gDashRecover();
		void aDashAnticipate();
		void aDash();
		void aDashRecover();
		void sphereAnticipateG();
		void sphereAnticipateA();
		void sphere();
		void sphereRecover();
		void throwNeedleAnticipate();
		void throwNeedle();
		void throwNeedleRecover();
		void counterAnticipate();
		void counterStance();
		void counterEnd();
		void counterAttackAnticipate();
		void counterAttack();
		void counterAttackRecover();
		void barbThrowAnticipate();
		void barbThrow();
		void barbThrowRecover();
		void stunAir();
		void stun();
		void stunStance();
		void wounded();
		void flash();
		void wait();

		void jumpAnticipateCompleteEvent();
		void jumpCompleteEvent();
		void landCompleteEvent();
		void sphereAnticipateACompleteEvent();
		void sphereAnticipateGCompleteEvent();
		void sphereCompleteEvent();
		void sphereRecoverCompleteEvent();
		void aDashAnticipateCompleteEvent();
		void aDashRecoverCompleteEvent();
		void aDashCompleteEvent();
		void gDashAnticipateCompleteEvent();
		void gDashRecoverCompleteEvent();
		void gDashCompleteEvent();
		void throwNeedleAnticipateCompleteEvent();
		void throwNeedleCompleteEvent();
		void throwNeedleRecoverCompleteEvent();
		void counterAnticipateCompleteEvent();
		void counterStanceCompleteEvent();
		void counterEndCompleteEvent();
		void counterAttackAnticipateCompleteEvent();
		void counterAttackCompleteEvent();
		void counterAttackRecoverCompleteEvent();
		void evadeAnticipateCompleteEvent();
		void evadeCompleteEvent();
		void runCompleteEvent();
		void barbThrowAnticipateCompleteEvent();
		void barbThrowCompleteEvent();
		void barbThrowRecoverCompleteEvent();
		void stunCompleteEvent();

		void initializeFlag();

	private:
		class Transform*	tr;
		class Animator*		mAnimator;
		class Scene*		curScene;
		class RigidBody*	mRigidBody;
		class Collider*		mCollider;

		class Barb01* barb01;
		class Barb02* barb02;
		class Barb03* barb03;
		class Barb04* barb04;
		class SphereBall* ball;
		class HornetNeedle* needle;
		class CounterAttackCollider* caCol;
		
		class Sound* runSound;
		class Sound* landSound;
		class Sound* jumpSound;
		class Sound* dashSound;
		class Sound* deathSound;
		class Sound* laugh01Sound;
		class Sound* laugh02Sound;
		class Sound* stun01Sound;
		class Sound* stun02Sound;
		class Sound* stun03Sound;
		class Sound* attack01Sound;
		class Sound* attack02Sound;
		class Sound* attack03Sound;
		class Sound* attack04Sound;
		class Sound* attack05Sound;
		class Sound* attack06Sound;
		class Sound* attack07Sound;
		class Sound* needleCatchSound;
		class Sound* needleThrowSound;
		class Sound* sphereSound;
		class Sound* barbThrowSound;
		class Sound* parrySound;
		class Sound* slashSound;
		class Sound* hitSound;
		class Sound* bossStunSound;

		eHornetState mState;
		eDirection mDirection = eDirection::Left;
		int jumpPattern;
		int idlePattern;
		int attackPattern;
		int movePattern;
		double mTime;
		double hitTime = 0.0f;;
		int hp;
		int stunHp;

		bool idleFlag						= false;
		bool runFlag						= false;
		bool jumpAnticipateFlag				= false;
		bool jumpFlag						= false;
		bool landFlag						= false;
		bool evadeAnticipateFlag			= false;
		bool evadeFlag						= false;
		bool gDashAnticipateFlag			= false;
		bool gDashFlag						= false;
		bool gDashRecoverFlag				= false;
		bool aDashAnticipateFlag			= false;
		bool aDashFlag						= false;
		bool aDashRecoverFlag				= false;
		bool sphereAnticipateGFlag			= false;
		bool sphereAnticipateAFlag			= false;
		bool sphereFlag						= false;
		bool sphereRecoverFlag				= false;
		bool throwNeedleAnticipateFlag		= false;
		bool throwNeedleFlag				= false;
		bool throwNeedleRecoverFlag			= false;
		bool counterAnticipateFlag			= false;
		bool counterStanceFlag				= false;
		bool counterEndFlag					= false;
		bool counterAttackAnticipateFlag	= false;
		bool counterAttackFlag				= false;
		bool counterAttackRecoverFlag		= false;
		bool barbThrowAnticipateFlag		= false;
		bool barbThrowFlag					= false;
		bool barbThrowRecoverFlag			= false;
		bool stunAirFlag					= false;
		bool stunFlag						= false;
		bool woundedFlag					= false;
		bool flashFlag						= false;
		bool eventFlag						= false;
		bool needleCatchFlag				= false;
		bool moveFlag						= false;
		bool airToGFlag						= false;
		bool stunStanceFlag					= false;
		bool deathFlag						= false;
		bool waitFlag						= false;
		bool hitFlag						= false;

		// 싱글톤 객체 인스턴스
		static Hornet* instance;
	};
}



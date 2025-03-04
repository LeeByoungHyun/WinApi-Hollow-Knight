#include "MantisLordsManager.h"
#include "yaSceneManager.h"
#include "yaTime.h"
#include "yaObject.h"
#include "yaResourceManager.h"
#include "yaSound.h"

#include "MantisLord1.h"
#include "MantisLord2.h"
#include "MantisLord3.h"
#include "MantisLordsProjectile.h"
#include "Fade.h"

namespace ya
{
	MantisLordsManager::MantisLordsManager()
	{

	}

	MantisLordsManager::~MantisLordsManager()
	{

	}

	void MantisLordsManager::Initialize()
	{
		GameObject::Initialize();

		mantisLord1 = MantisLord1::GetInstance();
		mantisLord2 = MantisLord2::GetInstance();
		mantisLord3 = MantisLord3::GetInstance();
		
		// 미리 투사체를 생성해두고 투사체 필요한 패턴에서만 활성화
		projectile1 = object::Instantiate<MantisLordsProjectile>(mantisLord1->GetComponent<Transform>()->GetPos(), eLayerType::Monster);
		projectile2 = object::Instantiate<MantisLordsProjectile>(mantisLord1->GetComponent<Transform>()->GetPos(), eLayerType::Monster);
		projectile3 = object::Instantiate<MantisLordsProjectile>(mantisLord1->GetComponent<Transform>()->GetPos(), eLayerType::Monster);
		projectile1->SetState(eState::Pause);
		projectile2->SetState(eState::Pause);
		projectile3->SetState(eState::Pause);

		//fade = ya::Fade::GetInstance();
		mPhase = ePhaseState::Wait;
		victorySound = ResourceManager::Load<Sound>(L"VictorySound", L"..\\Resources\\Sound\\Hallownest_Call.wav");
	}

	void MantisLordsManager::Update()
	{
		GameObject::Update();

		activeScene = SceneManager::GetActiveScene();

		// 플레이어가 보스룸 진입시 1페이즈 진입
		if (activeScene->GetType() == eSceneType::MantisLordsBoss && StartFlag1 == false)
		{
			//mPhase = ePhaseState::Phase1Start;
		}

		// 모든 보스가 죽으면 보스 인사 애니메이션 출력 후 씬 종료
		if (mantisLord1->GetState() == MantisLord1::eMantisLordsState::ThroneWounded 
			&& mantisLord2->GetState() == MantisLord2::eMantisLordsState::ThroneWounded
			&& mantisLord3->GetState() == MantisLord3::eMantisLordsState::ThroneWounded
			&& flag4 == false)
		{
			mTime += Time::DeltaTime();
			if (mTime >= 5.0f)
			{
				mantisLord1->SetState(MantisLord1::eMantisLordsState::ThroneBow);
				mantisLord2->SetState(MantisLord2::eMantisLordsState::ThroneBow);
				mantisLord3->SetState(MantisLord3::eMantisLordsState::ThroneBow);

				mTime = 0.0f;
				//flag4 = true;
			}

		}

		if (mantisLord1->GetState() == MantisLord1::eMantisLordsState::ThroneBow)
		{
			mTime += Time::DeltaTime();
			if (mTime >= 5.0f)
			{
				endFlag = true;
			}
		}

		// 2페이즈에서 보스가 모두 대기상태일 경우 
		if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Idle
			&& mantisLord3->GetState() == MantisLord3::eMantisLordsState::Idle
			&& mantis2AttackFlag == true
			&& mantis3AttackFlag == true)
		{
			mPhase = ePhaseState::Phase2;
			mantis2AttackFlag = false;
			mantis3AttackFlag = false;
		}

		// 2페이즈 진행중 둘 중 하나라도 죽을 시 3페이즈 진입
		if ((mantisLord2->GetState() == MantisLord2::eMantisLordsState::ThroneWounded
			|| mantisLord3->GetState() == MantisLord3::eMantisLordsState::ThroneWounded)
			&& (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Idle
				|| mantisLord3->GetState() == MantisLord3::eMantisLordsState::Idle))
		{
			mPhase = ePhaseState::Phase3;
			if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::ThroneWounded)
				mantis2DeathFlag = true;
			else if (mantisLord3->GetState() == MantisLord3::eMantisLordsState::ThroneWounded)
				mantis3DeathFlag = true;
		}

		/*
		// 모든 보스가 죽으면 메인 홀 씬으로 변경
		if (flag4 == true)
		{
			if (mantisLord1->GetEndFlag() == true)
			{
				mTime += Time::DeltaTime();

				if (mTime >= 1.0f && flag == false)
				{
					Fade::GetInstance()->SetFadeColor(Fade::eColor::White);
					Fade::GetInstance()->SetFadeState(Fade::eFadeState::FadeOut);
					victorySound->Play(false);
					flag = true;
				}
				if (mTime >= 5.0f)
				{
					Fade::GetInstance()->SetFadeState(Fade::eFadeState::FadeIn);
					SceneManager::LoadScene(eSceneType::MainHall);
					mTime = 0.0f;
				}
			}
		}
		*/
		switch (mPhase)
		{
		case ya::MantisLordsManager::ePhaseState::Phase1Start:
			phase1Start();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase1:
			phase1();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Start:
			phase2Start();
			break; 
		
		case ya::MantisLordsManager::ePhaseState::Phase2:
			phase2();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo1:
			phase2Combo1();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo2:
			phase2Combo2();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo3:
			phase2Combo3();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo4:
			phase2Combo4();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo5:
			phase2Combo5();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase2Combo6:
			phase2Combo6();
			break;

		case ya::MantisLordsManager::ePhaseState::Phase3:
			phase3();
			break;

		case ya::MantisLordsManager::ePhaseState::Wait:
			wait();
			break;

		default:
			break;
		}
	}

	void MantisLordsManager::Release()
	{
		GameObject::Release();
	}

	void MantisLordsManager::Reset()
	{
		StartFlag1 = false;
		StartFlag2 = false;
		startFlag3 = false;
		flag4 = false;
		mantis2AttackFlag = false;
		mantis3AttackFlag = false;
		mantis2DeathFlag = false;
		mantis3DeathFlag = false;
		flag = false;
		endFlag = false;
		mTime = 0.0f;
		projectile1->SetState(eState::Pause);
		projectile2->SetState(eState::Pause);
		projectile3->SetState(eState::Pause);
		mPhase = ePhaseState::Wait;
		mantisLord1->Reset();
		mantisLord2->Reset();
		mantisLord3->Reset();
	}

	void MantisLordsManager::phase1Start()
	{
		mTime += Time::DeltaTime();
		/*
		if (mTime >= 3.0f)
		{
			mantisLord1->SetState(MantisLord1::eMantisLordsState::ThroneStand);
			mPhase = ePhaseState::Phase1;
			mTime = 0.0f;
			StartFlag1 = true;
		}
		*/
		mantisLord1->SetState(MantisLord1::eMantisLordsState::ThroneStand);
		mPhase = ePhaseState::Phase1;
		mTime = 0.0f;
		StartFlag1 = true;
	}

	void MantisLordsManager::phase1()
	{
		// 패턴 3가지 랜덤하게 실행
		if (mantisLord1->GetState() == MantisLord1::eMantisLordsState::Idle)
		{
			mTime += Time::DeltaTime();
			if (mTime >= 2.0f)
			{
				srand((unsigned int)time(NULL));
				int pattern = rand() % 3;
				mTime = 0.0f;
				switch (pattern)
				{
				case 0:	// Dash
					mantisLord1->SetState(MantisLord1::eMantisLordsState::DashArrive);
					break;

				case 1:	// Dstab
					mantisLord1->SetState(MantisLord1::eMantisLordsState::DstabArrive);
					break;

				case 2:	// WallThrow
					mantisLord1->SetState(MantisLord1::eMantisLordsState::WallArrive);
					break;

				default:
					mantisLord1->SetState(MantisLord1::eMantisLordsState::Idle);
					break;
				}
			}
		}

		// throw 패턴에서 투사체 활성화
		if (mantisLord1->GetState() == MantisLord1::eMantisLordsState::Throw 
			&& projectile1->GetActiveFlag() == false)
		{
			projectile1->SetState(eState::Active);
			projectile1->SetProjectileState(MantisLordsProjectile::eProjectileState::Active);
			projectile1->SetPos(mantisLord1->GetComponent<Transform>()->GetPos());
			if (mantisLord1->GetDirection() == MantisLord1::eDirection::Left)
				projectile1->SetDirection(MantisLordsProjectile::eDirection::Left);
			else
				projectile1->SetDirection(MantisLordsProjectile::eDirection::Right);
		}

		// 1번 보스가 죽으면 2페이즈 진입
		if (mantisLord1->GetState() == MantisLord1::eMantisLordsState::ThroneWounded && StartFlag2 == false)
		{
			mTime += Time::DeltaTime();
			if (mTime >= 2.0f)
			{
				mPhase = ePhaseState::Phase2Start;
				mTime = 0.0f;
				StartFlag2 = true;
			}
		}
	}

	void MantisLordsManager::phase2Start()
	{
		mTime += Time::DeltaTime();
		if (mTime >= 2.0f)
		{
			mantisLord2->SetState(MantisLord2::eMantisLordsState::ThroneStand);
			mantisLord3->SetState(MantisLord3::eMantisLordsState::ThroneStand);
			mPhase = ePhaseState::Phase2;
			mTime = 0.0f;
			StartFlag2 = true;
		}
	}

	void MantisLordsManager::phase2()
	{
		// 두 보스가 모두 대기상태일 경우 패턴 실행
		if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Idle
			&& mantisLord3->GetState() == MantisLord3::eMantisLordsState::Idle)
		{
			mTime += Time::DeltaTime();
			if (mTime >= 2.0f)
			{
				mantis2AttackFlag = false;
				mantis3AttackFlag = false;
				mTime = 0.0f;

				srand((unsigned int)time(NULL));
				int pattern = rand() % 6;
				switch (pattern)
				{
				case 0:	// Dash, Dstab
					mPhase = ePhaseState::Phase2Combo1;
					break;

				case 1:	// Wall
					mPhase = ePhaseState::Phase2Combo2;
					break;

				case 2:	// Wall
					mPhase = ePhaseState::Phase2Combo3;
					break;

				case 3:	// Wall
					mPhase = ePhaseState::Phase2Combo4;
					break;

				case 4:	// Wall
					mPhase = ePhaseState::Phase2Combo5;
					break;

				case 5:	// Wall
					mPhase = ePhaseState::Phase2Combo6;
					break;

				default:
					mantisLord2->SetState(MantisLord2::eMantisLordsState::Idle);
					mantisLord3->SetState(MantisLord3::eMantisLordsState::Idle);
					break;
				}

			}
		}
	}

	void MantisLordsManager::phase3()
	{
		if (mantis2DeathFlag == true)
		{
			// 패턴 3가지 랜덤하게 실행
			if (mantisLord3->GetState() == MantisLord3::eMantisLordsState::Idle)
			{
				mTime += Time::DeltaTime();
				if (mTime >= 0.5f)
				{
					srand((unsigned int)time(NULL));

					int direct = rand() % 2;
					if (direct == 0)
						mantisLord3->SetDirection(MantisLord3::eDirection::Left);
					else
						mantisLord3->SetDirection(MantisLord3::eDirection::Right);

					int pattern = rand() % 3;
					mTime = 0.0f;
					switch (pattern)
					{
					case 0:	// Dash
						mantisLord3->SetState(MantisLord3::eMantisLordsState::DashArrive);
						break;

					case 1:	// Dstab
						mantisLord3->SetState(MantisLord3::eMantisLordsState::DstabArrive);
						break;

					case 2:	// WallThrow
						mantisLord3->SetState(MantisLord3::eMantisLordsState::WallArrive);
						break;

					default:
						mantisLord3->SetState(MantisLord3::eMantisLordsState::Idle);
						break;
					}
				}
			}

			// throw 패턴에서 투사체 활성화
			if (mantisLord3->GetState() == MantisLord3::eMantisLordsState::Throw
				&& projectile3->GetActiveFlag() == false)
			{
				projectile3->SetState(eState::Active);
				projectile3->SetProjectileState(MantisLordsProjectile::eProjectileState::Active);
				projectile3->SetPos(mantisLord3->GetComponent<Transform>()->GetPos());
				if (mantisLord3->GetDirection() == MantisLord3::eDirection::Left)
					projectile3->SetDirection(MantisLordsProjectile::eDirection::Left);
				else
					projectile3->SetDirection(MantisLordsProjectile::eDirection::Right);
			}
		}

		else if (mantis3DeathFlag == true)
		{
			// 패턴 3가지 랜덤하게 실행
			if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Idle)
			{
				mTime += Time::DeltaTime();
				if (mTime >= 1.0f)
				{
					srand((unsigned int)time(NULL));

					int direct = rand() % 2;
					if (direct == 0)
						mantisLord2->SetDirection(MantisLord2::eDirection::Left);
					else
						mantisLord2->SetDirection(MantisLord2::eDirection::Right);

					int pattern = rand() % 3;
					mTime = 0.0f;
					switch (pattern)
					{
					case 0:	// Dash
						mantisLord2->SetState(MantisLord2::eMantisLordsState::DashArrive);
						break;

					case 1:	// Dstab
						mantisLord2->SetState(MantisLord2::eMantisLordsState::DstabArrive);
						break;

					case 2:	// WallThrow
						mantisLord2->SetState(MantisLord2::eMantisLordsState::WallArrive);
						break;

					default:
						mantisLord2->SetState(MantisLord2::eMantisLordsState::Idle);
						break;
					}
				}
			}

			// throw 패턴에서 투사체 활성화
			if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Throw
				&& projectile2->GetActiveFlag() == false)
			{
				projectile2->SetState(eState::Active);
				projectile2->SetProjectileState(MantisLordsProjectile::eProjectileState::Active);
				projectile2->SetPos(mantisLord2->GetComponent<Transform>()->GetPos());
				if (mantisLord2->GetDirection() == MantisLord2::eDirection::Left)
					projectile2->SetDirection(MantisLordsProjectile::eDirection::Left);
				else
					projectile2->SetDirection(MantisLordsProjectile::eDirection::Right);
			}
		}
	}

	void MantisLordsManager::wait()
	{
	}

	void MantisLordsManager::phase2Combo1()
	{
		// 2번보스 돌진 후 3번보스 돌진
		if (mantis2AttackFlag == false)
		{
			srand((unsigned int)time(NULL));
			int direct = rand() % 2;
			switch (direct)
			{
			case 0:
				mantisLord2->SetDirection(MantisLord2::eDirection::Left);
				mantisLord3->SetDirection(MantisLord3::eDirection::Left);
				break;

			case 1:
				mantisLord2->SetDirection(MantisLord2::eDirection::Right);
				mantisLord3->SetDirection(MantisLord3::eDirection::Right);
				break;
			}

			mantisLord2->SetState(MantisLord2::eMantisLordsState::DashArrive);
			mantis2AttackFlag = true;
			mTime = 0.0f;
		}

		mTime += Time::DeltaTime();
		if (mantis3AttackFlag == false && mTime >= 1.2f)
		{
			mantisLord3->SetState(MantisLord3::eMantisLordsState::DashArrive);
			mantis3AttackFlag = true;
			mTime = 0.0f;
		}
	}

	void MantisLordsManager::phase2Combo2()
	{
		// 2번보스 돌진 후 3번보스 내려찍기
		if (mantis2AttackFlag == false)
		{
			srand((unsigned int)time(NULL));
			int direct = rand() % 2;
			switch (direct)
			{
			case 0:
				mantisLord2->SetDirection(MantisLord2::eDirection::Left);
				break;

			case 1:
				mantisLord2->SetDirection(MantisLord2::eDirection::Right);
				break;
			}

			mantisLord2->SetState(MantisLord2::eMantisLordsState::DashArrive);
			mantis2AttackFlag = true;
			mTime = 0.0f;
		}

		mTime += Time::DeltaTime();
		if (mantis3AttackFlag == false && mTime >= 1.2f)
		{
			mantisLord3->SetState(MantisLord3::eMantisLordsState::DstabArrive);
			mantis3AttackFlag = true;
			mTime = 0.0f;
		}
	}

	void MantisLordsManager::phase2Combo3()
	{
		// 2번보스 내려찍기 후 3번보스 돌진
		if (mantis2AttackFlag == false)
		{
			srand((unsigned int)time(NULL));
			int direct = rand() % 2;
			switch (direct)
			{
			case 0:
				mantisLord3->SetDirection(MantisLord3::eDirection::Left);
				break;

			case 1:
				mantisLord3->SetDirection(MantisLord3::eDirection::Right);
				break;
			}

			mantisLord2->SetState(MantisLord2::eMantisLordsState::DstabArrive);
			mantis2AttackFlag = true;
			mTime = 0.0f;
		}

		mTime += Time::DeltaTime();
		if (mantis3AttackFlag == false && mTime >= 1.2f)
		{
			mantisLord3->SetState(MantisLord3::eMantisLordsState::DashArrive);
			mantis3AttackFlag = true;
			mTime = 0.0f;
		}
	}

	void MantisLordsManager::phase2Combo4()
	{
		// 2번보스 내려찍기 후 3번보스 내려찍기
		if (mantis2AttackFlag == false)
		{
			mantisLord2->SetState(MantisLord2::eMantisLordsState::DstabArrive);
			mantis2AttackFlag = true;
			mTime = 0.0f;
		}

		mTime += Time::DeltaTime();
		if (mantis3AttackFlag == false && mTime >= 1.2f)
		{
			mantisLord3->SetState(MantisLord3::eMantisLordsState::DstabArrive);
			mantis3AttackFlag = true;
			mTime = 0.0f;
		}
	}

	void MantisLordsManager::phase2Combo5()
	{
		// 동시에 반대편에서 돌진
		if (mantis2AttackFlag == false)
		{
			mantisLord2->SetDirection(MantisLord2::eDirection::Left);
			mantisLord3->SetDirection(MantisLord3::eDirection::Right);

			mantisLord2->SetState(MantisLord2::eMantisLordsState::DashArrive);
			mantisLord3->SetState(MantisLord3::eMantisLordsState::DashArrive);
			mantis3AttackFlag = true;
			mantis2AttackFlag = true;
			mTime = 0.0f;
		}
	}

	void MantisLordsManager::phase2Combo6()
	{
		if (mantis2AttackFlag == false)
		{
			// 동시에 벽에 붙어 투사체 공격
			mantisLord2->SetDirection(MantisLord2::eDirection::Left);
			mantisLord3->SetDirection(MantisLord3::eDirection::Right);

			mantisLord2->SetState(MantisLord2::eMantisLordsState::WallArrive);
			mantisLord3->SetState(MantisLord3::eMantisLordsState::WallArrive);

			mantis3AttackFlag = true;
			mantis2AttackFlag = true;
		}

		// throw 패턴에서 투사체 활성화
		if (mantisLord2->GetState() == MantisLord2::eMantisLordsState::Throw
			&& projectile2->GetActiveFlag() == false)
		{
			projectile2->SetState(eState::Active);
			projectile2->SetProjectileState(MantisLordsProjectile::eProjectileState::Active);
			projectile2->SetPos(mantisLord2->GetComponent<Transform>()->GetPos());
			if (mantisLord2->GetDirection() == MantisLord2::eDirection::Left)
				projectile2->SetDirection(MantisLordsProjectile::eDirection::Left);
			else
				projectile2->SetDirection(MantisLordsProjectile::eDirection::Right);
		}

		// throw 패턴에서 투사체 활성화
		if (mantisLord3->GetState() == MantisLord3::eMantisLordsState::Throw
			&& projectile3->GetActiveFlag() == false)
		{
			projectile3->SetState(eState::Active);
			projectile3->SetProjectileState(MantisLordsProjectile::eProjectileState::Active);
			projectile3->SetPos(mantisLord3->GetComponent<Transform>()->GetPos());
			if (mantisLord3->GetDirection() == MantisLord3::eDirection::Left)
				projectile3->SetDirection(MantisLordsProjectile::eDirection::Left);
			else
				projectile3->SetDirection(MantisLordsProjectile::eDirection::Right);
		}
	}
}


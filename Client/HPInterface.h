#pragma once
class HPInterface
{
};

#pragma once
#include "yaGameObject.h"

namespace ya
{
	class HPInterface : public GameObject
	{
	public:
		enum class eHPState
		{
			Remain00,
			Remain01,
			Remain02,
			Remain03,
			Remain04,
			Remain05,
		};

		HPInterface();
		~HPInterface();

		// 싱글톤
		// 이미 인스턴스가 존재하면 인스턴스를 반환, 없다면 인스턴스 생성
		static HPInterface* GetInstance()
		{
			if (instance == nullptr)
				instance = new HPInterface();

			return instance;
		}

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

		void initializeHP();

	private:
		void remain00();
		void remain01();
		void remain02();
		void remain03();
		void remain04();
		void remain05();

		class Player* mPlayer;
		class HPobject01* hp01;
		class HPobject02* hp02;
		class HPobject03* hp03;
		class HPobject04* hp04;
		class HPobject05* hp05;
		class Image* mImage;
		class Transform* tr;
		class Animator* mAnimator;

		eHPState mHPstate;
		class Scene* activeScene;
		float mTime = 0.0f;

		bool remainFlag00 = false;
		bool remainFlag01 = false;
		bool remainFlag02 = false;
		bool remainFlag03 = false;
		bool remainFlag04 = false;
		bool remainFlag05 = false;

		// 싱글톤 객체 인스턴스
		static HPInterface* instance;
	};
}



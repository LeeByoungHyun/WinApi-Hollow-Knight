#include "yaMainHallScene.h"
#include "yaSceneManager.h"
#include "yaInput.h"


namespace ya
{
	MainHallScene::MainHallScene()
	{

	}

	MainHallScene::~MainHallScene()
	{

	}

	void MainHallScene::Initialize()
	{
		// 게임오브젝트 추가는 이곳에
		mPlayer = new Player();
		AddGameObject(mPlayer, eLayerType::Player);
		mPlayer->SetName(L"Player");

		mBackboard = new Backboard();
		AddGameObject(mBackboard, eLayerType::BG);
		mBackboard->SetName(L"backboard");

		Scene::Initialize();
	}

	void MainHallScene::Update()
	{
		Scene::Update();

		if (Input::GetKeyState(eKeyCode::P) == eKeyState::Down)
		{
			SceneManager::LoadScene(eSceneType::Boss);
		}
	}

	void MainHallScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

	}

	void MainHallScene::Release()
	{
		Scene::Release();

	}

	void MainHallScene::Enter()
	{
		
	}

	void MainHallScene::Exit()
	{

	}
}
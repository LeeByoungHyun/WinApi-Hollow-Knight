#pragma once
#include "yaGameObject.h"

namespace ya
{
	class Snow04 : public GameObject
	{
	public:

		Snow04();
		~Snow04();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render(HDC hdc) override;
		virtual void Release() override;

	private:
		class Image* mImage;
		class Transform* tr;
	};
}



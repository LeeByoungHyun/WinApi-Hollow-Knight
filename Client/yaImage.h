#pragma once
#include "yaResource.h"

namespace ya
{
	class Image : public Resource
	{
	public:
		static Image* Create(const std::wstring& name, UINT widht, UINT height);

		Image();
		~Image();

		virtual HRESULT Load(const std::wstring& path) override;

		HDC GetHdc() { return mHdc; }
		HBITMAP GetBitmap() { return mBitmap; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }

		COLORREF GetPixel(int x, int y);
		void SetPixel(int x, int y, COLORREF color);

	private:
		HBITMAP mBitmap;
		HDC mHdc;
		UINT mWidth;
		UINT mHeight;
	};
}

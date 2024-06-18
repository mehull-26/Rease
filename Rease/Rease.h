#ifndef REASE_H
#define REASE_H

#include <Windows.h>
#include <windowsx.h>

#include<vector>
#include<string>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <iostream>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <d2d1_1.h>

using namespace std;

template<class Interface>
inline void
safeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

const double PI = 3.14159265358979323846;
namespace Rez
{
	extern bool Running;
	extern MSG Event;
	class Color { public: float r = 0; float g = 0; float b = 0; float a = 0;  Color(float a, float b, float c, float d) :r(a), g(b), b(c), a(d) {}; };
	class Vector2 { public: int x = 0; int y = 0; Vector2(int a, int b) :x(a), y(b) {}; };

	void WindowCreator(string title, int width, int height, DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME);
	void EventHandler();
	void Render();
	void SetBGcolor(Color color);

	void CreateEllipse(Vector2 Center, float radiusX, float radiusY, Color color, bool fill, float stroke = 1);
	void CreatePolygon(Rez::Vector2 TopLeftPt, int sides, int sidelength, Rez::Color color, float stroke = 1.0f);
	void CreateLine(Rez::Vector2 pt1, Rez::Vector2 pt2, Rez::Color color, float stroke = 1.0f);
	void CreateGeometry(int ElementCount, D2D1_POINT_2F PointArr[], Color color, bool fill, float stroke = 1.0f);
	void AddImage(Rez::Vector2 topLeftPoint, std::string Path, float opacity, float scale);

	bool IsKeyDown(unsigned int key);
	bool MouseButtonDown(string button);
	extern Vector2 ClickCoord ;
	extern Vector2 CurrentMouseCoord ;

	void Destroy();
}


class Rease
{
public:
	Rease();
	~Rease();
	HRESULT Initialize(string title, int width, int height, DWORD style);

	HRESULT Scene();
	inline void DrawEllipse(Rez::Vector2 Center, float radiusX, float radiusY, bool fill, float stroke);
	inline void DrawPolygon(Rez::Vector2 startPt, int sides, int sidelength, float stroke = 1.0f);
	inline void DrawLine(Rez::Vector2 pt1, Rez::Vector2 pt2, float stroke = 1.0f);
	inline void DrawGeometry(int, bool fill, D2D1_POINT_2F pt[], int i, float stroke = 1.0f);
	D2D1_POINT_2F DrawSlantLine(D2D1_POINT_2F StartPt, float angle, float length, float stroke);
	HRESULT LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* m_pIWICFactory, PCWSTR uri, ID2D1Bitmap** ppBitmap);


	friend void Rez::Destroy();

public:
	int Counters[5] = { 0 };
	enum LinkedCounter { Ellipse, Polygon, Line, Geometry, Image};
	struct CounterData
	{
		Rez::Vector2 Pt1 = Rez::Vector2(0, 0);
		Rez::Vector2 Pt2 = Rez::Vector2(0, 0);
		float a = 0; float b = 0; float c = 0;
		D2D1_COLOR_F color = D2D1::ColorF(0, 0, 0, 1);
		bool fill = true;
		D2D1_POINT_2F* Arr;
		std::string string;
	};
	Rez::Color BGcolor = Rez::Color(0, 0, 0, 1);


	std::vector<CounterData> ellipseData;
	std::vector<CounterData> LineData;
	std::vector<CounterData> PolygonData;
	std::vector<CounterData> GeometryData;
	std::vector<CounterData> ImageData;
	std::vector< ID2D1PathGeometry*> GeometrySinkVector;
	std::vector< ID2D1Bitmap*> ImageSinkVector;

	bool m_keys[256] = { false };
	bool m_Mbuttons[3] = { false };

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();

	wchar_t* StringToWChar(const std::string& str);
	void FreeWChar(wchar_t* buffer);
	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lparam);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pFactory;
	IWICImagingFactory* m_pIWICFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	ID2D1SolidColorBrush* m_pBrush1;
	ID2D1SolidColorBrush* m_pBrush2;


	ID2D1EllipseGeometry* m_pEllipse;
	ID2D1PathGeometry* m_pFigure;
};


#endif
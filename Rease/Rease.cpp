#include "Rease.h"
Rease app;

HRESULT Rease::LoadBitmapFromFile(ID2D1RenderTarget* pRenderTarget, IWICImagingFactory* m_pIWICFactory, PCWSTR uri, ID2D1Bitmap** ppBitmap)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	HRESULT hr = m_pIWICFactory->CreateDecoderFromFilename(
		uri,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	if (SUCCEEDED(hr)) {
		hr = pDecoder->GetFrame(0, &pSource);
	}
	if (SUCCEEDED(hr)) {
		hr = m_pIWICFactory->CreateFormatConverter(&pConverter);
	}
	if (SUCCEEDED(hr)) {
		hr = pConverter->Initialize(
			pSource,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0f,
			WICBitmapPaletteTypeMedianCut
		);
	}
	if (SUCCEEDED(hr)) {
		hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, nullptr, ppBitmap);
	}
	if (pDecoder) pDecoder->Release();
	if (pSource) pSource->Release();
	if (pConverter) pConverter->Release();
	return hr;
}
wchar_t* Rease::StringToWChar(const std::string& str) {
	// Get the length of the resulting wide string
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (len == 0) {
		// Failed to convert
		throw std::runtime_error("Error converting string to wchar_t*");
	}

	// Allocate memory for the wide string
	wchar_t* buffer = new wchar_t[len];

	// Convert the string to wide characters
	if (MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, len) == 0) {
		// Failed to convert
		delete[] buffer;
		throw std::runtime_error("Error converting string to wchar_t*");
	}

	return buffer;
}
void Rease::FreeWChar(wchar_t* buffer) {
	delete[] buffer;
}
Rease::Rease()
{
	m_hwnd = NULL;
	m_pIWICFactory = NULL;
	m_pRenderTarget = NULL;
	m_pFactory = NULL;

	m_pBrush1 = NULL;
	m_pBrush2 = NULL;

	m_pEllipse = NULL;
	//m_pFigure = NULL;

}
Rease::~Rease()
{
	safeRelease(&m_pFactory);
	safeRelease(&m_pIWICFactory);
	safeRelease(&m_pRenderTarget);

	safeRelease(&m_pBrush1);
	safeRelease(&m_pBrush2);

	safeRelease(&m_pEllipse);
	safeRelease(&m_pFigure);
}


HRESULT Rease::Initialize(std::string title, int width, int height, DWORD style)
{
	HRESULT hr = S_OK;
	hr = CreateDeviceIndependentResources();
	HINSTANCE hInstance = GetModuleHandle(NULL);
	wchar_t* a = StringToWChar(title);

	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Rease::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = hInstance;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"Rease";

		RegisterClassEx(&wcex);


		m_hwnd = CreateWindow(L"Rease", L"MAIN", style, GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2, 0, 0, NULL, NULL, hInstance, this);

		if (m_hwnd)
		{
			float dpi = GetDpiForWindow(m_hwnd);

			SetWindowPos(m_hwnd, NULL, NULL, NULL, static_cast<int>(ceil(width * dpi / 96.f)),
				static_cast<int>(ceil(height * dpi / 96.f)),
				SWP_NOMOVE);

			SetWindowText(m_hwnd, a);
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
			FreeWChar(a);
		}
		hr = CreateDeviceResources();

	}



	return hr;
}
HRESULT Rease::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory);
	return hr;
}
HRESULT Rease::CreateDeviceResources()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{

		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
		D2D1_HWND_RENDER_TARGET_PROPERTIES hwndRtProps = D2D1::HwndRenderTargetProperties(m_hwnd, size);

		hr = m_pFactory->CreateHwndRenderTarget(
			rtProps,
			hwndRtProps,
			&m_pRenderTarget
		);


		// Initialize WIC
		if (SUCCEEDED(hr))
		{
			hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
			if (SUCCEEDED(hr)) {
				hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					nullptr,
					CLSCTX_INPROC_SERVER,
					IID_IWICImagingFactory,
					(LPVOID*)&m_pIWICFactory
				);
			}
		}
		// Load the bitmap from a file (example: L"image.png")
		/*if (SUCCEEDED(hr)) {
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pIWICFactory, L"image.png", &m_pBitmap);
		}*/
		if (SUCCEEDED(hr))
		{
			// Create a gray brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&m_pBrush1
			);
		}

		if (SUCCEEDED(hr))
		{
			// Create a red brush.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::DarkRed),
				&m_pBrush2
			);
		}


	}
	return hr;
}
HRESULT Rease::Scene()
{
	HRESULT hr = S_OK;
		for (int i = 0; i < Counters[Geometry]; i++)
		{
			DrawGeometry(GeometryData[i].a, GeometryData[i].fill, GeometryData[i].Arr, i);
		}
		for (int i = 0; i < Counters[Image]; i++)
		{
			wchar_t* PathW = StringToWChar(ImageData[i].string);
			hr = LoadBitmapFromFile(m_pRenderTarget, m_pIWICFactory, PathW, &ImageSinkVector[i]);
		}

		m_pRenderTarget->BeginDraw();
		m_pRenderTarget->Clear(D2D1::ColorF(BGcolor.r,BGcolor.g,BGcolor.b,BGcolor.a));

		for (int i = 0; i < Counters[Ellipse]; i++)
		{
			app.m_pBrush1->SetColor(ellipseData[i].color);
			DrawEllipse(ellipseData[i].Pt1, ellipseData[i].a, ellipseData[i].b, ellipseData[i].fill, ellipseData[i].c);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		for (int i = 0; i < Counters[Line]; i++)
		{
			m_pBrush2->SetColor(LineData[i].color);
			DrawLine(LineData[i].Pt1, LineData[i].Pt2, LineData[i].c);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		for (int i = 0; i < Counters[Polygon]; i++)
		{
			m_pBrush2->SetColor(PolygonData[i].color);
			DrawPolygon(PolygonData[i].Pt1, PolygonData[i].a, PolygonData[i].b, PolygonData[i].c);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		for (int i = 0; i < Counters[Geometry]; i++)
		{
			m_pBrush2->SetColor(GeometryData[i].color);
			if (GeometryData[i].fill) {
				m_pRenderTarget->FillGeometry(GeometrySinkVector[i], m_pBrush2);
			}
			else { m_pRenderTarget->DrawGeometry(GeometrySinkVector[i], m_pBrush2, GeometryData[i].c); }
			safeRelease(&GeometrySinkVector[i]);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		for (int i = 0; i < Counters[Image]; i++)
		{
			if (ImageSinkVector[i]) {
				m_pRenderTarget->DrawBitmap(
					ImageSinkVector[i],
					D2D1::RectF(ImageData[i].Pt1.x, ImageData[i].Pt1.y, static_cast<float>((ImageSinkVector[i]->GetSize().width) * ImageData[i].b), static_cast<float>((ImageSinkVector[i]->GetSize().height) * ImageData[i].b)), ImageData[i].a
				);
			}
			safeRelease(&ImageSinkVector[i]);
		}
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->EndDraw();

		return hr;
}

inline void Rease::DrawEllipse(Rez::Vector2 Center, float radiusX, float radiusY, bool fill, float stroke)
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(Center.x, Center.y), radiusX, radiusY);
	m_pFactory->CreateEllipseGeometry(ellipse, &m_pEllipse);
	if (m_pEllipse)
	{
		if (fill)
		{
			m_pRenderTarget->FillGeometry(m_pEllipse, m_pBrush1);
		}
		else
		{
			m_pRenderTarget->DrawGeometry(m_pEllipse, m_pBrush1, stroke);
		}
	}
}
inline void Rease::DrawPolygon(Rez::Vector2 startPt, int sides, int sidelength, float stroke)
{
	D2D1_POINT_2F end = D2D1::Point2F(startPt.x, startPt.y);
	float angle = 0;

	for (int i = 0; i < sides; i++)
	{
		end = DrawSlantLine(end, angle, sidelength, stroke);
		angle = angle + (360 / sides);
	}
}
inline void Rease::DrawLine(Rez::Vector2 pt1, Rez::Vector2 pt2, float stroke)
{
	m_pRenderTarget->DrawLine(D2D1::Point2F(pt1.x, pt1.y), D2D1::Point2F(pt2.x, pt2.y), m_pBrush2, stroke);
}
inline void Rease::DrawGeometry(int Points, bool fill, D2D1_POINT_2F pt[], int i, float stroke)
{
	HRESULT hr = m_pFactory->CreatePathGeometry(&GeometrySinkVector[i]);
	if (SUCCEEDED(hr))
	{
		ID2D1GeometrySink* pSink = NULL;
		hr = GeometrySinkVector[i]->Open(&pSink);

		if (SUCCEEDED(hr))
		{
			if (fill)
			{
				pSink->BeginFigure(pt[0], D2D1_FIGURE_BEGIN_FILLED);
			}
			else
			{
				pSink->BeginFigure(pt[0], D2D1_FIGURE_BEGIN_HOLLOW);
			}
			m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

			pSink->AddLines(&pt[0], Points);


		}
		if (fill)
		{
			pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
		}
		else
		{
			pSink->EndFigure(D2D1_FIGURE_END_OPEN);
		}
		pSink->Close();
		safeRelease(&pSink);
	}
}

D2D1_POINT_2F Rease::DrawSlantLine(D2D1_POINT_2F StartPt, float angle, float length, float stroke)
{
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(angle, StartPt));

	// endPoint defines one end of the hand.
	D2D_POINT_2F endPoint = D2D1::Point2F(StartPt.x + length, StartPt.y);
	// Draw a line from the center of the ellipse to endPoint.
	m_pRenderTarget->DrawLine(StartPt, D2D1::Point2F(endPoint.x + 2 * stroke / 5, endPoint.y + 1 * stroke / 5), m_pBrush1, stroke);
	return D2D1::Point2F(StartPt.x + length * cos((PI / 180) * angle), StartPt.y + length * sin((PI / 180) * angle));
}

LRESULT CALLBACK Rease::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Rease* pApp = (Rease*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(pApp)
		);

		result = 1;
	}
	else
	{
		Rease* pApp = reinterpret_cast<Rease*>(static_cast<LONG_PTR>(
			::GetWindowLongPtrW(
				hwnd,
				GWLP_USERDATA
			)));

		bool wasHandled = false;

		if (pApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				pApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;
			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hwnd, NULL, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			/*case WM_PAINT:
			{
				pApp->Scene();
				ValidateRect(hwnd, NULL);
			}*/
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_KEYDOWN:
			{
				app.m_keys[unsigned int(wParam)] = true;
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_KEYUP:
			{
				app.m_keys[unsigned int(wParam)] = false;
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_LBUTTONDOWN:
			{
				app.m_Mbuttons[0] = true;
				Rez::ClickCoord = Rez::Vector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_RBUTTONDOWN:
			{
				app.m_Mbuttons[2] = true;
				Rez::ClickCoord = Rez::Vector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_MBUTTONDOWN:
			{
				app.m_Mbuttons[1] = true;
				Rez::ClickCoord = Rez::Vector2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_LBUTTONUP:
			{
				app.m_Mbuttons[0] = false;
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_RBUTTONUP:
			{
				app.m_Mbuttons[2] = false;
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_MBUTTONUP:
			{
				app.m_Mbuttons[1] = false;
			}
			result = 1;
			wasHandled = true;
			break;

			case WM_MOUSEMOVE:
			{
				Rez::CurrentMouseCoord = Rez::Vector2(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
			}
			result = 1;
			wasHandled = true;
			break;
			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}
void Rease::OnResize(UINT width, UINT height)
{
	if (m_pRenderTarget)
	{
		D2D1_SIZE_U size;
		size.width = width;
		size.height = height;

		// Note: This method can fail, but it's okay to ignore the
		// error here -- it will be repeated on the next call to
		// EndDraw
		HRESULT hr = m_pRenderTarget->Resize(size);
	}
}


namespace Rez
{
	bool Running = true;
	MSG Event = {};

	void WindowCreator(std::string title, int width, int height, DWORD style)
	{
		// Ignore the return value because we want to continue running even in the
	// unlikely event that HeapSetInformation fails.
		HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
		if (SUCCEEDED(CoInitialize(NULL)))
		{
			app.Initialize(title,width,height,style);
		}
	}
	void EventHandler()
	{
		while (PeekMessage(&Event, NULL, 0, 0, PM_REMOVE)) {
			if (Event.message == WM_QUIT) {
				break;
			}
			TranslateMessage(&Event);
			DispatchMessage(&Event);
		}
	}
	void Render()
	{
		app.Scene();
	}
	void SetBGcolor(Color color)
	{
		app.BGcolor = color;
	}


	void CreateEllipse(Vector2 Center, float radiusX, float radiusY, Color color, bool fill, float stroke)
	{
		app.Counters[app.Ellipse]++;
		Rease::CounterData a;
		a.Pt1 = Center; a.a = radiusX; a.b = radiusY; a.c = stroke; a.color = D2D1::ColorF(color.r, color.g, color.b, color.a); a.fill = fill;
		app.ellipseData.push_back(a);
		//InvalidateRect(app.m_hwnd, nullptr, FALSE);
	}
	void CreatePolygon(Vector2 TopLeftPt, int sides, int sidelength, Color color, float stroke)
	{
		app.Counters[app.Polygon]++;
		Rease::CounterData a;
		a.Pt1 = TopLeftPt; a.a = sides; a.b = sidelength; a.c = stroke; a.color = D2D1::ColorF(color.r, color.g, color.b, color.a);
		app.PolygonData.push_back(a);
	}
	void CreateLine(Vector2 pt1, Vector2 pt2, Color color, float stroke )
	{
		app.Counters[app.Line]++; 
		Rease::CounterData a;
		a.Pt1 = pt1; a.Pt2 = pt2; a.c = stroke; a.color = D2D1::ColorF(color.r, color.g, color.b, color.a);
		app.LineData.push_back(a);

	}
	void CreateGeometry(int ElementCount, D2D1_POINT_2F PointArr[], Color color,bool fill, float stroke )
	{
		app.Counters[app.Geometry]++;
		Rease::CounterData a;
		a.a = ElementCount; a.c = stroke; a.color = D2D1::ColorF(color.r, color.g, color.b, color.a); a.fill = fill; a.Arr = PointArr;
		app.GeometryData.push_back(a);
		ID2D1PathGeometry* m_pGeometry = NULL;
		app.GeometrySinkVector.push_back(m_pGeometry);
	}
	void AddImage(Vector2 topLeftPoint, std::string Path, float opacity,float scale)
	{
		app.Counters[app.Image]++;
		Rease::CounterData a;
		a.Pt1 = topLeftPoint; a.string = Path; a.a = opacity; a.b = scale;
		app.ImageData.push_back(a); 
		ID2D1Bitmap* m_pBitmap = NULL;
		app.ImageSinkVector.push_back(m_pBitmap);
	}

	bool IsKeyDown(unsigned int key)
	{
		// Return what state the key is in (pressed/not pressed).
		return app.m_keys[key];
	}
	bool MouseButtonDown(string button)
	{
		if (button == "left" || button == "Left")
		{
			return app.m_Mbuttons[0];
		}
		if (button == "right" || button == "Right")
		{
			return app.m_Mbuttons[2];
		}
		if (button == "middle" || button == "Middle")
		{
			return app.m_Mbuttons[1];
		}
	}
	Vector2 ClickCoord = Vector2(0,0);
	Vector2 CurrentMouseCoord = Vector2(0, 0);

	void Destroy()
	{
		safeRelease(&app.m_pFactory);
		safeRelease(&app.m_pIWICFactory);
		safeRelease(&app.m_pRenderTarget);

		safeRelease(&app.m_pBrush1);
		safeRelease(&app.m_pBrush2);

		safeRelease(&app.m_pEllipse);
		safeRelease(&app.m_pFigure);
		CoUninitialize();
	}
}

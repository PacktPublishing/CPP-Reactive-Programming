////////////////////////////////////////////////////
//
// A Simple MFC based Clock Program
//
// Written by Praseed Pai K.T.
//              http://praseedp.blogspot.com
//
//
//
#include "stdafx.h"
#include <afxwin.h>
#include <afxext.h>
#include <math.h>

//////////////////////////////////
//
// constant to represent PI
//
//
const double PI = 3.14159;

/////////////////////////////
//
// The Main Frame Window
//
//
class CClockFrame :public CFrameWnd
{
private:
	RECT _rect;
public:

	CClockFrame()
	{
		HBRUSH brush = (HBRUSH)::CreateSolidBrush(RGB(175, 238, 238));
		CString mywindow = AfxRegisterWndClass(
			CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, 0, brush, 0);

		Create(mywindow, _T("MFC Clock By Praseed Pai"));
	}

	int OnCreate(LPCREATESTRUCT l)
	{
		CFrameWnd::OnCreate(l);
		SetTimer(1, 1000, NULL);
		return 1;
	}

	void OnPaint()
	{
		CPaintDC d(this);
		CBrush b(RGB(100, 149, 237));

		int x1 = -200;
		int y1 = -220;
		int x2 = 210;
		int y2 = 200;
		Transform(&x1, &y1);
		Transform(&x2, &y2);

		CRect rect(x1, y1, x2, y2);
		d.FillRect(&rect, &b);



		CPen p2(PS_SOLID, 2, RGB(153, 0, 0));
		d.SelectObject(&p2);

		int fhour;
		int fmin;
		int fsec;

		CTime t1;



		CTime t = CTime::GetCurrentTime();

		fhour = t.GetHour() % 12;
		fmin = t.GetMinute();
		fsec = t.GetSecond();

		fhour += fmin / 60;
		fhour = fhour * 360 / 12;
		fmin = fmin * 360 / 60;
		fsec = fsec * 360 / 60;

		int xs = 120 * cos((-fsec * PI / 180.0) + PI / 2.0);
		int ys = 120 * sin((-fsec * PI / 180.0) + PI / 2.0);

		int xm = 100 * cos((-fmin * PI / 180.0) + PI / 2.0);
		int ym = 100 * sin((-fmin * PI / 180.0) + PI / 2.0);


		int xh = 60 * cos((-fhour * PI / 180.0) + PI / 2.0);
		int yh = 60 * sin((-fhour * PI / 180.0) + PI / 2.0);

		int x = 0;
		int y = 0;


		Transform(&xh, &yh);
		Transform(&x, &y);

		d.MoveTo(x, y);
		d.LineTo(xh, yh);


		Transform(&xm, &ym);


		d.MoveTo(x, y);
		d.LineTo(xm, ym);


		Transform(&xs, &ys);
		d.MoveTo(x, y);
		d.LineTo(xs, ys);


		char *str[] = { "3","2","1","12","11","10","9","8","7","6","5","4" };
		CFont f;
		f.CreatePointFont(120, _T("Times New Roman"));
		d.SelectObject(&f);
		d.SetTextColor(RGB(204, 0, 0));
		d.SetBkMode(TRANSPARENT);
		for (int i = 0; i < 12; i++)
		{
			int x = 200 * cos(i * PI / 6);
			int y = 200 * sin(i * PI / 6);
			Transform(&x, &y);
			d.TextOut(x, y, CString(str[i]), strlen(str[i]));

		}

	}

	void Transform(int *px, int *py)
	{
		::GetClientRect(m_hWnd, &_rect);
		int width = (_rect.right - _rect.left) / 2;
		int height = (_rect.bottom - _rect.top) / 2;
		*px = *px + width;
		*py = height - *py;

	}

	void OnTimer(UINT n)
	{

		this->Invalidate(0);

	}

	DECLARE_MESSAGE_MAP();
};

BEGIN_MESSAGE_MAP(CClockFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////
// Our Application Object
//
//
//
//
class CClockApp :public CWinApp
{
public:
	int InitInstance()
	{
		CClockFrame *p;
		p = new CClockFrame();
		p->ShowWindow(1);
		m_pMainWnd = p;
		return 1;
	}

};

/////////////////////////
//
// Every Application require a 
// Global Application Object instance

CClockApp a;

//Eod main.cpp
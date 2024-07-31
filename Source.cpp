#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

TCHAR szClassName[] = TEXT("Window");

// ダイアグプロシージャ DialogProc
INT_PTR CALLBACK DialogProc1(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EndDialog(hDlg, IDOK);
			break;
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

INT_PTR CALLBACK DialogProc2(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hDialog1;
	static int hChildDialogWidth;
	static int hChildDialogHeight;
	static int xPos;
	static int yPos;
	switch (msg)
	{
	case WM_INITDIALOG:
		{
			hDialog1 = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), hDlg, (DLGPROC)DialogProc1);
			RECT rect1;
			GetWindowRect(hDialog1, &rect1);
			hChildDialogWidth = rect1.right - rect1.left;
			hChildDialogHeight = rect1.bottom - rect1.top;
		}
		break;
 	case WM_SIZE:
		{
			RECT rect2;
			GetClientRect(hDlg, &rect2);
			int nWidth2 = rect2.right;
			int nHeight2 = rect2.bottom;

			SCROLLINFO si = { sizeof(si) };
			si.fMask = SIF_ALL;

			si.nMin = 0;
			si.nMax = hChildDialogHeight;
			si.nPage = nHeight2;
			si.nPos = 0;

			SetScrollInfo(hDlg, SB_VERT, &si, TRUE);

			si.nMin = 0;
			si.nMax = hChildDialogWidth;
			si.nPage = nWidth2;
			si.nPos = 0;

			SetScrollInfo(hDlg, SB_HORZ, &si, TRUE);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta > 0) {
				SendMessage(hDlg, WM_VSCROLL, SB_LINEUP, 0);
			}
			else {
				SendMessage(hDlg, WM_VSCROLL, SB_LINEDOWN, 0);
			}
		}
		break;
	case WM_MOUSEHWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (delta > 0) {
				SendMessage(hDlg, WM_HSCROLL, SB_LINERIGHT, 0);
			}
			else {
				SendMessage(hDlg, WM_HSCROLL, SB_LINELEFT, 0);
			}
		}
		break;
	case WM_VSCROLL:
		{
			SCROLLINFO si = {};
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hDlg, SB_VERT, &si);
			switch (LOWORD(wParam))
			{
			case SB_TOP:
				si.nPos = 0;
				break;
			case SB_BOTTOM:
				si.nPos = si.nMax - (int)si.nPage;
				break;
			case SB_LINEUP:
				si.nPos -= 16;
				break;
			case SB_LINEDOWN:
				si.nPos += 16;
				break;
			case SB_PAGEUP:
				si.nPos -= 32;
				break;
			case SB_PAGEDOWN:
				si.nPos += 32;
				break;
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			}
			si.nPos = max(0, min(si.nPos, si.nMax - (int)si.nPage));
			SetScrollInfo(hDlg, SB_VERT, &si, TRUE);
			SetWindowPos(hDialog1, 0, xPos, -si.nPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			yPos = -si.nPos;
		}
		break;
	case WM_HSCROLL:
		{
			SCROLLINFO si = {};
			si.cbSize = sizeof(si);
			si.fMask = SIF_ALL;
			GetScrollInfo(hDlg, SB_HORZ, &si);
			switch (LOWORD(wParam))
			{
			case SB_LEFT:
				si.nPos = 0;
				break;
			case SB_RIGHT:
				si.nPos = si.nMax - (int)si.nPage;
				break;
			case SB_LINELEFT:
				si.nPos -= 16;
				break;
			case SB_LINERIGHT:
				si.nPos += 16;
				break;
			case SB_PAGELEFT:
				si.nPos -= 32;
				break;
			case SB_PAGERIGHT:
				si.nPos += 32;
				break;
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			}
			si.nPos = max(0, min(si.nPos, si.nMax - (int)si.nPage));
			SetScrollInfo(hDlg, SB_HORZ, &si, TRUE);
			SetWindowPos(hDialog1, 0, -si.nPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
			xPos = -si.nPos;
		}
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND hDialog2;
	switch (msg)
	{
	case WM_CREATE:
		hDialog2 = CreateDialog(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG2), hWnd, (DLGPROC)DialogProc2);
		break;
	case WM_SIZE:
		MoveWindow(hDialog2, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	WNDCLASS wndclass = {
		CS_HREDRAW | CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("Window"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,
		0,
		CW_USEDEFAULT,
		0,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

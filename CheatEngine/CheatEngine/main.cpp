#include "include.h"
#include "cheatEngine.h"


#define SEARCH 1
#define NEXT 2
#define WRITE 3

HWND hwnd,hEdit;

// The main window class name.
static TCHAR szWindowClass[] = _T("CheatEngine");


// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("CheatEngine");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"),
			_T("TuAntiVirus"), NULL);

		return 1;
	}
	// Store instance handle in our global variable
	hInst = hInstance;

	// The parameters to CreateWindowEx explained:
	// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hwnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hwnd)
	{
		MessageBox(NULL,
			_T("Call to Createwindow failed"),
			_T("TuAntivirus"), NULL);
		return 1;
	}
	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;

}



//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT     ps;
	HDC             hdc;
	TCHAR logo[] = _T("CheatEngine");
	TCHAR Value[] = _T("Value:");

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// Here your application is laid out.
		// For this introduction, we just print out "CheatEngine!"
		// in the bottom left corner.
		SetBkColor(hdc, RGB(240, 240, 240));
		TextOut(hdc, 20, 500, logo, _tcslen(logo));
		TextOut(hdc, 130, 20, Value, _tcslen(Value));
		// End application-specific layout section.
		EndPaint(hWnd, &ps);
		break;
	case WM_CREATE:
		CreateWindow(L"Button",L"Search",
			WS_CHILD | WS_VISIBLE,
			20, 20, 100, 80,
			hWnd, (HMENU)SEARCH, NULL, NULL);
		CreateWindow(L"Button",L"NextScan",
			WS_CHILD | WS_VISIBLE,
			20, 120, 100, 80,
			hWnd, (HMENU)NEXT, NULL, NULL);
		CreateWindow(L"Button", L"Write",
			WS_CHILD | WS_VISIBLE,
			20, 220, 100, 80,
			hWnd, (HMENU)WRITE, NULL, NULL);
		hEdit = CreateWindow(L"Edit", L"...",
			WS_VISIBLE | WS_CHILD,
			180, 20, 200, 20,
			hWnd, NULL, NULL, NULL);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == SEARCH)
		{
			HANDLE process_snapshot = 0;
			PROCESSENTRY32 pe32 = { 0 };

			pe32.dwSize = sizeof(PROCESSENTRY32);
			process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			Process32First(process_snapshot, &pe32);

			do {
				// see if correct process is open
				if (wcscmp(pe32.szExeFile, L"popcapgame1.exe") == 0) {
					// Retrieve a process handle so that we can read and write the game's memory
					HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);


					std::wstring title;
					title.reserve(GetWindowTextLength(hEdit) + 1);
					GetWindowText(hEdit, const_cast<WCHAR*>(title.c_str()), title.capacity());

					long value = std::stoi(title);
					search(process, value);
					// Close the process handle
					CloseHandle(process);
					break;
				}

			} while (Process32Next(process_snapshot, &pe32));
		}
		else if (LOWORD(wParam) == NEXT)
		{
			HANDLE process_snapshot = 0;
			PROCESSENTRY32 pe32 = { 0 };

			pe32.dwSize = sizeof(PROCESSENTRY32);
			process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			Process32First(process_snapshot, &pe32);

			do {
				// see if correct process is open
				if (wcscmp(pe32.szExeFile, L"popcapgame1.exe") == 0) {
					// Retrieve a process handle so that we can read and write the game's memory
					HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);


					std::wstring title;
					title.reserve(GetWindowTextLength(hEdit) + 1);
					GetWindowText(hEdit, const_cast<WCHAR*>(title.c_str()), title.capacity());

					long value = std::stoi(title);
					next(process, value);
					// Close the process handle
					CloseHandle(process);
					break;
				}

			} while (Process32Next(process_snapshot, &pe32));
		}
		else if (LOWORD(wParam) == WRITE)
		{
			HANDLE process_snapshot = 0;
			PROCESSENTRY32 pe32 = { 0 };

			pe32.dwSize = sizeof(PROCESSENTRY32);
			process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			Process32First(process_snapshot, &pe32);

			do {
				// see if correct process is open
				if (wcscmp(pe32.szExeFile, L"popcapgame1.exe") == 0) {
					// Retrieve a process handle so that we can read and write the game's memory
					HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);


					std::wstring title;
					title.reserve(GetWindowTextLength(hEdit) + 1);
					GetWindowText(hEdit, const_cast<WCHAR*>(title.c_str()), title.capacity());

					long value = std::stoi(title);
					write(process, value);
					// Close the process handle
					CloseHandle(process);
					break;
				}

			} while (Process32Next(process_snapshot, &pe32));
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}
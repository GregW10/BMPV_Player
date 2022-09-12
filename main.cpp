#include <iostream>
#include <windows.h>
#include <string>

typedef std::basic_string<wchar_t> wstring;

unsigned int screenWidth;
unsigned int screenHeight;

#define xMainWindow (screenWidth/10)
#define yMainWindow (screenHeight/10)
#define mainWindowWidth (screenWidth - (2*screenWidth)/10)
#define mainWindowHeight (screenHeight - (2*screenHeight)/10)

#define OPEN_FILE_MENU_ID 1
#define EXIT_MENU_ID 2

HMENU mainMenuHandle;

inline HMENU create_menus(HWND hWnd) {
    if (hWnd == nullptr) {
        return nullptr;
    }
    HMENU hMainMenu = CreateMenu();
    HMENU hSubFileMenu = CreateMenu();
    AppendMenuW(hSubFileMenu, MF_STRING, OPEN_FILE_MENU_ID, L"Open File");
    AppendMenuW(hSubFileMenu, MF_SEPARATOR, reinterpret_cast<UINT_PTR>(nullptr), nullptr);
    AppendMenuW(hSubFileMenu, MF_STRING, EXIT_MENU_ID, L"Exit");
    AppendMenuW(hMainMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hSubFileMenu), L"File");
    SetMenu(hWnd, hMainMenu);
    return hMainMenu;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            mainMenuHandle = create_menus(hWnd);
            break;
        case WM_PAINT: {
            HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255)); // add parameter for setting default bg colour
            PAINTSTRUCT ps{};
            HDC hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &ps.rcPaint, hBrush);
            // std::cout << ps.rcPaint.bottom << " " << ps.rcPaint.top << " " << ps.rcPaint.left << " " << ps.rcPaint.right << std::endl;
            // ps.rcPaint.bottom += 100;
            // ps.rcPaint.top -= 100;
            // ps.rcPaint.left += 100;
            // ps.rcPaint.right -= 100;
            // FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(149, 234, 132)));
            EndPaint(hWnd, &ps);
            DeleteObject(hBrush);
            ReleaseDC(hWnd, hdc);
            break;
        }
        case WM_MENUSELECT:
            break;
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case EXIT_MENU_ID:
                    DestroyWindow(hWnd);
                    break;
            }
            break;
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    return 0; // change this
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    const wchar_t cls_name[] = L"Main Window Class";

    WNDCLASSW wc{};
    HWND hWnd;
    MSG msg{};

    wc.hInstance = hInstance;
    wc.lpszClassName = cls_name;
    wc.lpfnWndProc = WindowProc;

    ATOM mainClsAtom = RegisterClassW(&wc);
    if (mainClsAtom == 0) {
        MessageBoxW(nullptr, L"Error registering window class.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, cls_name, L"BMPV Player", WS_OVERLAPPEDWINDOW, xMainWindow, yMainWindow,
                           mainWindowWidth, mainWindowHeight, nullptr, nullptr, hInstance, nullptr);
    if (hWnd == nullptr) {
        MessageBoxW(nullptr, L"Error creating main window.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return static_cast<int>(msg.lParam);
}
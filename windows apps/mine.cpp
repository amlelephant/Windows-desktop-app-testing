#include <windows.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <string>

#pragma comment(lib, "user32.lib")

using namespace std;

// window constants
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 758

#define TEXT_AREA 2

#define SUBMIT_BUTTON 3

LRESULT CALLBACK PassWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK mainWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ArchivesWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK ArchivesProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

WNDPROC oldEditProc;

void mainScreenSubmit();
void AddMainWindowControls(HWND hWnd);
void AddPassControls(HWND hWnd);
void AddArchivesControls(HWND hWnd);
void createMainWindow();
void SubmitPassword();
void archivesSubmit();
void createArchivesWindow();

// main window widgets
HWND hWelcome, hMainEntry;

// password widgets
HWND hEdit, hPassPrompt, hSubmit;
bool cursorVisible = true;

// archives widgets
HWND hArchivesEntry;

HWND hMainWindow, hPassWindow, hArchivesWindow;

//functionality


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    // password window class
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"passwordWindowClass";
    wc.lpfnWndProc = PassWindowProcedure;

    // main window class
    WNDCLASSW wc2 = {0};

    wc2.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc2.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc2.hInstance = hInst;
    wc2.lpszClassName = L"mainWindowClass";
    wc2.lpfnWndProc = mainWindowProcedure;

    // archives window class
    WNDCLASSW wc3 = {0};

    wc3.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc3.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc3.hInstance = hInst;
    wc3.lpszClassName = L"archivesWindowClass";
    wc3.lpfnWndProc = ArchivesWindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;
    
    if (!RegisterClassW(&wc2))
        return -1;

    if (!RegisterClassW(&wc3))
        return -1;

    HWND hPassWindow = CreateWindowW(L"passwordWindowClass", L"Terminal Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, NULL, NULL);
    


    MSG msg = {0};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK mainWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        AddMainWindowControls(hWnd);
        SetFocus(hMainEntry);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

LRESULT CALLBACK PassWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        AddPassControls(hWnd);
        SetFocus(hEdit);
        break;
    case WM_TIMER:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:
        if (LOWORD(wp) == SUBMIT_BUTTON)
        {
            SubmitPassword();
        }
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

LRESULT CALLBACK ArchivesWindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        AddArchivesControls(hWnd);
        SetFocus(hArchivesEntry);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

LRESULT CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wp == VK_RETURN)
        {
            mainScreenSubmit();
            return 0;
        }
        break;
    }
    return CallWindowProc(oldEditProc, hWnd, msg, wp, lp);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wp == VK_RETURN)
        {
            SubmitPassword();
            return 0;
        }
        break;
    }
    return CallWindowProc(oldEditProc, hWnd, msg, wp, lp);
}

LRESULT CALLBACK ArchivesProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wp == VK_RETURN)
        {
            archivesSubmit();
            return 0;
        }
        break;
    }
    return CallWindowProc(oldEditProc, hWnd, msg, wp, lp);
}

void AddPassControls(HWND hWnd)
{
    //hPasswordPrompt = CreateWindowW(L"Static", L"Enter password: ", WS_VISIBLE | WS_CHILD, 20, 50, 120, 20, hWnd, 0, 0, 0);
    hPassPrompt = CreateWindowW(L"Static", L"Enter password: ", WS_VISIBLE | WS_CHILD, 0, 0, 120, 20, hWnd, 0, 0, 0);
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_PASSWORD | WS_BORDER, 120, 0, 500, 20, hWnd, (HMENU)TEXT_AREA, NULL, NULL);
    hSubmit = CreateWindowW(L"Button", L"Submit", WS_VISIBLE | WS_CHILD, 620, 0, 100, 20, hWnd, (HMENU)SUBMIT_BUTTON, NULL, NULL);
    //hCursor = CreateWindowW(L"Static", L"_", WS_VISIBLE | WS_CHILD, 140, 50, 10, 20, hWnd, 0, 0, 0);

    oldEditProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)EditProc);
}

void AddMainWindowControls(HWND hWnd)
{
    hWelcome = CreateWindowW(L"Static", L"welcome", WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH - 50) / 2, 50, 120, 20, hWnd, 0, 0, 0);
    std::wstring options = L"================\n"
                           L"  1. Archives\n"
                           L"  2. Arsenal\n"
                           L"  3. Patch Notes\n"
                           L"  4. Exit \n"
                           L"================";

    CreateWindowW(L"Static", options.c_str(), WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH - 100) / 2, 100, WINDOW_WIDTH, WINDOW_HEIGHT, hWnd, 0, 0, 0);

    hMainEntry = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL, (WINDOW_WIDTH - 100) / 2, 200, 10, 20, hWnd, (HMENU)TEXT_AREA, NULL, NULL);

    oldEditProc = (WNDPROC)SetWindowLongPtr(hMainEntry, GWLP_WNDPROC, (LONG_PTR)MainProc);
}

void AddArchivesControls(HWND hWnd)
{
    CreateWindowW(L"Static", L"Archives", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, 0, 0, 0);
    std::wstring options = L"=================\n"
                           L"  1. Logger\n"
                           L"  2. Read Entries\n"
                           L"  3. Images\n"
                           L"  4. Exit \n"
                           L"=================";
    CreateWindowW(L"Static", options.c_str(), WS_VISIBLE | WS_CHILD, (WINDOW_WIDTH - 100) / 2, 100, WINDOW_WIDTH, WINDOW_HEIGHT, hWnd, 0, 0, 0);
    hArchivesEntry = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL, (WINDOW_WIDTH - 100) / 2, 200, 10, 20, hWnd, (HMENU)TEXT_AREA, NULL, NULL);

    oldEditProc = (WNDPROC)SetWindowLongPtr(hArchivesEntry, GWLP_WNDPROC, (LONG_PTR)ArchivesProc);
}

void createMainWindow()
{
    hMainWindow = CreateWindowW(L"mainWindowClass", L"Main Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, NULL, NULL);
}

void createArchivesWindow()
{
    hArchivesWindow = CreateWindowW(L"archivesWindowClass", L"Archives", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, NULL, NULL);
}

void SubmitPassword()
{
    
    char *pass = new char[100];
    GetWindowTextA(hEdit, pass, 100);
    if (strcmp(pass, "password") == 0)
    {
        createMainWindow();
        ShowWindow(hMainWindow, SW_SHOW);
        DestroyWindow(hPassWindow);
    }
    else
    {
        MessageBoxA(hPassWindow, "Incorrect password", "Error", MB_OK);
        SetWindowTextA(hEdit, "");
    }
}



void mainScreenSubmit()
{
    char *text = new char[100];
    GetWindowTextA(hMainEntry, text, 100);
    if (strcmp(text, "1") == 0)
    {
        createArchivesWindow();
        ShowWindow(hArchivesWindow, SW_SHOW);
        //DestroyWindow(hMainWindow);
    }
    else if (strcmp(text, "2") == 0)
    {
        MessageBoxA(hMainWindow, "Arsenal", "Arsenal", MB_OK);
    }
    else if (strcmp(text, "3") == 0)
    {
        MessageBoxA(hMainWindow, "Patch Notes", "Patch Notes", MB_OK);
    }
    else if (strcmp(text, "4") == 0)
    {
        DestroyWindow(hMainWindow);
    }
    else
    {
        MessageBoxA(hMainWindow, "Invalid option", "Error", MB_OK);
    }
    SetWindowTextA(hMainEntry, "");
}

void archivesSubmit()
{
    char *text = new char[100];
    GetWindowTextA(hArchivesEntry, text, 100);
    if (strcmp(text, "1") == 0)
    {
        MessageBoxA(hMainWindow, "Archives", "Archives", MB_OK);
    }
    else if (strcmp(text, "2") == 0)
    {
        MessageBoxA(hMainWindow, "Arsenal", "Arsenal", MB_OK);
    }
    else if (strcmp(text, "3") == 0)
    {
        MessageBoxA(hMainWindow, "Patch Notes", "Patch Notes", MB_OK);
    }
    else if (strcmp(text, "4") == 0)
    {
        DestroyWindow(hMainWindow);
    }
    else
    {
        MessageBoxA(hMainWindow, "Invalid option", "Error", MB_OK);
    }
    SetWindowTextA(hArchivesEntry, "");
}
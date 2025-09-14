#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define open_file_button 1
#define SAVE_FILE_BUTTON 2

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void AddControls(HWND hWnd);
void open_file(HWND hWnd);
void save_file(HWND hWnd);

HWND hMainWindow, hEdit;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_COMMAND:
        switch (wp)
        {
        case open_file_button:
            open_file(hWnd);
            break;
        case SAVE_FILE_BUTTON:
            save_file(hWnd);
            break;
        }
        break;
    case WM_CREATE:
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        DestroyWindow(hWnd);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void display_file(char *path)
{
    FILE *file;
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    int _size = ftell(file);
    rewind(file);
    char *data = new char[_size + 1];
    fread(data, _size, 1, file);
    data[_size] = '\0';

    SetWindowText(hEdit, data);

    fclose(file);
}

void open_file(HWND hWnd)
{
    OPENFILENAME ofn;

    char file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All Files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetOpenFileName(&ofn);

    display_file(ofn.lpstrFile);
}

void writeFile(char *path)
{
    FILE *file;
    file = fopen(path, "w");

    int len = GetWindowTextLength(hEdit);
    char *data = new char[len + 1];
    GetWindowText(hEdit, data, len+1);

    fwrite(data, len+1, 1, file);

    fclose(file);
}

void save_file(HWND hWnd)
{
    OPENFILENAME ofn;

    char file_name[100];

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = file_name;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All Files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;

    GetSaveFileName(&ofn);

    writeFile(ofn.lpstrFile);
}

void AddControls(HWND hWnd)
{
    CreateWindowW(L"Button", L"Open File", WS_VISIBLE | WS_CHILD, 10, 10, 150, 36, hWnd, (HMENU)open_file_button, 0, 0);
    CreateWindowW(L"button", L"Save File", WS_VISIBLE | WS_CHILD, 170, 10, 150, 36, hWnd, (HMENU)SAVE_FILE_BUTTON, 0, 0);
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL, 10, 50, 400, 300, hWnd, 0, 0, 0);
}
#include <windows.h>
#include <stdlib.h>

#pragma comment(lib, "lgdi32.lib");

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define GENERATE_BUTTON 4

// cool name for a notepad remake is "notepad#"

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void AddMenus(HWND hWnd);
void AddControls(HWND hWnd);
void loadImages();
void registerDialogClass(HINSTANCE hInst);
void displayDialog(HWND hWnd);

HMENU hMenu;
HWND hName, hAge, hOut, hDog, hMainWindow;
HBITMAP hDogImage;

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

    registerDialogClass(hInst);

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
    int val;
    switch (msg)
    {
    case WM_COMMAND:

        switch (wp)
        {
        case FILE_MENU_EXIT:
            val = MessageBoxW(hWnd, L"Are you sure?", L"Wait!", MB_YESNO | MB_ICONEXCLAMATION);
            if (val == IDYES)
                DestroyWindow(hWnd);
            break;
        case FILE_MENU_NEW:
            displayDialog(hWnd);
            break;
        case GENERATE_BUTTON:
            char name[30], age[10], out[50];

            GetWindowText(hName, name, 30);
            GetWindowText(hAge, age, 10);

            if (strcmp(name, "") == 0 || strcmp(age, "") == 0)
            {
                val = MessageBoxW(hWnd, L"You did not enter anything!", 0, MB_ABORTRETRYIGNORE | MB_ICONERROR);
                switch (val)
                {
                case IDABORT:
                    DestroyWindow(hWnd);
                    break;
                case IDRETRY:
                    return 0;
                case IDIGNORE:
                    break;
                }
            }

            strcpy(out, name);
            strcat(out, " is ");
            strcat(out, age);
            strcat(out, " years old.");

            SetWindowText(hOut, out);
            break;
        }
        break;
    case WM_CREATE:
        AddMenus(hWnd);
        loadImages();
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hSubMenu = CreateMenu();

    AppendMenu(hSubMenu, MF_STRING, 0, "SubMenu Item");

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
    AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open SubMenu");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, 0);
    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_STRING, 2, "Help");

    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
    CreateWindowW(L"Static", L"Name : ", WS_VISIBLE | WS_CHILD, 100, 50, 98, 38, hWnd, 0, 0, 0);
    hName = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 50, 98, 38, hWnd, 0, 0, 0);

    CreateWindowW(L"Static", L"Age : ", WS_VISIBLE | WS_CHILD, 100, 90, 98, 38, hWnd, 0, 0, 0);
    hAge = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 200, 90, 98, 38, hWnd, 0, 0, 0);

    CreateWindowW(L"Button", L"Generate", WS_VISIBLE | WS_CHILD, 150, 140, 98, 38, hWnd, (HMENU)GENERATE_BUTTON, 0, 0);

    hOut = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 100, 200, 300, 200, hWnd, 0, 0, 0);

    hDog = CreateWindowW(L"Static", 0, WS_VISIBLE | WS_CHILD | SS_BITMAP, 350, 60, 100, 100, hWnd, 0, 0, 0);
    SendMessageW(hDog, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hDogImage);
}

void loadImages()
{
    hDogImage = (HBITMAP)LoadImageW(0, L"C:/Users/Aiden/Desktop/Christmas List/New folder/c++/dog.bmp", IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
}

LRESULT CALLBACK DialogProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch(msg){
        case WM_COMMAND:
            switch(wp){
                case 1:
                EnableWindow(hMainWindow, true);
                DestroyWindow(hWnd);
            break;
            }
        case WM_CLOSE:
            EnableWindow(hMainWindow, true);
            DestroyWindow(hWnd);
            break;
        
        default:
            return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void registerDialogClass(HINSTANCE hInst)
{
    WNDCLASSW dialog = {0};

    dialog.hbrBackground = (HBRUSH)COLOR_WINDOW;
    dialog.hCursor = LoadCursor(NULL, IDC_CROSS);
    dialog.hInstance = hInst;
    dialog.lpszClassName = L"myDialogClass";
    dialog.lpfnWndProc = DialogProcedure;

    RegisterClassW(&dialog);

}

void displayDialog(HWND hWnd)
{
    HWND hDlg = CreateWindowW(L"myDialogClass", L"Dialog", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 400,400,200,200, hWnd, 0, 0, 0);

    CreateWindowW(L"button", L"Close", WS_VISIBLE | WS_CHILD, 20,20,100,40, hDlg, (HMENU)1, 0, 0);

    EnableWindow(hWnd, false);

}
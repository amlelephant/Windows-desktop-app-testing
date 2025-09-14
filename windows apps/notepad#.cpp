#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <dwmapi.h>
#include <ostream>
#include <iostream>

using namespace std;

#pragma comment(lib, "ComDlg32.lib");
#pragma comment(lib, "dwmapi.lib");

// window constants
#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 758

// File constants
#define FILE_MENU_NEW 1
#define FILE_MENU_NEW_WINDOW 2
#define FILE_MENU_OPEN 3
#define FILE_MENU_SAVE 4
#define FILE_MENU_SAVE_AS 5
#define FILE_MENU_PAGE_SETUP 6
#define FILE_MENU_PRINT 7
#define FILE_MENU_EXIT 8

// Edit constants
#define EDIT_MENU_UNDO 9
#define EDIT_MENU_CUT 10
#define EDIT_MENU_COPY 11
#define EDIT_MENU_PASTE 12
#define EDIT_MENU_DELETE 13
#define EDIT_MENU_SEARCH_WITH_BING 14
#define EDIT_MENU_FIND 15
#define EDIT_MENU_FIND_NEXT 16
#define EDIT_MENU_FIND_PREVIOUS 17
#define EDIT_MENU_REPLACE 18
#define EDIT_MENU_GOTO 19
#define EDIT_MENU_SELECT_ALL 20
#define EDIT_MENU_TIME_DATE 21

#define TEXT_AREA 22

#define FIND_CANCEL 23
#define FIND_FIND_NEXT 24

// make file_opened
char* file_path;


HWND hMainWindow, hEdit, hFindWindow, hFindEdit;
HMENU hMenu;
HBRUSH g_hTitleBarBrush;
COLORREF g_titleBarColor = RGB(0, 0, 0); // Black color

void AddMenus(HWND hWnd);
void SetTitleBarColor(COLORREF color);
void AddControls(HWND hWnd);
void Open(HWND hWnd);
void display_file(char *path);
void undo();
void cut();
void copy();
void paste();
void del();
void select_all();
int find();
void FindText(HWND hWnd, const char *searchText);
void writeFile(char *path);
void saveAs(HWND hWnd);
void save();

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK FindProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    WNDCLASSW fnd = {0};
    fnd.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(255, 255, 255));
    fnd.hCursor = LoadCursor(NULL, IDC_ARROW);
    fnd.hInstance = hInst;
    fnd.lpszClassName = L"findClass";
    fnd.lpfnWndProc = FindProcedure;

    if (!RegisterClassW(&fnd))
        return -1;

    if (!RegisterClassW(&wc))
        return -1;

    hMainWindow = CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, NULL, NULL);

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
        case FILE_MENU_NEW:
            MessageBeep(MB_ICONINFORMATION);
            break;
        case FILE_MENU_NEW_WINDOW:
            break;
        case FILE_MENU_OPEN:
            Open(hWnd);
            break;
        case FILE_MENU_SAVE:
            save();
            break;
        case FILE_MENU_SAVE_AS:
            saveAs(hWnd);
            break;
        case FILE_MENU_PAGE_SETUP:
            break;
        case FILE_MENU_PRINT:
            break;
        case FILE_MENU_EXIT:
            DestroyWindow(hWnd);
            break;
        case EDIT_MENU_UNDO:
            undo();
            break;
        case EDIT_MENU_CUT:
            cut();
            break;
        case EDIT_MENU_COPY:
            copy();
            break;
        case EDIT_MENU_PASTE:
            paste();
            break;
        case EDIT_MENU_DELETE:
            del();
            break;
        case EDIT_MENU_SEARCH_WITH_BING:
            break;
        case EDIT_MENU_FIND:
            find();
            break;
        case EDIT_MENU_FIND_NEXT:
            break;
        case EDIT_MENU_FIND_PREVIOUS:
            break;
        case EDIT_MENU_REPLACE:
            break;
        case EDIT_MENU_GOTO:
            break;
        case EDIT_MENU_SELECT_ALL:
            select_all();
            break;
        case EDIT_MENU_TIME_DATE:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        AddControls(hWnd);
        AddMenus(hWnd);
        // SetTitleBarColor(g_titleBarColor);
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DWMCOLORIZATIONCOLORCHANGED:

        break;
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    {
        HDC hdcStatic = (HDC)wp;
        SetTextColor(hdcStatic, RGB(255, 255, 255));
        SetBkColor(hdcStatic, g_titleBarColor);
        return (INT_PTR)g_hTitleBarBrush;
    }
    case WM_SIZE:
    {
        // Retrieve the client area dimensions
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        int clientWidth = clientRect.right - clientRect.left;
        int clientHeight = clientRect.bottom - clientRect.top;

        // Calculate the new size and position of the text box
        int textBoxWidth = clientWidth;        // Adjust the desired width
        int textBoxHeight = clientHeight - 20; // Adjust the desired height
        int textBoxX = 5;                      // Adjust the desired X position
        int textBoxY = 0;                      // Adjust the desired Y position

        // Move and resize the text box
        HWND hTextBox = GetDlgItem(hWnd, TEXT_AREA); // Replace IDC_TEXT_BOX with your text box control ID
        MoveWindow(hTextBox, textBoxX, textBoxY, textBoxWidth, textBoxHeight, TRUE);

        // Redraw the window
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
    }
    break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void AddMenus(HWND hWnd)
{

    // create master menu
    hMenu = CreateMenu();

    // create submenus
    HMENU hFileMenu = CreateMenu();
    HMENU hEditMenu = CreateMenu();
    HMENU hFormatMenu = CreateMenu();
    HMENU hViewMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    // File menu
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_NEW, "Exit");
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_NEW_WINDOW, "New Window");
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_OPEN, "Open");
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_SAVE, "Save");
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_SAVE_AS, "Save As");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, 0);
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_PAGE_SETUP, "Page Setup");
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_PRINT, "Print");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, 0);
    AppendMenuA(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

    // Edit menu
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_UNDO, "Undo");
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, 0);
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_CUT, "Cut");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_COPY, "Copy");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_PASTE, "Paste");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_DELETE, "Delete");
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, 0);
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_SEARCH_WITH_BING, "Search with Bing");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_FIND, "Find");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_FIND_NEXT, "Find Next");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_REPLACE, "Replace");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_GOTO, "Go To");
    AppendMenu(hEditMenu, MF_SEPARATOR, 0, 0);
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_SELECT_ALL, "Select All");
    AppendMenuA(hEditMenu, MF_STRING, EDIT_MENU_TIME_DATE, "Time/Date");

    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hEditMenu, "Edit");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hFormatMenu, "Format");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hViewMenu, "View");
    AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "Help");

    SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd)
{
    // Create edit control that is always the size of the window
    hEdit = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_NOHIDESEL, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hWnd, (HMENU)TEXT_AREA, NULL, NULL);
}

/*
void SetTitleBarColor(COLORREF color)
{
    g_titleBarColor = color;

    // Create a solid brush with the specified color
    g_hTitleBarBrush = CreateSolidBrush(g_titleBarColor);

    // Set the colorization color using DWM API
    DWM_COLORIZATION_PARAMS params;
    params.ColorizationColor = g_titleBarColor;
    params.BlurColor = 0;             // You can set a different value if desired
    params.AfterglowColor = 0;        // You can set a different value if desired
    params.ColorizationOpacity = 100; // You can set a different value if desired

    DwmSetColorizationColor(&params, FALSE);

    SetTitleBarColor(hWnd, g_titleBarColor);
    // Invalidate and redraw the window
    RedrawWindow(hMainWindow, NULL, NULL, RDW_INVALIDATE);
}
*/

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

void Open(HWND hWnd)
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

    if (file_path != nullptr)
    {
        delete[] file_path;
    }

    // Allocate memory for file_path
    file_path = new char[ofn.nMaxFile];

    // Copy the contents of file_name to file_path
    strcpy(file_path, ofn.lpstrFile);

    display_file(ofn.lpstrFile);

    
}

void writeFile(char *path)
{
    FILE *file;
    file = fopen(path, "w");

    int len = GetWindowTextLength(hEdit);
    char *data = new char[len + 1];
    GetWindowText(hEdit, data, len + 1);

    fwrite(data, len + 1, 1, file);

    fclose(file);

    if (file_path != nullptr)
    {
        delete[] file_path;
        file_path = nullptr;
    }
}

void saveAs(HWND hWnd)
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

    if (file_path != nullptr)
    {
        delete[] file_path;
    }

    // Allocate memory for file_path
    file_path = new char[ofn.nMaxFile];

    // Copy the contents of file_name to file_path
    strcpy(file_path, ofn.lpstrFile);

    writeFile(ofn.lpstrFile);
}

void save()
{
    if (file_path == nullptr){
        saveAs(hMainWindow);
        return;
    } else {
        writeFile(file_path);
        return;
    }
}

void undo()
{
    SendMessage(hEdit, EM_UNDO, 0, 0);
}

void cut()
{
    SendMessage(hEdit, WM_CUT, 0, 0);
}

void copy()
{
    SendMessage(hEdit, WM_COPY, 0, 0);
}

void paste()
{
    SendMessage(hEdit, WM_PASTE, 0, 0);
}

void del()
{
    SendMessage(hEdit, WM_CLEAR, 0, 0);
}

void select_all()
{
    SendMessage(hEdit, EM_SETSEL, 0, -1);
}

int find()
{

    hFindWindow = CreateWindowW(L"findClass", L"Find", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 300, 100, NULL, NULL, NULL, NULL);

    MSG msg = {0};

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK FindProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        CreateWindowW(L"static", L"Find what:", WS_VISIBLE | WS_CHILD, 5, 5, 80, 25, hWnd, NULL, NULL, NULL);
        hFindEdit = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 90, 5, 100, 25, hWnd, NULL, NULL, NULL);
        CreateWindowW(L"button", L"Find", WS_VISIBLE | WS_CHILD, 195, 5, 80, 25, hWnd, (HMENU)FIND_FIND_NEXT, NULL, NULL);
        CreateWindowW(L"button", L"Cancel", WS_VISIBLE | WS_CHILD, 195, 35, 80, 25, hWnd, (HMENU)FIND_CANCEL, NULL, NULL);
        break;
    case WM_COMMAND:
        switch (wp)
        {
        case FIND_CANCEL:
            DestroyWindow(hWnd);
            break;
        case FIND_FIND_NEXT:
            // Find button clicked
            //hEdit = FindWindowEx(hWnd, NULL, "Edit", NULL);
            char searchText[100];
            GetWindowText(hFindEdit, searchText, sizeof(searchText));
            FindText(hWnd, searchText);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        DestroyWindow(hWnd);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
    return 0;
}

void FindText(HWND hWnd, const char* searchText)
{
    int len = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, 0, 5);
    char *data = new char[len + 1];
    char *datacopy = new char[len + 1];
    GetWindowText(hEdit, datacopy, len+1);
    GetWindowText(hEdit, data, len + 1);

    while (len > 0)
    {
        // Find the next occurrence of searchText within data
        char *pos = strstr(data, searchText);
        if (pos == NULL)
        {
            return;
        }
        else
        {
            // Calculate the index of the found occurrence
            int index = pos + data;
            std::cout << index << std::endl;

            // Highlight the found occurrence
            SendMessage(hEdit, EM_SETSEL, index, index + strlen(searchText));

            // Remove the found occurrence from data
            memmove(data, data + index + strlen(searchText), len - index - strlen(searchText) + 1);

            

            // Update data and len for the next iteration
            //data = data + index + strlen(searchText) + 1;

            // update data
            data = datacopy + index + strlen(searchText);

            std::cout << data << std::endl;
            // update len
            len = strlen(data);
            std::cout << len << std::endl;
            //std::cout << data << std::endl;
        }
    }
    /*
    char *pos = strstr(data, searchText);

    //pos = std::find(data, data + len, searchText[0]);

    if (pos == NULL)
    {
        MessageBox(hWnd, "Text not found", "Notepad", MB_OK | MB_ICONINFORMATION);
    }
    else
    {
        int index = pos - data;
        SendMessage(hEdit, EM_SETSEL, index, index + strlen(searchText));
    }
    */
}
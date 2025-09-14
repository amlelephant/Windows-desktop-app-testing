#include <windows.h>
#include <string>
#include <vector>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

const int CELL_SIZE = 13; // Size of each cell
const int HEIGHT = 50;
const int WIDTH = 100;

struct cell
{
    char character;
    COLORREF foregroundColor;
    COLORREF backgroundColor;
    int x;
    int y;

    // Constructor to initialize cell
    cell(char ch, COLORREF fgColor, COLORREF bgColor, int xPos, int yPos)
        : character(ch), foregroundColor(fgColor), backgroundColor(bgColor), x(xPos), y(yPos) {}
};

std::vector<std::vector<cell>> cellMatrix;

void addText(string text){
    
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char *className = "NumberedCellsClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;
    wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        className,
        "Numbered Cells",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 700,
        NULL, NULL, hInstance, NULL);

    // Populate cellMatrix
    cellMatrix.resize(HEIGHT);
    for (int y = 0; y < HEIGHT; ++y)
    {
        cellMatrix[y].resize(WIDTH, cell('f', RGB(255, 255, 255), RGB(0, 0, 0), 0, 0));
        for (int x = 0; x < WIDTH; ++x)
        {
            cellMatrix[y][x].x = x;
            cellMatrix[y][x].y = y;
        }
    }

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw cells
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                RECT rect = {x * CELL_SIZE, y * CELL_SIZE, (x + 1) * CELL_SIZE, (y + 1) * CELL_SIZE};
                Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
                // Convert character to string and draw it
                std::string text(1, cellMatrix[y][x].character);
                DrawTextA(hdc, text.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }

        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

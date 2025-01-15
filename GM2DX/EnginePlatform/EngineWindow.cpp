#include "PreCompile.h"
#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

// ��Ƽ�÷������� ¥����
//#ifdef _WINDOWS
//#include <Windows.h>
//#elseif _������
//
//#elseif �ȵ���̵�
//#endif 

HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasss;
std::map<HWND, UEngineWindow*> UEngineWindow::AllWindows;
std::function<bool(HWND, UINT, WPARAM, LPARAM)> UEngineWindow::CustomProc = nullptr;
int WindowCount = 0;
// bool UEngineWindow::LoopActive = true;

void UEngineWindow::SetCustomProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> _CustomProc)
{
    CustomProc = _CustomProc;
}

LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (nullptr != CustomProc)
    {
        if (true == CustomProc(hWnd, message, wParam, lParam))
        {
            return true;
        }
    }

    //if (true == AllWindows.contains(hWnd))
    //{
    //    MSGASSERT("�������� �ʴ� �����찡 �޼����� ���Խ��ϴ�.");
    //}

    switch (message)
    {
    case WM_CREATE:
        ++WindowCount;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_SETFOCUS:
    {
        if (true == AllWindows.contains(hWnd))
        {
            // MSGASSERT("�������� �ʴ� �����찡 �޼����� ���Խ��ϴ�.");
            AllWindows[hWnd]->IsFocusValue = true;
        }
        UEngineDebug::OutPutString("F");
        // AllWindows[]
        //Window.IsFocus = true;
    }
    break;
    case WM_KILLFOCUS:
    {
        if (true == AllWindows.contains(hWnd))
        {
            // MSGASSERT("�������� �ʴ� �����찡 �޼����� ���Խ��ϴ�.");
            AllWindows[hWnd]->IsFocusValue = false;
        }
        UEngineDebug::OutPutString("K");
    }
    break;
    case WM_DESTROY:
        --WindowCount;
        if (0 >= WindowCount)
        {
            UEngineWindow::LoopActive = false;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;

    // ������ ������ ����� �Ѵٸ� ���⼭ �Ϸ��� �Ѱ�.
    // ����Ʈ ������ Ŭ���� ���
    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;
    CreateWindowClass(wcex);
}

int UEngineWindow::WindowMessageLoop(std::function<void()> _StartFunction, std::function<void()> _FrameFunction, std::function<void()> _EndFunction)
{
    MSG msg = MSG();

    if (nullptr != _StartFunction)
    {
        _StartFunction();
    }

    if (nullptr == _FrameFunction)
    {
        MSGASSERT("������Ʈ ����� ���ε� �Ǿ� ���� �ʽ��ϴ�.");
        return 0;
    }

    while (true == LoopActive)
    {
        if(0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (false == LoopActive)
        {
            break;
        }

        _FrameFunction();
    }

    if (nullptr != _EndFunction)
    {
        _EndFunction();
    }

    return (int)msg.wParam;
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // �Ϲ����� ���� ����

    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasss.end();
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasss.find(std::string(_Class.lpszClassName));

    // ckw
    if (EndIter != FindIter)
    {
        // std::string ErrorText = "���� �̸��� ������ Ŭ������ 2�� ����߽��ϴ�" + std::string(_Class.lpszClassName);

        // std::string ���ο� ��� �ִ� �ɹ����� => std::string => std::vector<char>
        // std::vector<char> char* = new char[100];
        // ErrorText const char* �������ִ� �Լ��� c_str()
        // const char* Text = ErrorText.c_str();
        MSGASSERT(std::string(_Class.lpszClassName) + " ���� �̸��� ������ Ŭ������ 2�� ����߽��ϴ�");
        return;
    }

    RegisterClassExA(&_Class);

    WindowClasss.insert(std::pair{ _Class.lpszClassName, _Class });
}

UEngineWindow::UEngineWindow() 
{
    
}

UEngineWindow::~UEngineWindow()
{
    // �������ϴ� ������ �ظ��ϸ� ������� ������ ������ ����.
    if (nullptr != WindowHandle)
    {
        DestroyWindow(WindowHandle);
        WindowHandle = nullptr;
    }
}

void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    if (nullptr != WindowHandle)
    {
        return;
    }

    if (false == WindowClasss.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + " ������� ���� Ŭ������ ������â�� ������� �߽��ϴ�");
        return;
    }

    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " ������ ������ �����߽��ϴ�.");
        return;
    }

    // �����찡 ��������� hdc�� ���⼭ ���� �̴ϴ�.
    HDC WindowMainDC = GetDC(WindowHandle);

    AllWindows.insert({ WindowHandle, this });
}

void UEngineWindow::Open(std::string_view _TitleName /*= "Window"*/)
{
    // �� window �ȸ���� ������ �ϳ�?
    if (0 == WindowHandle)
    {
        // �����
        Create(_TitleName);
    }

    if (0 == WindowHandle)
    {
        return;
    }

	// �ܼ��� ����â�� �����ִ� �͸��� �ƴ϶�
	ShowWindow(WindowHandle, SW_SHOW);
    UpdateWindow(WindowHandle);
	// ShowWindow(WindowHandle, SW_HIDE);
}

void UEngineWindow::SetWindowPosAndScale(FVector _Pos, FVector _Scale)
{
    WindowSize = _Scale;
    RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };

    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);
    ::SetWindowPos(WindowHandle, nullptr, _Pos.iX(), _Pos.iY(), Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER);
}

FVector UEngineWindow::GetMousePos()
{
    POINT MousePoint;

    GetCursorPos(&MousePoint);
    // ������â ��ġ�������� ���콺 �������� 
    ScreenToClient(WindowHandle, &MousePoint);

    return FVector(MousePoint.x, MousePoint.y);
}
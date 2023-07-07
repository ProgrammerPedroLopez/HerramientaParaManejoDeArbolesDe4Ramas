#include <windows.h>
#include <string>
#include <fstream>
#include "resource.h"

#pragma warning (disable : 4996)

HWND ghDlg = 0;

struct nodo 
{
    std::string texto;
    nodo* previo;
    std::string emocion;

    nodo* opcion1;
    std::string respuesta1;
    nodo* opcion2;
    std::string respuesta2;
    nodo* opcion3;
    std::string respuesta3;
    nodo* opcion4;
    std::string respuesta4;

    int ID;

    int valor_De_ajuste;
};

nodo* raiz;
nodo* actual;

std::ofstream archivo;

wchar_t* buffer_texto = nullptr;
wchar_t *buffer_emocion = nullptr;
wchar_t *buffer_respuesta1 = nullptr;
wchar_t *buffer_respuesta2 = nullptr;
wchar_t *buffer_respuesta3 = nullptr;
wchar_t *buffer_respuesta4 = nullptr;

char* c_buffer_texto = nullptr;
char* c_buffer_emocion = nullptr;
char* c_buffer_respuesta1 = nullptr;
char* c_buffer_respuesta2 = nullptr;
char* c_buffer_respuesta3 = nullptr;
char* c_buffer_respuesta4 = nullptr;
char* buffer = nullptr;
char* c_buffer_ID = nullptr;
char* c_buffer_valor_de_ajuste = nullptr;

void AddNode(HWND hwnd);
void AddNode(std::string texto, std::string emocion, std::string respuesta1, std::string respuesta2, std::string respuesta3, std::string respuesta4);
void AddNode(int ID, int valor_de_ajuste, char texto[], char emocion[], char respuesta1[], char respuesta2[], char respuesta3[], char respuesta4[]);
void AddNode(nodo*& ptr, nodo*& prev, int ID, int valor_de_ajuste, char texto[], char emocion[], char respuesta1[], char respuesta2[], char respuesta3[], char respuesta4[]);

void SaveNode(nodo* ptr);
void LoadNode(std::string line);

void SaveFile(LPWSTR path);
void LoadFile(LPWSTR path);

void funcion_guardar(HWND);
void funcion_abrir(HWND);

nodo* FindFatherNode(nodo*& ptr, int ID_father);

nodo* FindActiveNode(nodo*& ptr);

void DeleteTree();

void Clean(HWND hwnd);

void SetEditControls(HWND hwnd);

BOOL CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
    ghDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)WindowProc);
    ShowWindow(ghDlg, nCmdShow);
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));
    while (GetMessage(&msg, 0, 0, 0))
    {
        if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

BOOL CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_INITDIALOG:
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_SAVE:
            funcion_guardar(hwnd);
            break;

        case ID_FILE_LOAD:
            DeleteTree();
            funcion_abrir(hwnd);
            actual = raiz;
            if (actual != nullptr)
            {
                if (actual->previo != nullptr)
                {
                    actual = actual->previo;
                    Clean(hwnd);
                    SetEditControls(hwnd);
                }
            }
            break;

        case IDC_BUTTON1:
            AddNode(hwnd);
            break;

        case IDC_BUTTON2:
            if (actual->opcion1 == nullptr)
            {
                actual->opcion1 = new nodo;
                actual->opcion1->previo = actual;
                actual = actual->opcion1;
                actual->opcion1 = nullptr;
                actual->opcion2 = nullptr;
                actual->opcion3 = nullptr;
                actual->opcion4 = nullptr;
                actual->valor_De_ajuste = 1;
                actual->ID = (actual->previo->ID * 4) + actual->valor_De_ajuste;
                Clean(hwnd);
            }
            else 
            {
                actual = actual->opcion1;
                Clean(hwnd);
                SetEditControls(hwnd);
            }
            
            break;

        case IDC_BUTTON3:
            if (actual->opcion2 == nullptr)
            {
                actual->opcion2 = new nodo;
                actual->opcion2->previo = actual;
                actual = actual->opcion2;
                actual->opcion1 = nullptr;
                actual->opcion2 = nullptr;
                actual->opcion3 = nullptr;
                actual->opcion4 = nullptr;
                actual->valor_De_ajuste = 2;
                actual->ID = (actual->previo->ID * 4) + actual->valor_De_ajuste;
                Clean(hwnd);
            }
            else
            {
                actual = actual->opcion2;
                Clean(hwnd);
                SetEditControls(hwnd);
            }
            
            break;

        case IDC_BUTTON4:
            if (actual->opcion3 == nullptr)
            {
                actual->opcion3 = new nodo;
                actual->opcion3->previo = actual;
                actual = actual->opcion3;
                actual->opcion1 = nullptr;
                actual->opcion2 = nullptr;
                actual->opcion3 = nullptr;
                actual->opcion4 = nullptr;
                actual->valor_De_ajuste = 3;
                actual->ID = (actual->previo->ID * 4) + actual->valor_De_ajuste;
                Clean(hwnd);
            }
            else
            {
                actual = actual->opcion3;
                Clean(hwnd);
                SetEditControls(hwnd);
            }
            
            break;

        case IDC_BUTTON5:
            if (actual->opcion4 == nullptr)
            {
                actual->opcion4 = new nodo;
                actual->opcion4->previo = actual;
                actual = actual->opcion4;
                actual->opcion1 = nullptr;
                actual->opcion2 = nullptr;
                actual->opcion3 = nullptr;
                actual->opcion4 = nullptr;
                actual->valor_De_ajuste = 4;
                actual->ID = (actual->previo->ID * 4) + actual->valor_De_ajuste;
                Clean(hwnd);
            }
            else
            {
                actual = actual->opcion4;
                Clean(hwnd);

               /* mbstowcs(buffer_texto, actual->texto.c_str(), actual->texto.length());
                SetDlgItemText(hwnd, IDC_EDIT1, buffer_texto);
                mbstowcs(buffer_emocion, actual->emocion.c_str(), actual->emocion.length());
                SetDlgItemText(hwnd, IDC_EDIT6, buffer_emocion);
                mbstowcs(buffer_respuesta1, actual->respuesta1.c_str(), actual->respuesta1.length());
                SetDlgItemText(hwnd, IDC_EDIT2, buffer_respuesta1);
                mbstowcs(buffer_respuesta2, actual->respuesta2.c_str(), actual->respuesta2.length());
                SetDlgItemText(hwnd, IDC_EDIT3, buffer_respuesta2);
                mbstowcs(buffer_respuesta3, actual->respuesta3.c_str(), actual->respuesta3.length());
                SetDlgItemText(hwnd, IDC_EDIT4, buffer_respuesta3);
                mbstowcs(buffer_respuesta4, actual->respuesta4.c_str(), actual->respuesta4.length());
                SetDlgItemText(hwnd, IDC_EDIT5, buffer_respuesta4);*/
                SetEditControls(hwnd);
            }
            
            break;

        case IDC_BUTTON6:

            if (actual != nullptr) 
            {
                if (actual->previo != nullptr)
                {
                    actual = actual->previo;
                    Clean(hwnd);
                    SetEditControls(hwnd);
                }
            }
            
            break;
        }
        break;

    case WM_CLOSE:
        DeleteTree();
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    }

    return FALSE;
}

void AddNode(HWND hwnd)
{
    wchar_t buffer_size[10];
    int nChar = 0;

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT1, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_texto = new wchar_t[nChar + 1];
    c_buffer_texto = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT1, buffer_texto, nChar + 1);
    wcstombs(c_buffer_texto, buffer_texto, nChar + 1);

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT6, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_emocion = new wchar_t[nChar + 1];
    c_buffer_emocion = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT6, buffer_emocion, nChar + 1);
    wcstombs(c_buffer_emocion, buffer_emocion, nChar + 1);

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT2, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_respuesta1 = new wchar_t[nChar + 1];
    c_buffer_respuesta1 = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT2, buffer_respuesta1, nChar + 1);
    wcstombs(c_buffer_respuesta1, buffer_respuesta1, nChar + 1);

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT3, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_respuesta2 = new wchar_t[nChar + 1];
    c_buffer_respuesta2 = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT3, buffer_respuesta2, nChar + 1);
    wcstombs(c_buffer_respuesta2, buffer_respuesta2, nChar + 1);

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT4, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_respuesta3 = new wchar_t[nChar + 1];
    c_buffer_respuesta3 = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT4, buffer_respuesta3, nChar + 1);
    wcstombs(c_buffer_respuesta3, buffer_respuesta3, nChar + 1);

    nChar = SendDlgItemMessage(hwnd, IDC_EDIT5, WM_GETTEXTLENGTH, NULL, NULL);
    buffer_respuesta4 = new wchar_t[nChar + 1];
    c_buffer_respuesta4 = new char[nChar + 1];
    GetDlgItemText(hwnd, IDC_EDIT5, buffer_respuesta4, nChar + 1);
    wcstombs(c_buffer_respuesta4, buffer_respuesta4, nChar + 1);

    std::string texto = c_buffer_texto;
    std::string emocion = c_buffer_emocion;
    std::string respuesta1 = c_buffer_respuesta1;
    std::string respuesta2 = c_buffer_respuesta2;
    std::string respuesta3 = c_buffer_respuesta3;
    std::string respuesta4 = c_buffer_respuesta4;

    AddNode(texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);

    delete[] c_buffer_texto;
    delete[] c_buffer_emocion;
    delete[] c_buffer_respuesta1;
    delete[] c_buffer_respuesta2;
    delete[] c_buffer_respuesta3;
    delete[] c_buffer_respuesta4;

    delete[] buffer_texto;
    delete[] buffer_emocion;
    delete[] buffer_respuesta1;
    delete[] buffer_respuesta2;
    delete[] buffer_respuesta3;
    delete[] buffer_respuesta4;
}

void AddNode(std::string texto, std::string emocion, std::string respuesta1, std::string respuesta2, std::string respuesta3, std::string respuesta4)
{
    if (raiz == nullptr)
    {
        raiz = new nodo;
        raiz->texto = texto;
        raiz->emocion = emocion;
        raiz->respuesta1 = respuesta1;
        raiz->respuesta2 = respuesta2;
        raiz->respuesta3 = respuesta3;
        raiz->respuesta4 = respuesta4;
        raiz->opcion1 = nullptr;
        raiz->opcion2 = nullptr;
        raiz->opcion3 = nullptr;
        raiz->opcion4 = nullptr;
        raiz->previo = nullptr;
        raiz->ID = 0;
        raiz->valor_De_ajuste = 0;
        actual = raiz;
    }
    else
    {
        actual->texto = texto;
        actual->emocion = emocion;
        actual->respuesta1 = respuesta1;
        actual->respuesta2 = respuesta2;
        actual->respuesta3 = respuesta3;
        actual->respuesta4 = respuesta4;
    }
}

void AddNode(int ID, int valor_de_ajuste, char texto[], char emocion[], char respuesta1[], char respuesta2[], char respuesta3[], char respuesta4[])
{
    if (ID == 0)
    {
        AddNode(raiz, raiz, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    /*if (ID == 1)
    {
        AddNode(raiz->opcion1, raiz, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (ID == 2)
    {
        AddNode(raiz->opcion2, raiz, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (ID == 3)
    {
        AddNode(raiz->opcion3, raiz, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (ID == 4)
    {
        AddNode(raiz->opcion4, raiz, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }*/

    int rango = 0;
    actual = raiz;

    if ((rango * 4) + 1 <= ID && ID <= (rango * 4) + 4)
    {
        if (valor_de_ajuste == 1)
        {
            AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 2)
        {
            AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 3)
        {
            AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 4)
        {
            AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }
    }

    rango++;
    if ((rango * 4) + 1 <= ID && ID <= (rango * 4) + 4)
    {
        actual = actual->opcion1;
        if (valor_de_ajuste == 1)
        {
            AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 2)
        {
            AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 3)
        {
            AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 4)
        {
            AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }
    }

    rango++;
    if ((rango * 4) + 1 <= ID && ID <= (rango * 4) + 4)
    {
        actual = actual->opcion2;
        if (valor_de_ajuste == 1)
        {
            AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 2)
        {
            AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 3)
        {
            AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 4)
        {
            AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }
    }

    rango++;
    if ((rango * 4) + 1 <= ID && ID <= (rango * 4) + 4)
    {
        actual = actual->opcion3;
        if (valor_de_ajuste == 1)
        {
            AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 2)
        {
            AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 3)
        {
            AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 4)
        {
            AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }
    }

    rango++;
    if ((rango * 4) + 1 <= ID && ID <= (rango * 4) + 4)
    {
        actual = actual->opcion4;
        if (valor_de_ajuste == 1)
        {
            AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 2)
        {
            AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 3)
        {
            AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }

        if (valor_de_ajuste == 4)
        {
            AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
            return;
        }
    }

    //Logica para cuando el ID mayor a 21
    int ID_father = (ID - valor_de_ajuste) / 4; //ID of previous node
    actual = raiz;

    actual = FindFatherNode(raiz, ID_father);

    if (valor_de_ajuste == 1)
    {
        AddNode(actual->opcion1, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (valor_de_ajuste == 2)
    {
        AddNode(actual->opcion2, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (valor_de_ajuste == 3)
    {
        AddNode(actual->opcion3, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }

    if (valor_de_ajuste == 4)
    {
        AddNode(actual->opcion4, actual, ID, valor_de_ajuste, texto, emocion, respuesta1, respuesta2, respuesta3, respuesta4);
        return;
    }
}

void AddNode(nodo*& ptr, nodo*& prev, int ID, int valor_de_ajuste, char texto[], char emocion[], char respuesta1[], char respuesta2[], char respuesta3[], char respuesta4[])
{
    ptr = new nodo;
    ptr->ID = ID;
    ptr->valor_De_ajuste = valor_de_ajuste;
    ptr->texto = texto;
    ptr->emocion = emocion;
    ptr->respuesta1 = respuesta1;
    ptr->respuesta2 = respuesta2;
    ptr->respuesta3 = respuesta3;
    ptr->respuesta4 = respuesta4;
    ptr->previo = prev;
    ptr->opcion1 = nullptr;
    ptr->opcion2 = nullptr;
    ptr->opcion3 = nullptr;
    ptr->opcion4 = nullptr;
    actual = ptr;
}

void SaveNode(nodo* ptr)
{
    if (ptr == nullptr)
    {
        return;
    }

    archivo << ptr->ID << '\t' << ptr->valor_De_ajuste << '\t' << ptr->texto << '\t' << ptr->emocion << '\t' << ptr->respuesta1 << '\t' << ptr->respuesta2 << '\t' << ptr->respuesta3 << '\t' << ptr->respuesta4 << std::endl;
    SaveNode(ptr->opcion1);
    SaveNode(ptr->opcion2);
    SaveNode(ptr->opcion3);
    SaveNode(ptr->opcion4);
}

void LoadNode(std::string line)
{
    buffer = new char[line.length() + 1];
    ZeroMemory(buffer, line.length() + 1);
    strcpy(buffer, line.c_str());
    ///codigo para leer la linea y ponerla en un nodo

    int k = 0;
    while (buffer[k] != '\t')
    {
        k++;
    }
    c_buffer_ID = new char[k + 1];
    ZeroMemory(c_buffer_ID, k + 1);
    int i = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_ID[i] = buffer[i];
        i++;
    }

    k++;
    int l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_valor_de_ajuste = new char[l + 1];
    ZeroMemory(c_buffer_valor_de_ajuste, l + 1);
    int j = 0;
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_valor_de_ajuste[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_texto = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_texto, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_texto[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_emocion = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_emocion, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_emocion[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_respuesta1 = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_respuesta1, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_respuesta1[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_respuesta2 = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_respuesta2, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_respuesta2[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\t')
    {
        k++;
        l++;
    }
    c_buffer_respuesta3 = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_respuesta3, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_respuesta3[j] = buffer[i];
        i++;
        j++;
    }

    k++;
    l = 0;
    while (buffer[k] != '\0')
    {
        k++;
        l++;
    }
    c_buffer_respuesta4 = new char[l + 1];
    j = 0;
    ZeroMemory(c_buffer_respuesta4, l + 1);
    while (buffer[i] != '\0')
    {
        if (buffer[i] == '\t')
        {
            i++;
            break;
        }
        c_buffer_respuesta4[j] = buffer[i];
        i++;
        j++;
    }

    AddNode(atoi(c_buffer_ID), atoi(c_buffer_valor_de_ajuste), c_buffer_texto, c_buffer_emocion, c_buffer_respuesta1, c_buffer_respuesta2, c_buffer_respuesta3, c_buffer_respuesta4);

    delete[] buffer;
    delete[] c_buffer_ID;
    delete[] c_buffer_valor_de_ajuste;
    delete[] c_buffer_texto;
    delete[] c_buffer_emocion;
    delete[] c_buffer_respuesta1;
    delete[] c_buffer_respuesta2;
    delete[] c_buffer_respuesta3;
    delete[] c_buffer_respuesta4;

    buffer = nullptr;
    c_buffer_ID = nullptr;
    c_buffer_valor_de_ajuste = nullptr;
    c_buffer_texto = nullptr;
    c_buffer_emocion = nullptr;
    c_buffer_respuesta1 = nullptr;
    c_buffer_respuesta2 = nullptr;
    c_buffer_respuesta3 = nullptr;
    c_buffer_respuesta4 = nullptr;
}

void SaveFile(LPWSTR path)
{
    if (raiz == nullptr)
    {
        return;
    }
    actual = raiz;

    archivo.open(path, std::ios::trunc);
    SaveNode(actual);
    archivo.close();
}

void LoadFile(LPWSTR path)
{
    std::ifstream MyFile(path);
    std::string line;
    if (MyFile.is_open())
    {
        while (std::getline(MyFile, line))
        {
            LoadNode(line);
        }
        MyFile.close();
    }
    else
    {
        MessageBox(NULL, L"Archivo no existe", L"Error", MB_ICONERROR);
    }
}

void funcion_guardar(HWND padre) 
{
    OPENFILENAME openfilename;

    TCHAR szFile[MAX_PATH + 1];
    ZeroMemory(szFile, sizeof(szFile));

    ZeroMemory(&openfilename, sizeof(OPENFILENAME));
    openfilename.lStructSize = sizeof(OPENFILENAME);
    openfilename.hwndOwner = padre;
    openfilename.lpstrFile = szFile;
    openfilename.nMaxFile = MAX_PATH;
    openfilename.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
    openfilename.nFilterIndex = 1;
    openfilename.lpstrFileTitle = NULL;
    openfilename.nMaxFileTitle = 0;
    openfilename.lpstrInitialDir = NULL;
    openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetSaveFileName(&openfilename)) 
    {
        SaveFile(openfilename.lpstrFile);
    }
    else 
    {
        MessageBox(NULL, TEXT("Could not save this file"), TEXT("File I/O Error"), MB_ICONSTOP);
    }
}

void funcion_abrir(HWND padre) 
{
    OPENFILENAME openfilename;

    TCHAR szFile[MAX_PATH + 1];
    ZeroMemory(szFile, sizeof(szFile));

    ZeroMemory(&openfilename, sizeof(OPENFILENAME));
    openfilename.lStructSize = sizeof(OPENFILENAME);
    openfilename.hwndOwner = padre;
    openfilename.lpstrFile = szFile;
    openfilename.nMaxFile = MAX_PATH;
    openfilename.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
    openfilename.nFilterIndex = 1;
    openfilename.lpstrFileTitle = NULL;
    openfilename.nMaxFileTitle = 0;
    openfilename.lpstrInitialDir = NULL;
    openfilename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&openfilename))
    {
        LoadFile(openfilename.lpstrFile);
    }
    else
    {
        MessageBox(NULL, TEXT("Could not open this file"), TEXT("File I/O Error"), MB_ICONSTOP);
    }
}

nodo* FindFatherNode(nodo*& ptr, int ID_father)
{
    nodo* retorno = nullptr;

    if (ptr->ID == ID_father)
    {
        return ptr;
    }
    else
    {
        if (ptr->opcion1 != nullptr)
        {
            retorno = FindFatherNode(ptr->opcion1, ID_father);
        }

        if (ptr->opcion2 != nullptr)
        {
            retorno = FindFatherNode(ptr->opcion2, ID_father);
        }

        if (ptr->opcion3 != nullptr)
        {
            retorno = FindFatherNode(ptr->opcion3, ID_father);
        }

        if (ptr->opcion4 != nullptr)
        {
            retorno = FindFatherNode(ptr->opcion4, ID_father);
        }

        return retorno;
    }
}

nodo* FindActiveNode(nodo*& ptr)
{
    nodo* retorno = nullptr;

    if (ptr == nullptr) 
    {
        return nullptr;
    }

    if (ptr->opcion1 == nullptr && ptr->opcion2 == nullptr && ptr->opcion3 == nullptr && ptr->opcion4 == nullptr)
    {
        return ptr;
    }
    else 
    {
        if (ptr->opcion1 != nullptr)
        {
            retorno = FindActiveNode(ptr->opcion1);
        }

        if (ptr->opcion2 != nullptr)
        {
            retorno = FindActiveNode(ptr->opcion2);
        }

        if (ptr->opcion3 != nullptr)
        {
            retorno = FindActiveNode(ptr->opcion3);
        }

        if (ptr->opcion4 != nullptr)
        {
            retorno = FindActiveNode(ptr->opcion4);
        }
    }
    return retorno;
}

void DeleteTree() 
{
    actual = raiz;

    nodo* aux = nullptr;
    nodo* temp = nullptr;

    while (true)
    {
        aux = FindActiveNode(actual);
        if (aux == raiz) 
        {
            delete raiz;
            raiz = nullptr;
            break;
        }
        if (aux != nullptr) 
        {
            if (aux->valor_De_ajuste == 1) { temp = aux->previo;  delete aux; temp->opcion1 = nullptr; }
            if (aux->valor_De_ajuste == 2) { temp = aux->previo;  delete aux; temp->opcion2 = nullptr; }
            if (aux->valor_De_ajuste == 3) { temp = aux->previo;  delete aux; temp->opcion3 = nullptr; }
            if (aux->valor_De_ajuste == 4) { temp = aux->previo;  delete aux; temp->opcion4 = nullptr; }
            
        }
    }

    
}

void Clean(HWND hwnd)
{
    SetDlgItemText(hwnd, IDC_EDIT1, L"");
    SetDlgItemText(hwnd, IDC_EDIT6, L"");
    SetDlgItemText(hwnd, IDC_EDIT2, L"");
    SetDlgItemText(hwnd, IDC_EDIT3, L"");
    SetDlgItemText(hwnd, IDC_EDIT4, L"");
    SetDlgItemText(hwnd, IDC_EDIT5, L"");
}

void SetEditControls(HWND hwnd)
{
    buffer_texto = new wchar_t[actual->texto.length() + 1];
    ZeroMemory(buffer_texto, (sizeof(wchar_t) * actual->texto.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_texto, actual->texto.c_str(), actual->texto.length());
    SetDlgItemText(hwnd, IDC_EDIT1, buffer_texto);
    delete[] buffer_texto;
    buffer_texto = nullptr;

    buffer_emocion = new wchar_t[actual->emocion.length() + 1];
    ZeroMemory(buffer_emocion, (sizeof(wchar_t) * actual->emocion.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_emocion, actual->emocion.c_str(), actual->emocion.length());
    SetDlgItemText(hwnd, IDC_EDIT6, buffer_emocion);
    delete[] buffer_emocion;
    buffer_emocion = nullptr;

    buffer_respuesta1 = new wchar_t[actual->respuesta1.length() + 1];
    ZeroMemory(buffer_respuesta1, (sizeof(wchar_t) * actual->respuesta1.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_respuesta1, actual->respuesta1.c_str(), actual->respuesta1.length());
    SetDlgItemText(hwnd, IDC_EDIT2, buffer_respuesta1);
    delete[] buffer_respuesta1;
    buffer_respuesta1 = nullptr;

    buffer_respuesta2 = new wchar_t[actual->respuesta2.length() + 1];
    ZeroMemory(buffer_respuesta2, (sizeof(wchar_t) * actual->respuesta2.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_respuesta2, actual->respuesta2.c_str(), actual->respuesta2.length());
    SetDlgItemText(hwnd, IDC_EDIT3, buffer_respuesta2);
    delete[] buffer_respuesta2;
    buffer_respuesta2 = nullptr;

    buffer_respuesta3 = new wchar_t[actual->respuesta3.length() + 1];
    ZeroMemory(buffer_respuesta3, (sizeof(wchar_t) * actual->respuesta3.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_respuesta3, actual->respuesta3.c_str(), actual->respuesta3.length());
    SetDlgItemText(hwnd, IDC_EDIT4, buffer_respuesta3);
    delete[] buffer_respuesta3;
    buffer_respuesta3 = nullptr;

    buffer_respuesta4 = new wchar_t[actual->respuesta4.length() + 1];
    ZeroMemory(buffer_respuesta4, (sizeof(wchar_t) * actual->respuesta4.length()) + (sizeof(wchar_t) * 1));
    mbstowcs(buffer_respuesta4, actual->respuesta4.c_str(), actual->respuesta4.length());
    SetDlgItemText(hwnd, IDC_EDIT5, buffer_respuesta4);
    delete[] buffer_respuesta4;
    buffer_respuesta4 = nullptr;
}

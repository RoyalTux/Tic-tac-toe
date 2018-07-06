#include "game.h"
#include <tchar.h>
#include <string>
using namespace std;

INT sec = 0, min = 0; TCHAR str[100];
static const int length = 9;
static game Game;

void time()
{
	if (sec == 60)
	{
		min++;
		sec = 0;
	}
	if (sec < 10 && min < 10)
	{
		wsprintf(str, TEXT("0%d:0%d"), min, sec);
		sec++;
	}
	else if (sec >= 10)
	{
		wsprintf(str, TEXT("0%d:%d"), min, sec);
		sec++;
	}
	else if (sec >= 10 && min >= 10)
	{
		wsprintf(str, TEXT("%d:0%d"), min, sec);
		sec++;
	}
}

void tErase()
{
	sec = 0; min = 0;
}

BOOL CALLBACK MenuEventHandler(HWND DialogDescriptor, UINT Message, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE ApplicationDescriptor, HINSTANCE PreviousDescriptor, LPSTR CommandLine, int CmdShow)
{
	DialogBox(ApplicationDescriptor, MAKEINTRESOURCE(IDD_MAIN), NULL, MenuEventHandler);
	return 0;
}

BOOL CALLBACK MenuEventHandler(HWND DialogDescriptor, UINT Message, WPARAM wParam, LPARAM lParam)
{
	int res;
	static bool PC = false;
	static HICON hIcon;
	static HBITMAP X = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2));
	static HBITMAP O = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
	static DWORD BUTTONS[length] = { IDC_BUTTON1, IDC_BUTTON2, IDC_BUTTON3, IDC_BUTTON4,
		IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8, IDC_BUTTON9 };
	static HWND hPlay, hPause;
	switch (Message)
	{
	case WM_CLOSE:
		MessageBox(DialogDescriptor, TEXT("Версия 1.0"), TEXT(" Автор - Щебетовский Д.Г. ЕКО П2"), MB_ICONINFORMATION | MB_OK);
		res = MessageBox(DialogDescriptor, TEXT("Вы уверены?"), TEXT("Выход"), MB_OKCANCEL | MB_ICONQUESTION | MB_DEFBUTTON2);
		if (res == IDOK)
		{
			EndDialog(DialogDescriptor, 0);
		}
		return TRUE;
	case WM_INITDIALOG:
		hPlay = GetDlgItem(DialogDescriptor, IDC_BUTTON10);
		hPause = GetDlgItem(DialogDescriptor, IDC_BUTTON11);
		EnableWindow(hPause, FALSE);
		EnableWindow(hPlay, FALSE);
		//
		hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		SendMessage(DialogDescriptor, WM_SETICON, 1, (LPARAM)hIcon);
		CheckDlgButton(DialogDescriptor, IDC_RADIO_O, BST_CHECKED);
		Game.ClearMap();
		tErase();
		SetDlgItemText(DialogDescriptor, IDC_GROUP, (string() + "Текущий знак: \"" + Game.GetCurrentSymbol() + "\"").c_str());
		return TRUE;
	case WM_TIMER:
	{
		time();
		SetWindowText(DialogDescriptor, str);
		if (PC == true && !IsWindowEnabled(GetDlgItem(DialogDescriptor, IDC_BUTTON_MODE)))
		{
			PC = false;
			int i = Game.GetMove();
			if (i == -1)
				MessageBox(DialogDescriptor, "Ошибка ПК", "ОШИБКА", MB_ICONERROR | MB_OK);
			if (Game.GetCurrentSymbol() == 'X')
				SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(X));
			else
				SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(O));
			if (Game.CheckWin() || Game.CheckDraw())
			{
				if (Game.CheckWin())
				{
					KillTimer(DialogDescriptor, 1);
					res = MessageBox(DialogDescriptor, (string("Выиграл: \"") + Game.GetCurrentSymbol() + "\"" + " Время игры: " + str + " Продолжить?").c_str(), "Конец игры", MB_ICONINFORMATION | MB_OKCANCEL);
					if (res == IDCANCEL)
					{
						EndDialog(DialogDescriptor, 0);
					}
					else tErase();
				}
				else
				{
					KillTimer(DialogDescriptor, 1);
					res = MessageBox(DialogDescriptor, (string("Ничья!  Время игры: ") + str + " Продолжить?").c_str(), "Конец игры", MB_ICONINFORMATION | MB_OKCANCEL);
					if (res == IDCANCEL)
					{
						EndDialog(DialogDescriptor, 0);
					}
					else tErase();
				}
				for (int i = 0; i < length; i++)
				{
					SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), NULL);
				}
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_X), FALSE);
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_O), FALSE);
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_BUTTON_MODE), FALSE);
				Game.ClearMap();
				return TRUE;
			}
			Game.SetCurrentSymbol(Game.Next());
			SetDlgItemText(DialogDescriptor, IDC_GROUP, (string() + "Текущий знак: \"" + Game.GetCurrentSymbol() + "\"").c_str());
		}
		return TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_RADIO_X && IsDlgButtonChecked(DialogDescriptor, IDC_RADIO_X))
		{
			Game.SetCurrentSymbol('X');
			SetDlgItemText(DialogDescriptor, IDC_GROUP, (string() + "Текущий знак: \"" + Game.GetCurrentSymbol() + "\"").c_str());
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_RADIO_O && IsDlgButtonChecked(DialogDescriptor, IDC_RADIO_O))
		{
			Game.SetCurrentSymbol('O');
			SetDlgItemText(DialogDescriptor, IDC_GROUP, (string() + "Текущий знак: \"" + Game.GetCurrentSymbol() + "\"").c_str());
			return TRUE;
		}
		if (LOWORD(wParam) == IDC_BUTTON_MODE)
		{
			if (PC == true)
			{
				PC = false;
				SetDlgItemText(DialogDescriptor, IDC_BUTTON_MODE, "Игрок");
			}
			else
			{
				PC = true;
				SetDlgItemText(DialogDescriptor, IDC_BUTTON_MODE, "ПК");
				SetTimer(DialogDescriptor, 1, 1000, NULL);
			}
			return TRUE;
		}
		if (PC == false)
		{
			for (int i = 0; i <length; i++)
			{
				if (LOWORD(wParam) == BUTTONS[i])
				{
					if (SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_GETIMAGE, NULL, NULL) == NULL)
					{
						if (Game.GetCurrentSymbol() == 'X')
						{
							SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(X));
						}
						else
							SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), LPARAM(O));
						Game.SetMap(i, Game.GetCurrentSymbol());
					}
				}
			}
			if (Game.CheckWin() || Game.CheckDraw())
			{
				if (Game.CheckWin())
				{
					KillTimer(DialogDescriptor, 1);
					res = MessageBox(DialogDescriptor, (string("Выиграл: \"") + Game.GetCurrentSymbol() + "\"" + "Время игры: " + str + " Продолжить?").c_str(), "Конец игры", MB_ICONINFORMATION | MB_OKCANCEL);
					if (res == IDCANCEL)
					{
						EndDialog(DialogDescriptor, 0);
					}
					else tErase();
				}
				else
				{
					KillTimer(DialogDescriptor, 1);;
					res = MessageBox(DialogDescriptor, (string("Ничья!  Время игры: ") + str + " Продолжить?").c_str(), "Конец игры", MB_ICONINFORMATION | MB_OKCANCEL);
					if (res == IDCANCEL)
					{
						EndDialog(DialogDescriptor, 0);
					}
					else tErase();
				}
				for (int i = 0; i < length; i++)
				{
					SendMessage(GetDlgItem(DialogDescriptor, BUTTONS[i]), BM_SETIMAGE, WPARAM(IMAGE_BITMAP), NULL);
				}
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_X), FALSE);
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_O), FALSE);
				EnableWindow(GetDlgItem(DialogDescriptor, IDC_BUTTON_MODE), FALSE);
				Game.ClearMap();
				KillTimer(DialogDescriptor, 1);
				return TRUE;
			}
			Game.SetCurrentSymbol(Game.Next());
			SetDlgItemText(DialogDescriptor, IDC_GROUP, (string() + "Текущий знак: \"" + Game.GetCurrentSymbol() + "\"").c_str());
		}
		if (IsWindowEnabled(GetDlgItem(DialogDescriptor, IDC_BUTTON_MODE)))
		{
			EnableWindow(GetDlgItem(DialogDescriptor, IDC_BUTTON_MODE), FALSE);
			EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_X), FALSE);
			EnableWindow(GetDlgItem(DialogDescriptor, IDC_RADIO_O), FALSE);
		}
		PC = true;
		SetTimer(DialogDescriptor, 1, 1000, NULL);
		return TRUE;
	}
	return FALSE;
}

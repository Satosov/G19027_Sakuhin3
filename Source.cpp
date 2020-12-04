#include "DxLib.h"

#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	500
#define COLOR_BIT		32
#define FPS				60

#define WINDOW_BAR_ON	0
#define GAME_TITLE		"���i���鋐�啺��"



VOID MY_START(VOID);
VOID MY_PLAY(VOID);
VOID MY_END(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);		//�L�[�̓��͏�Ԃ��X�V
BOOL MY_KEY_DOWN(int);
BOOL MY_KEY_UP(int);
BOOL MY_KEYDOWN_KEEP(int, int);


char AllKeyState[256] = { '\0' };
char OldAllKeyState[256] = { '\0' };
int gamescene;

enum SCENE
{
	SCENESTART = 1,
	SCENEPLAYING = 2,
	SCENEEND = 3,
	SCENEESCAPE = 4
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);									//�E�B���h�E���[�h�ɐݒ肷��
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BIT);	//�c500�A��1000�A�J���[�r�b�g32�ŃE�B���h�E��\������
	SetWindowStyleMode(WINDOW_BAR_ON);						//�^�C�g���o�[���f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_TITLE));					//�^�C�g��
	SetAlwaysRunFlag(TRUE);									//��A�N�e�B�u�ł����s����

	if (DxLib_Init() == 1) { return -1; }					//DxLib����������
	SetDrawScreen(DX_SCREEN_BACK);							//Draw�n�̊֐��𗠉�ʂɕ`�悷��
	
	gamescene = SCENESTART;

	while (TRUE)
	{
		ClearDrawScreen();
		MY_ALL_KEYDOWN_UPDATE();
		if (ClearDrawScreen() != 0) { break; }

		switch (gamescene)
		{
		case SCENESTART:
			DrawString(0, 0, "start\n", RGB(255, 255, 255));
			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				gamescene = SCENEPLAYING;
			}
			break;
		case SCENEPLAYING:
			DrawString(0, 0, "play\n", RGB(255, 255, 255));
			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				gamescene = SCENEEND;
			}
			break;
		case SCENEEND:
			DrawString(0, 0, "end\n", RGB(255, 255, 255));
			if (AllKeyState[KEY_INPUT_RETURN] == 1)
			{
				gamescene = SCENESTART;
			}
			break;
		}
		WaitTimer(1000 / FPS);
		ScreenFlip();

		if (AllKeyState[KEY_INPUT_ESCAPE] == 1)
			break;
	}

	DxLib_End();

	return 0;
}

VOID MY_ALL_KEY_DOWN_UP(VOID)
{
	char TempKey[256];

	for (int i = 0; i < 256; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}
	GetHitKeyStateAll(TempKey);

	for (int i = 0; i < 256; i++)
	{
		if (TempKey[i] != 0)
		{
			AllKeyState[i]++;
		}
		if (TempKey[i] != 0)
		{
			AllKeyState[i] = 0;
		}
	}
	return;
}

BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1
		&& AllKeyState[KEY_INPUT_] == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int DownTime)
{
	int UpdateTime = DownTime * FPS;
	if(AllKeyState[KEY_INPUT_]>UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
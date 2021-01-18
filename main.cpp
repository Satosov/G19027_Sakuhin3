#pragma warning(disable:4010)
#include "DxLib.h"

#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	500
#define COLOR_BIT		32
#define FPS				60

#define WINDOW_BAR_ON	0
#define GAME_TITLE		"���i���鋐�啺��"

#define MAP_KIND_MAX	3

#define SHOTGUN_MAX		5		//�U�e�͍ő�5�������[�h��
#define ROCKET_MAX		1		//�ő�1��
#define MACHINEGUN_MAX	500		//�@�֏e�͍ő�500�������[�h��
#define ENERGYGUN_MAX	1		//�ő�1��

#define ENEMY_HELI_WEAPON_MAX	3

VOID MY_START(VOID);
VOID MY_START_DRAW(VOID);
VOID MY_PLAY(VOID);
VOID MY_PLAY_DRAW(VOID);
VOID MY_END(VOID);
VOID MY_END_DRAW(VOID);

VOID MY_ALL_KEYDOWN_UPDATE(VOID);
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

typedef struct MY_IMAGE
{
	int Handle;		//�摜�̊Ǘ��ԍ�
	int x;
	int y;
	int width;		//�摜�̕�
	int height;		//�摜�̍���
	double radian;	//����̊p�x
	BOOL IsView;
}IMAGE;


typedef struct WEAPON_INFO
{
	IMAGE image;

	BOOL SHOT;
	int reload;
	int reload_MAX;
};

typedef struct CHARA_INFO		//�L�����N�^�[�̃f�[�^
{
	int hp;
	int speed;
};

MY_IMAGE image_background;
MY_IMAGE image_player;			//���@
MY_IMAGE image_player_weapon;	//���c�ł��镐��
MY_IMAGE image_player_tama;		//�v���C���[�̒e
MY_IMAGE image_enemy;			//�G�@
MY_IMAGE image_enemy_tama;		//�G�̒e

CHARA_INFO player;		//�v���C���[
CHARA_INFO enemy_heli;	//�w���R�v�^�[�G
CHARA_INFO enemy_jet;	//�ߋ����^�G
CHARA_INFO enemy_drone;	//�e�N�j�b�N�^�G

int distance;		//���킩��e�̋���


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, COLOR_BIT);

	SetMainWindowText(TEXT(GAME_TITLE));
	SetAlwaysRunFlag(TRUE);
	SetWindowStyleMode(WINDOW_BAR_ON);

	if (DxLib_Init() == 1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);

	gamescene = 1;

	image_player_weapon.x = 50;
	image_player_weapon.y = 100;


	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }
		if (ClearDrawScreen() != 0) { break; }

		MY_ALL_KEYDOWN_UPDATE();
		ClearDrawScreen();

		switch (gamescene)
		{
		case SCENESTART:
			MY_START();
			MY_START_DRAW();
			break;
		case SCENEPLAYING:
			MY_PLAY();
			MY_PLAY_DRAW();
			break;
		case SCENEEND:
			MY_END();
			MY_END_DRAW();
			break;
		}
		WaitTimer(1000 / FPS);
		ScreenFlip();

	}

	DxLib_End();

	return 0;
}

VOID MY_ALL_KEYDOWN_UPDATE(VOID)
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
		else
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
	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

VOID MY_START(VOID)
{

	if (AllKeyState[KEY_INPUT_RETURN] == 1)
	{
		gamescene = SCENEPLAYING;
	}
	return;
}

VOID MY_START_DRAW(VOID)
{
	DrawString(0, 0, "start\npush :ENTER", RGB(255, 255, 255));

	return;
}

VOID MY_PLAY(VOID)
{
	image_player.x = 100;
	image_player.y = 150;

	if (AllKeyState[KEY_INPUT_SPACE] == 1)
	{
		gamescene = SCENEEND;
	}
	if (AllKeyState[KEY_INPUT_1] == 1)
	{
		if (image_player_tama.IsView == FALSE)
		{
			image_player_tama.IsView == TRUE;
		}
	}

	return;
}

VOID MY_PLAY_DRAW(VOID)
{
	DrawString(0, 0, "play\npush :SPACE", RGB(255, 255, 255));


	DrawBox(image_player.x, image_player.y, 200, 499, RGB(255, 0, 0), TRUE);	//���@�̓����蔻��

	if (image_player_tama.IsView == TRUE)
	{
		//DrawCircle()
	}

	return;
}

VOID MY_END(VOID)
{
	if (AllKeyState[KEY_INPUT_ESCAPE] == 1)
	{
		gamescene = SCENESTART;
	}

	return;
}

VOID MY_END_DRAW(VOID)
{
	DrawString(0, 0, "end\npush :ESCAPE", RGB(255, 255, 255));

	return;
}
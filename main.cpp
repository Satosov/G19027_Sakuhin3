#pragma warning(disable:4010)
#include "DxLib.h"

#define WINDOW_WIDTH	1000
#define WINDOW_HEIGHT	500
#define COLOR_BIT		32
#define FPS				60

#define WINDOW_BAR_ON	0
#define GAME_TITLE		"���i���鋐�啺��"

//#define MAP_KIND_MAX	3

#define SG_MAX		5		//�U�e�͍ő�5�� �v�����[�h
#define MG_MAX	4		//�@�֏e�͍ő�500�� �v�����[�h
//#define ROCKET_MAX		1		//�ő�1��
//#define EG_MAX	1		//�ő�1��
//#define TAMA_MAX	255

#define ENEMY_MAX	100

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
	int x2;			//�l�p�`
	int y2;
	int width;		//�摜�̕�
	int height;		//�摜�̍���
	double radian;	//����̊p�x
	BOOL IsView;
}IMAGE;

typedef struct STRUCT_TAMA
{
	char path[SG_MAX];

	int x;
	int y;
	BOOL IsView;
	double radian;		//�e�̊p�x
}TAMA;

typedef struct CHARA_INFO		//�L�����N�^�[�̃f�[�^
{
	IMAGE image;
	int hp;

	BOOL SGCanshot;			//SG���łĂ邪�ǂ���
	BOOL MGCanshot;
	int ShotReloadCnt;
	int ShotReloadCntMAX;

	TAMA sgtama[SG_MAX];
	TAMA mgtama[MG_MAX];
}CHARA;

CHARA player;		//�v���C���[
CHARA enemy[ENEMY_MAX];		//�G

//MY_IMAGE image_background;
//MY_IMAGE image_player;			//���@
//MY_IMAGE image_player_weapon;	//���c�ł��镐��
//MY_IMAGE image_player_tama;		//�v���C���[�̒e
//MY_IMAGE image_enemy;			//�G�@
//MY_IMAGE image_enemy_tama;		//�G�̒e
//
//CHARA_INFO enemy_heli;	//�w���R�v�^�[�G
//CHARA_INFO enemy_jet;	//�ߋ����^�G
//CHARA_INFO enemy_drone;	//�e�N�j�b�N�^�G

//int distance;		//���킩��e�̋���


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

	//image_player_weapon.x = 160;
	//image_player_weapon.y = 160;
	player.ShotReloadCnt = 0;
	player.ShotReloadCntMAX = 60;	//�����[�h


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

	//�e�̏�����
	for (int cnt = 0; cnt < SG_MAX; cnt++)
	{
		player.sgtama[cnt].IsView = FALSE;
	}
	for (int cnt = 0; cnt < MG_MAX; cnt++)
	{
		player.mgtama[cnt].IsView = FALSE;
	}

	for (int cnt = 0; cnt < ENEMY_MAX; cnt++)
	{
		enemy[cnt].image.IsView = FALSE;
		//enemy[cnt].hp = 0;			//hp�̏�����
	}

	return;
}

VOID MY_START_DRAW(VOID)
{
	DrawString(0, 0, "start\npush :ENTER", RGB(255, 255, 255));

	//DrawGraph()		//�w�i�̕`��\��

	return;
}

VOID MY_PLAY(VOID)
{
	for (int cnt = 0; cnt < ENEMY_MAX; cnt++)		//�v���C���n�܂�Ɠ����ɕ`��J�n
	{
		enemy[cnt].image.IsView = TRUE;
		break;
	}
	player.image.x = 100;		//��
	player.image.y = 150;
	player.image.x2 = 200;
	player.image.y2 = 499;

	enemy[0].image.x = 495;
	enemy[0].image.x2 = 505;
	enemy[0].image.y = 165;
	enemy[0].image.y2 = 175;

	if (AllKeyState[KEY_INPUT_SPACE] == TRUE)
	{
		gamescene = SCENEEND;
	}

	if (AllKeyState[KEY_INPUT_Z] == 1)			//Z���������Ƃ�SG
	{
		if (player.SGCanshot == TRUE)		//���Ă�Ƃ�
		{
			player.SGCanshot = FALSE;
			
			for (int cnt = 0; cnt < SG_MAX; cnt++)
			{
				if (player.sgtama[cnt].IsView == FALSE)
				{
					player.sgtama[cnt].x = 170;		//�����W
					player.sgtama[cnt].y = 170;
					player.sgtama[cnt].IsView = TRUE;
					//break;
				}
			}
		}
	}

	//�e���łĂȂ��Ƃ�	SG
	if (player.SGCanshot == FALSE)
	{
		if (player.ShotReloadCnt == player.ShotReloadCntMAX)
		{
			player.ShotReloadCnt = 0;
			player.SGCanshot = TRUE;		//�����[�h����
		}

		player.ShotReloadCnt++;
	}

	if (AllKeyState[KEY_INPUT_X] == TRUE)		//�}�V���K���̒e�̐���
	{
		if (player.MGCanshot == TRUE)			//���Ă�Ƃ�
		{
			for (int cnt = 0; cnt < MG_MAX; cnt++)
			{
				player.mgtama[cnt].x = 170;		//�����W
				player.mgtama[cnt].y = 210;
				player.mgtama[cnt].IsView = TRUE;
				break;
			}
		}
	}

	//�e���łĂȂ��Ƃ��@MG
	if (player.MGCanshot == FALSE)
	{
		if (player.ShotReloadCnt == player.ShotReloadCntMAX)
		{
			player.ShotReloadCnt = 0;
			player.MGCanshot = TRUE;
		}

		player.ShotReloadCnt++;
	}

	return;
}

VOID MY_PLAY_DRAW(VOID)
{
	DrawString(0, 0, "play\npush :SPACE", RGB(255, 255, 255));

	DrawBox(player.image.x, player.image.y, player.image.x2,player.image.y2, RGB(255, 0, 0), TRUE);	//���@�̓����蔻��

	//**************************�V���b�g�K��********************
	for (int cnt = 0; cnt < SG_MAX; cnt++)
	{
		if (player.sgtama[cnt].IsView == TRUE)
		{
			DrawCircle(
				player.sgtama[cnt].x,
				player.sgtama[cnt].y,
				5, RGB(0, 255, 255), TRUE);
		}
		if (player.sgtama[2].x > WINDOW_WIDTH
			|| player.sgtama[2].y > WINDOW_HEIGHT
			|| player.sgtama[2].y < 0)
		{
			player.sgtama[cnt].IsView = FALSE;	//��ʊO�ɂł�ƏI��
		}
		else
		{
			player.sgtama[0].x += 3; player.sgtama[0].y -= 2;
			player.sgtama[1].x += 3; player.sgtama[1].y -= 1;
			player.sgtama[2].x += 3;
			player.sgtama[3].x += 3; player.sgtama[3].y += 1;
			player.sgtama[4].x += 3; player.sgtama[4].y += 2;
		}
	}

	//**************************�}�V���K��********************
	for (int cnt = 0; cnt < MG_MAX; cnt++)
	{
		if (player.mgtama[cnt].IsView == TRUE)//�`��ł���Ƃ�
		{
			DrawCircle(
				player.mgtama[cnt].x,
				player.mgtama[cnt].y,
				4, RGB(255, 255, 0), TRUE);
		}
		if (player.mgtama[cnt].x > WINDOW_WIDTH
			|| player.mgtama[cnt].y > WINDOW_HEIGHT
			|| player.mgtama[cnt].y < 0)		//��ʊO�ɂł�ƕ`��I��
		{
			player.mgtama[cnt].IsView = FALSE;
		}
		else
		{
			player.mgtama[cnt].x += 8;
		}
	}

	//�G�̕`��J�n
	if (enemy[0].image.IsView == TRUE)
	{
		DrawBox(enemy[0].image.x, enemy[0].image.y, enemy[0].image.x2, enemy[0].image.y2, RGB(0, 0, 255), TRUE);	//�G�@��

			//��@image_tama.x < image_husen[i].x + image_husen[i].width&&
			//			image_tama.y<image_husen[i].y + image_husen[i].height &&
			//			image_tama.x + image_tama.width>image_husen[i].x &&
			//			image_tama.y + image_tama.height>image_husen[i].y
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
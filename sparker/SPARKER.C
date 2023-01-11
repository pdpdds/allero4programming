/*
 *      A simple game the use of the Allegro library.
 */


#include <allegro.h>
#include "sparker.h"
#include <stdio.h>

#ifdef _WIN32
#include <synchapi.h>
#include <conio.h>
#endif

int random()
{
	return rand();
}

BITMAP* page1, * page2;

DATAFILE* gamedata;
DATAFILE* stagedata;

void GameEnd();

int game_roop;
int game_roop_switch;
int game_roop_speed = 64;

int title_hotspot[48];


int key_minus = 0;
int key_plus = 0;
int key_misail = 0;
int key_bomb = 0;

int key_start = 0;

typedef struct {
	int life;
	int bomb;
	int start;
	int clear;
	int motion;
	int motion_count;
	int twincle;
	int misail;
	int misail_count;
	int propeller;
	int x, y;
	int cx1, cy1, cx2, cy2;
} SPARKER;

SPARKER                 sparker;

typedef struct {
	int life;
	int x, y;
} SPARKER__MISAIL;

SPARKER__MISAIL         sparker_misail[256];

typedef struct {
	int life;
	int misail;
	int misail_count;
	int x, y;
} SPARKER_MISAIL_SPARKER;

SPARKER_MISAIL_SPARKER  sparker_misail_sparker;

typedef struct {
	int life;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
} SPARKER_BOMB;

SPARKER_BOMB            sparker_bomb;

typedef struct {
	int life;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
} SPARKER_BOOM;

SPARKER_BOOM            sparker_boom;


int stage_number;
int stage_count;
int stage_event_count;
int stage_boss_flag;

int stage_event[4][101][4] = { {{13,640,60,-150},
							 {1,8,32,-24},{2,4,64,-24},{2,4,96,-24},{2,4,128,-24},
							 {1,4,256,-24},{2,4,224,-24},{2,4,192,-24},{2,4,160,-24},
							 {9,64,-32,-32},{0,0,0,0},{9,64,-64,-32},{0,0,0,0},
							 {3,4,96,-24},{4,4,64,-24},{3,4,192,-24},{4,4,224,-24},
							 {3,4,32,-24},{3,4,128,-24},{3,4,160,-24},{3,4,256,-24},
							 {0,0,0,0},{5,32,96,-32},{0,0,0,0},{5,32,192,-32},
							 {6,32,32,-32},{6,32,128,-32},{6,32,160,-32},{6,32,256,-32},
							 {10,48,128,-48},{10,48,256,-48},{2,4,32,-24},{3,4,192,-24},
							 {2,6,32,-24},{2,6,64,-24},{2,6,96,-24},{2,6,160,-24},
							 {2,6,192,-24},{2,6,224,-24},{2,6,256,-24},{2,6,288,-24},
							 {0,0,0,0},{9,64,320,-32},{0,0,0,0},{9,64,332,-32},
							 {5,32,32,-32},{6,32,128,-32},{5,32,160,-32},{6,32,256,-32},
							 {9,64,-32,-32},{0,0,0,0},{4,8,256,-32},{4,8,288,-32},
							 {7,132,64,-64},{0,0,0,0},{4,8,192,-32},{4,8,224,-32},
							 {10,48,128,-48},{4,8,64,-32},{10,48,256,-32},{4,8,224,-32},
							 {2,6,96,-24},{2,6,128,-24},{2,6,192,-24},{6,32,224,-32},
							 {3,6,32,-24},{3,6,96,-24},{3,6,192,-24},{3,6,256,-24},
							 {3,6,64,-24},{3,6,96,-24},{5,32,64,-24},{6,32,224,-32},
							 {8,96,32,-32},{0,0,0,0},{8,96,192,-32},{0,0,0,0},
							 {4,8,96,-24},{4,8,192,-24},{4,8,224,-24},{4,8,64,-24},
							 {2,6,160,-24},{10,48,64,-48},{2,6,160,-24},{10,48,128,-48},
							 {5,32,64,-32},{6,32,96,-32},{5,32,192,-32},{6,32,224,-32},
							 {0,0,0,0},{6,32,64,-32},{0,0,0,0},{6,32,224,-32},
							 {4,8,32,-24},{4,8,128,-24},{4,8,160,-24},{4,8,256,-24},
							 {2,6,64,-24},{0,0,0,0},{2,6,224,-24},{0,0,0,0}},
							{{14,800,-640,-184},
							 {3,6,32,-24},{3,6,64,-24},{3,6,96,-24},{3,6,128,-24},
							 {3,6,256,-24},{3,6,224,-24},{3,6,192,-24},{3,6,160,-24},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {1,6,256,-24},{2,6,224,-24},{2,6,192,-24},{2,6,160,-24},
							 {2,6,32,-24},{2,6,64,-24},{2,6,96,-24},{2,6,128,-24},
							 {12,132,256,-96},{0,0,0,0},{1,6,96,-24},{4,8,96,-24},
							 {12,132,160,-96},{5,32,32,-32},{5,32,128,-32},{5,32,224,-32},
							 {0,0,0,0},{3,6,96,-24},{0,0,0,0},{3,6,256,-24},
							 {7,132,128,-64},{0,0,0,0},{6,32,64,-32},{6,32,192,-32},
							 {0,0,0,0},{11,164,320,-48},{0,0,0,0},{8,96,32,-32},
							 {0,0,0,0},{0,0,0,0},{3,32,96,-24},{4,32,224,-24},
							 {0,0,0,0},{5,32,32,-32},{0,0,0,0},{6,32,192,-32},
							 {0,0,0,0},{2,6,32,-24},{2,6,64,-24},{2,6,96,-24},
							 {12,132,32,-96},{0,0,0,0},{3,6,64,-24},{3,6,96,-24},
							 {4,8,32,-24},{4,8,64,-24},{4,8,96,-24},{4,8,128,-24},
							 {0,0,0,0},{5,32,192,-32},{0,0,0,0},{5,32,224,-32},
							 {0,0,0,0},{3,6,32,-24},{2,6,96,-24},{3,6,160,-24},
							 {0,0,0,0},{6,32,128,-32},{0,0,0,0},{6,32,256,-32},
							 {2,4,32,-24},{2,4,96,-24},{2,4,160,-24},{2,4,224,-24},
							 {0,0,0,0},{11,164,332,-48},{0,0,0,0},{1,6,32,-32},
							 {3,6,128,-24},{3,6,192,-24},{0,0,0,0},{4,8,224,-24},
							 {4,8,224,-24},{0,0,0,0},{4,8,128,-24},{0,0,0,0},
							 {0,0,0,0},{5,32,32,-32},{5,32,96,-32},{0,0,0,0},
							 {2,6,160,-24},{2,6,192,-24},{2,4,288,-24},{2,4,256,-24},
							 {0,0,0,0},{4,8,32,-24},{4,8,224,-24},{8,96,128,-32}},
							{{15,1024,96,-232},
							 {5,48,32,-32},{5,48,256,-32},{6,48,192,-48},{6,48,96,-48},
							 {0,0,0,0},{4,8,64,-24},{0,0,0,0},{4,8,224,-24},
							 {8,164,96,-64},{3,8,256,-24},{4,10,160,-24},{4,10,224,-24},
							 {0,0,0,0},{1,8,32,-24},{2,10,64,-24},{2,10,96,-24},
							 {0,0,0,0},{1,8,128,-24},{2,10,160,-24},{2,10,192,-24},
							 {4,10,32,-24},{4,10,96,-24},{4,10,288,-24},{4,10,224,-24},
							 {9,64,-32,-24},{3,10,224,-24},{3,10,160,-24},{3,10,256,-24},
							 {9,64,320,-24},{4,10,256,-24},{4,10,160,-24},{4,10,224,-24},
							 {0,0,0,0},{10,48,64,-32},{0,0,0,0},{10,48,160,-32},
							 {5,48,32,-32},{2,10,64,-24},{6,48,256,-32},{2,10,224,-24},
							 {0,0,0,0},{7,96,64,-64},{0,0,0,0},{7,96,160,-96},
							 {2,10,32,-24},{2,10,288,-24},{2,10,128,-24},{2,10,256,-24},
							 {6,48,128,-32},{0,0,0,0},{6,48,224,-32},{0,0,0,0},
							 {10,48,64,-32},{10,48,96,-48},{4,10,224,-24},{4,10,256,-24},
							 {0,0,0,0},{6,48,32,-32},{5,48,224,-32},{0,0,0,0},
							 {8,96,32,-32},{3,10,96,-24},{2,10,160,-32},{2,10,192,-24},
							 {0,0,0,0},{9,96,32,-24},{0,0,0,0},{9,96,224,-24},
							 {3,10,64,-32},{3,10,96,-24},{3,10,224,-32},{3,10,192,-24},
							 {0,0,0,0},{5,48,32,-32},{8,96,160,-64},{6,48,256,-32},
							 {0,0,0,0},{4,10,32,-24},{4,10,96,-24},{0,0,0,0},
							 {5,48,160,-32},{0,0,0,0},{3,10,128,-24},{8,96,224,-32},
							 {0,0,0,0},{1,10,32,-24},{3,10,64,-56},{3,10,128,-24},
							 {2,10,96,-24},{2,10,160,-88},{2,10,192,-56},{2,10,224,-24},
							 {6,48,32,-32},{5,48,128,-32},{6,48,192,-32},{5,48,256,-32},
							 {3,10,64,-24},{4,10,96,-88},{4,10,128,-56},{4,10,160,-24}},
							{{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},
							 {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}} };


typedef struct {
	int life;
	int kind;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
} TARGET;

TARGET                  target[64];

typedef struct {
	int life;
	int kind;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
	int xp, yp;
} TARGET_MISAIL;

TARGET_MISAIL           target_misail[64];

typedef struct {
	int life;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
} TARGET_SPARK;

TARGET_SPARK            target_spark[64];

typedef struct {
	int life;
	int kind;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
	int xp, yp;
} TARGET_PIECE;

TARGET_PIECE            target_piece[64];

typedef struct {
	int life;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
	int sx, sy;
	int item;
} TARGET_BOOM;

TARGET_BOOM             target_boom[64];

typedef struct {
	int life;
	int kind;
	int sprite_number;
	int motion;
	int motion_count;
	int x, y;
	int xp, yp;
} TARGET_ITEM;

TARGET_ITEM             target_item[64];


int move_xy_plus[17][8][2] = { {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}},
							  {{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}} };

int cross_xy_target[17][2][4] = { {{0,0,0,0},{0,0,0,0}},
								 {{2,12,21,15},{0,0,0,0}},
								 {{2,12,21,15},{0,0,0,0}},
								 {{4,12,19,15},{0,0,0,0}},
								 {{6,12,17,15},{0,0,0,0}},
								 {{4,14,28,16},{0,0,0,0}},
								 {{4,14,28,16},{0,0,0,0}},
								 {{6,20,59,27},{0,0,0,0}},
								 {{11,21,54,26},{0,0,0,0}},
								 {{8,4,25,15},{0,0,0,0}},
								 {{2,8,20,22},{0,0,0,0}},
								 {{52,8,193,26},{0,0,0,0}},
								 {{3,16,21,62},{0,0,0,0}},
								 {{48,60,153,80},{48,128,153,143}},
								 {{46,25,302,38},{328,18,564,42}},
								 {{16,40,109,67},{0,0,0,0}},
								 {{0,0,0,0},{0,0,0,0}} };

int cross_xy_target_misail[3][4] = { {0,0,0,0},
									{0,0,4,4},{2,2,6,6} };

int cross_xy_item[3][4] = { {0,0,0,0},
						   {2,6,22,10},{2,4,21,12} };


void boom_sparker();

void boom_target(int number);

void make_target_misail(int x, int y, int xp, int yp, int kind);
void make_target_spark(int x, int y);
void make_target_piece(int x, int y, int xp, int yp, int kind);
void make_target_boom(int x, int y, int sx, int sy, int item);
void make_target_item(int x, int y, int kind);

int check_cross(int xa1, int xb1, int ya1, int yb1, int xa2, int xb2, int ya2, int yb2);


void control_game_roop()
{
	game_roop_switch = 1;
}

END_OF_FUNCTION(control_game_roop);


void draw_title_fire()
{
	int c, c2;
	int x, y;
	unsigned long address;
	unsigned char temp[320];

	for (c = 0; c < SCREEN_W; c++)
		temp[c] = 0;

	for (c = 0; c < 48; c++) {
		for (c2 = title_hotspot[c] - 20; c2 < title_hotspot[c] + 20; c2++)
			if ((c2 >= 0) && (c2 < SCREEN_W))
				temp[c2] = MIN(temp[c2] + 20 - ABS(title_hotspot[c] - c2), 192);

		title_hotspot[c] += (random() & 7) - 3;
		if (title_hotspot[c] < 0)
			title_hotspot[c] += SCREEN_W;
		else if (title_hotspot[c] >= SCREEN_W)
			title_hotspot[c] -= SCREEN_W;
	}

	for (c = 0; c < SCREEN_W; c++)
		putpixel(page2, c, SCREEN_H - 1, temp[c]);

	for (y = 0; y < SCREEN_H - 1; y++) {
		address = bmp_read_line(page2, y + 1);

#if _WIN32		
		memcpy(temp, (unsigned char*)address , SCREEN_W);
#else
		movedata(page2->seg, address, _my_ds(), (unsigned)temp, SCREEN_W);
#endif

		for (x = 0; x < SCREEN_W; x++)
			if (temp[x] > 0)
				temp[x]--;


 		address = bmp_write_line(page2, y);
#if _WIN32		
		memcpy((void*)address, (void*)temp, SCREEN_W);
#else
		movedata(_my_ds(), (unsigned)temp, page2->seg, address, SCREEN_W);
#endif
	}
}

void draw_title()
{
	int i;

	clear(screen);

	set_palette(gamedata[SPARKER_TITLE_PALETTE].dat);

	for (i = 1; i <= SCREEN_H; i++) {
		draw_title_fire();

		if (i <= 64) {
			blit(gamedata[SPARKER_TITLE_1].dat, screen, 0, 0, 0, 96 - i, 320, i);
			if (i <= 16)
				blit(gamedata[SPARKER_UNDER_TITLE_1].dat, screen, 0, 0, 0, 112 - i, 320, i);
		}
		else {
			if (i % 16 < 8)
				draw_sprite(screen, gamedata[PLEASE_WAIT].dat, 102, 160);
			else
				rectfill(screen, 102, 160, 217, 169, 0);
		}

#ifdef _WIN32
		Sleep(10);
#else
		delay(10);
#endif
	}

	clear_to_color(screen, 255);

	i = 0;

	while (!key[KEY_ENTER]) {
		if (key[KEY_ESC]) {
			allegro_exit();
			printf("Thanks for playing! -ProYBY\n\n");
			exit(1);
		}

		draw_title_fire();

		blit(page2, page1, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		draw_sprite(page1, gamedata[SPARKER_TITLE_2].dat, 0, 32);
		draw_sprite(page1, gamedata[SPARKER_UNDER_TITLE_2].dat, 0, 96);

		if (i > 16)
			draw_sprite(page1, gamedata[PRESS_START].dat, 102, 160);

		blit(page1, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

		i++;

		if (i >= 32)
			i = 0;

#ifdef _WIN32
		Sleep(10);
#else
		delay(10);
#endif		
	}

	stage_number = 0;

	sparker.life = 2;
	sparker.bomb = 2;
	sparker.misail = 2;
	sparker.cx1 = 12;
	sparker.cy1 = 6;
	sparker.cx2 = 20;
	sparker.cy2 = 26;
}

void openning()
{
	allegro_init();
	install_keyboard();
	install_timer();

	if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "A") != 0) {
		printf("\nError initialising sound\n%s\n\n", allegro_error);
		exit(1);
	}

	fade_out(4);

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else
	/* set a graphics mode sized 320x200 */
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return;
		}
	}


	page1 = create_bitmap(SCREEN_W, SCREEN_H);
	page2 = create_bitmap(SCREEN_W, SCREEN_H);

	clear(page1);
	clear(page2);

	gamedata = load_datafile("SPARKER.DAT");
	if (!gamedata) {
		allegro_exit();
		printf("Error loading SPARKER.DAT\n\n");
		exit(1);
	}

	//if ((midi_driver == &midi_awe32) || (midi_driver == &midi_digmid))
		//play_midi(gamedata[ALLEGRO_MUSIC].dat, FALSE);
	//else
		play_sample(gamedata[ALLEGRO_SPL].dat, 255, 128, 1000, FALSE);

	play_memory_fli(gamedata[ALLEGRO_LOGO].dat, screen, FALSE, NULL);

	rest(1000);
	fade_out(1);

	LOCK_VARIABLE(game_roop_switch);
	LOCK_FUNCTION(control_game_roop);

	install_int_ex(control_game_roop, MSEC_TO_TIMER(game_roop_speed));

	draw_title();
}

void load_stagefile()
{
	char stagefile[20];

	if (stage_number == 0) {
		draw_sprite(screen, gamedata[INTO_THE_JUNGLE].dat, 102, 80);
		strcpy(stagefile, "STAGE_A.DAT");
	}
	if (stage_number == 1) {
		draw_sprite(screen, gamedata[ON_THE_SEA].dat, 102, 80);
		strcpy(stagefile, "STAGE_B.DAT");
	}
	if (stage_number == 2) {
		draw_sprite(screen, gamedata[OUT_OF_THE_BLUE].dat, 102, 88);
		strcpy(stagefile, "STAGE_C.DAT");
	}
	if (stage_number == 3) {
		draw_sprite(screen, gamedata[FLYING_SKY].dat, 102, 88);
		strcpy(stagefile, "STAGE_D.DAT");
	}
	stagedata = load_datafile(stagefile);
	if (!stagefile) {
		allegro_exit();
		printf("Error loading %s\n\n", stagefile);
		exit(1);
	}

	play_midi(gamedata[MIDI_STAGE_A + stage_number].dat, TRUE);

	rest(1000);
}

void control_game_roop_speed()
{
	if (key[KEY_MINUS_PAD] && key_minus == 0) {
		game_roop_speed += 1;
		key_minus = 1;
	}
	if (!key[KEY_MINUS_PAD] && key_minus == 1)
		key_minus = 0;
	if (key[KEY_PLUS_PAD] && key_plus == 0) {
		game_roop_speed -= 1;
		key_plus = 1;
	}
	if (!key[KEY_PLUS_PAD] && key_plus == 1)
		key_plus = 0;

	if (game_roop_speed < 1)
		game_roop_speed = 1;
	if (game_roop_speed > 256)
		game_roop_speed = 256;

	LOCK_VARIABLE(game_roop_switch);
	LOCK_FUNCTION(control_game_roop);

	install_int_ex(control_game_roop, MSEC_TO_TIMER(game_roop_speed));
}

void move_everyone()
{
	int i, k;

	if (stage_count > 0) {
		if (game_roop % 4 == 0)
			stage_count -= 1;
		if (game_roop % 32 == 0)
			stage_event_count += 1;
	}
	if (stage_count <= 0 && stage_count > -200) {
		if (stage_number == 0)
			stage_count -= 4;
		if (stage_number == 1)
			stage_count -= 1;
		if (stage_number == 2)
			stage_count -= 1;
		if (stage_number == 3)
			stage_count -= 4;
		stage_event_count = 0;
	}
	if (stage_count <= -200) {
		if (stage_number == 0)
			stage_count -= 4;
		if (stage_number == 1)
			stage_count = stage_count;
		if (stage_number == 2) {
			for (i = 0; i < 64; i++) {
				if (target[i].kind == 15) {
					if (target[i].life > 0) {
						if (target[i].motion_count % 128 >= 64 && target[i].motion_count % 2 == 0)
							stage_count -= 2;
					}
					break;
				}
			}
		}
		if (stage_number == 3)
			stage_count -= 8;
		if (stage_count <= -400)
			stage_count = -200;
	}

	if (sparker.start == 0 && sparker.clear == 0 && sparker_bomb.life == 0 && sparker_boom.life == 0) {
		if (key[KEY_LEFT]) {
			sparker.motion_count -= 1;
			sparker.x -= 4;
			if (sparker.x <= -2)
				sparker.x = -2;
		}
		if (key[KEY_RIGHT]) {
			sparker.motion_count += 1;
			sparker.x += 4;
			if (sparker.x >= 289)
				sparker.x = 289;
		}
		if (!key[KEY_LEFT] && !key[KEY_RIGHT]) {
			if (sparker.motion_count < 0)
				sparker.motion_count += 1;
			if (sparker.motion_count > 0)
				sparker.motion_count -= 1;
		}
		if (sparker.motion_count <= -6) {
			sparker.motion = 0;
			sparker.motion_count = -6;
		}
		if (sparker.motion_count <= -4 && sparker.motion_count > -6)
			sparker.motion = 1;
		if (sparker.motion_count > -4 && sparker.motion_count < 4)
			sparker.motion = 2;
		if (sparker.motion_count >= 4 && sparker.motion_count < 6)
			sparker.motion = 3;
		if (sparker.motion_count >= 6) {
			sparker.motion = 4;
			sparker.motion_count = 6;
		}
		if (key[KEY_UP]) {
			sparker.y -= 4;
			if (sparker.y <= -3)
				sparker.y = -3;
		}
		if (key[KEY_DOWN]) {
			sparker.y += 4;
			if (sparker.y >= 172)
				sparker.y = 172;
		}
		if (key[KEY_SPACE]) {
			if (sparker_misail_sparker.life == 0)
				sparker.misail_count += 1;
			if (sparker.misail_count == 32)
				play_sample(gamedata[SAMP_ITEM].dat, 255, 128, 1000, FALSE);
			if (key_misail == 0) {
				for (i = 0; i < 256; i += 8) {
					if (sparker_misail[i].life == 0 && sparker_misail[i + 1].life == 0 &&
						sparker_misail[i + 2].life == 0 && sparker_misail[i + 3].life == 0 &&
						sparker_misail[i + 4].life == 0 && sparker_misail[i + 5].life == 0 &&
						sparker_misail[i + 6].life == 0 && sparker_misail[i + 7].life == 0) {
						for (k = 0; k < sparker.misail; k++)
							sparker_misail[i + k].life = 1;
						sparker_misail[i].x = sparker.x + 12;
						sparker_misail[i].y = sparker.y + 0;
						sparker_misail[i + 1].x = sparker.x + 16;
						sparker_misail[i + 1].y = sparker.y + 0;
						sparker_misail[i + 2].x = sparker.x + 8;
						sparker_misail[i + 2].y = sparker.y + 2;
						sparker_misail[i + 3].x = sparker.x + 20;
						sparker_misail[i + 3].y = sparker.y + 2;
						sparker_misail[i + 4].x = sparker.x + 4;
						sparker_misail[i + 4].y = sparker.y + 4;
						sparker_misail[i + 5].x = sparker.x + 24;
						sparker_misail[i + 5].y = sparker.y + 4;
						sparker_misail[i + 6].x = sparker.x + 0;
						sparker_misail[i + 6].y = sparker.y + 6;
						sparker_misail[i + 7].x = sparker.x + 28;
						sparker_misail[i + 7].y = sparker.y + 6;

						play_sample(gamedata[SAMP_SHOOT_01].dat, 64, 128, 1000, FALSE);

						break;
					}
				}
				key_misail = 1;
			}
		}
		if (!key[KEY_SPACE]) {
			if (sparker.misail_count >= 32) {
				sparker_misail_sparker.life = 1;
				sparker_misail_sparker.misail = sparker.misail;
				sparker_misail_sparker.misail_count = 0;
				sparker_misail_sparker.x = sparker.x;
				sparker_misail_sparker.y = sparker.y - 8;
				sparker.misail_count = 0;

				play_sample(gamedata[SAMP_SHOOT_02].dat, 255, 128, 1000, FALSE);
			}
			else
				sparker.misail_count = 0;
			if (key_misail == 1)
				key_misail = 0;
		}
		if (key[KEY_ALT] && sparker.bomb > 0 && key_bomb == 0) {
			sparker.bomb -= 1;
			sparker.motion = 2;
			sparker.motion_count = 0;

			sparker_bomb.life = 1;
			sparker_bomb.sprite_number = BOOM_A_01;
			sparker_bomb.motion = 0;
			sparker_bomb.motion_count = 0;
			sparker_bomb.x = sparker.x - 144;
			sparker_bomb.y = sparker.y - 144;

			key_bomb = 1;
		}
		if (!key[KEY_ALT] && sparker.bomb > 0 && key_bomb == 1)
			key_bomb = 0;
	}
	if (sparker.twincle > 0) {
		sparker.twincle += 1;
		if (sparker.twincle > 96)
			sparker.twincle = 0;
	}
	sparker.propeller += 1;
	if (sparker.propeller > 2)
		sparker.propeller = 0;
	if (sparker.start == 1 && sparker_boom.life == 0) {
		sparker.y -= 4;
		if (sparker.y <= 152)
			sparker.start = 0;
	}
	if (sparker.clear == 1 && sparker.start == 0)
		sparker.y -= 4;

	for (i = 0; i < 256; i++) {
		if (sparker_misail[i].life == 1) {
			sparker_misail[i].y -= 12;
			if (sparker_misail[i].y < -32)
				sparker_misail[i].life = 0;
		}
	}
	if (sparker_misail_sparker.life == 1) {
		if (sparker_misail_sparker.misail_count % 2) {
			for (i = 0; i < 256; i += 8) {
				if (sparker_misail[i].life == 0 && sparker_misail[i + 1].life == 0 &&
					sparker_misail[i + 2].life == 0 && sparker_misail[i + 3].life == 0 &&
					sparker_misail[i + 4].life == 0 && sparker_misail[i + 5].life == 0 &&
					sparker_misail[i + 6].life == 0 && sparker_misail[i + 7].life == 0) {
					for (k = 0; k < sparker_misail_sparker.misail; k++)
						sparker_misail[i + k].life = 1;
					sparker_misail[i].x = sparker_misail_sparker.x + 12;
					sparker_misail[i].y = sparker_misail_sparker.y + 0;
					sparker_misail[i + 1].x = sparker_misail_sparker.x + 16;
					sparker_misail[i + 1].y = sparker_misail_sparker.y + 0;
					sparker_misail[i + 2].x = sparker_misail_sparker.x + 8;
					sparker_misail[i + 2].y = sparker_misail_sparker.y + 2;
					sparker_misail[i + 3].x = sparker_misail_sparker.x + 20;
					sparker_misail[i + 3].y = sparker_misail_sparker.y + 2;
					sparker_misail[i + 4].x = sparker_misail_sparker.x + 4;
					sparker_misail[i + 4].y = sparker_misail_sparker.y + 4;
					sparker_misail[i + 5].x = sparker_misail_sparker.x + 24;
					sparker_misail[i + 5].y = sparker_misail_sparker.y + 4;
					sparker_misail[i + 6].x = sparker_misail_sparker.x + 0;
					sparker_misail[i + 6].y = sparker_misail_sparker.y + 6;
					sparker_misail[i + 7].x = sparker_misail_sparker.x + 28;
					sparker_misail[i + 7].y = sparker_misail_sparker.y + 6;

					play_sample(gamedata[SAMP_SHOOT_01].dat, 64, 128, 1000, FALSE);

					break;
				}
			}
		}
		sparker_misail_sparker.misail_count += 1;
		sparker_misail_sparker.y -= 1;
		if (sparker_misail_sparker.y <= -32)
			sparker_misail_sparker.life = 0;
	}
	if (sparker_bomb.life == 1) {
		if ((sparker_bomb.motion_count >= 0 && sparker_bomb.motion_count < 2) ||
			(sparker_bomb.motion_count >= 16 && sparker_bomb.motion_count < 18))
			sparker.motion = 5;
		if ((sparker_bomb.motion_count >= 2 && sparker_bomb.motion_count < 4) ||
			(sparker_bomb.motion_count >= 14 && sparker_bomb.motion_count < 16))
			sparker.motion = 6;
		if ((sparker_bomb.motion_count >= 4 && sparker_bomb.motion_count < 6) ||
			(sparker_bomb.motion_count >= 12 && sparker_bomb.motion_count < 14))
			sparker.motion = 7;
		if ((sparker_bomb.motion_count >= 6 && sparker_bomb.motion_count < 8) ||
			(sparker_bomb.motion_count >= 10 && sparker_bomb.motion_count < 12))
			sparker.motion = 8;
		if (sparker_bomb.motion_count >= 8 && sparker_bomb.motion_count < 10)
			sparker.motion = 9;
		if (sparker_bomb.motion_count == 2 || sparker_bomb.motion_count == 6 ||
			sparker_bomb.motion_count == 10 || sparker_bomb.motion_count == 14)
			sparker_bomb.motion += 1;
		if (sparker_bomb.motion_count == 2)
			play_sample(gamedata[SAMP_BOOM_03].dat, 255, 128, 1000, FALSE);
		sparker_bomb.motion_count += 1;
		if (sparker_bomb.motion_count >= 18)
			sparker_bomb.life = 0;
	}
	if (sparker_boom.life == 1) {
		if (sparker_boom.motion_count % 2 && sparker_boom.motion_count < 8)
			sparker_boom.motion += 1;
		sparker_boom.motion_count += 1;
		if (sparker_boom.motion_count > 24)
			sparker_boom.life = 0;
	}

	if (stage_event[stage_number][stage_event_count][0] > 0 && stage_boss_flag == 0 && game_roop % 32 == 0) {
		for (i = 0; i < 64; i++) {
			if (target[i].life == 0) {
				target[i].life = stage_event[stage_number][stage_event_count][1];
				target[i].kind = stage_event[stage_number][stage_event_count][0];
				if (target[i].kind == 1)
					target[i].sprite_number = TARGET_PLANE_A;
				if (target[i].kind == 2)
					target[i].sprite_number = TARGET_PLANE_B;
				if (target[i].kind == 3)
					target[i].sprite_number = TARGET_PLANE_C_01;
				if (target[i].kind == 4)
					target[i].sprite_number = TARGET_PLANE_D_01;
				if (target[i].kind == 5)
					target[i].sprite_number = TARGET_PLANE_E_01;
				if (target[i].kind == 6)
					target[i].sprite_number = TARGET_PLANE_F_01;
				if (target[i].kind == 7)
					target[i].sprite_number = TARGET_PLANE_G;
				if (target[i].kind == 8)
					target[i].sprite_number = TARGET_PLANE_H;
				if (target[i].kind == 9)
					target[i].sprite_number = TARGET_TANK_A_01;
				if (target[i].kind == 10)
					target[i].sprite_number = TARGET_TANK_B_01;
				if (target[i].kind == 11)
					target[i].sprite_number = TARGET_SHIP_A_01;
				if (target[i].kind == 12)
					target[i].sprite_number = TARGET_SHIP_B_01;
				if (target[i].kind == 13 || target[i].kind == 14 ||
					target[i].kind == 15 || target[i].kind == 16) {
					target[i].sprite_number = BOSS_01;
					stage_boss_flag = 1;
					play_midi(gamedata[MIDI_BOSS].dat, TRUE);
				}
				target[i].motion = 0;
				target[i].motion_count = 0;
				target[i].x = stage_event[stage_number][stage_event_count][2];
				target[i].y = stage_event[stage_number][stage_event_count][3];

				break;
			}
		}
	}

	for (i = 0; i < 64; i++) {
		if (target[i].life > 0) {
			if (target[i].kind == 1) {
				target[i].y += 1;
			}
			if (target[i].kind == 2) {
				if (target[i].y % 32 == 0)
					make_target_misail(target[i].x + 10, target[i].y + 18, 4, 4, 1);
				if (target[i].y % 32 == 12)
					make_target_misail(target[i].x + 10, target[i].y + 18, 0, 4, 1);
				if (target[i].y % 32 == 24)
					make_target_misail(target[i].x + 10, target[i].y + 18, -4, 4, 1);
				target[i].y += 1;
			}
			if (target[i].kind == 3) {
				if (target[i].motion_count >= 0 && target[i].motion_count < 64) {
					target[i].motion = 2;
					if (target[i].motion_count % 32 == 16) {
						make_target_misail(target[i].x + 10, target[i].y + 18, -4, 4, 1);
						make_target_misail(target[i].x + 10, target[i].y + 18, 4, 4, 1);
					}
					target[i].y += 1;
				}
				if (target[i].motion_count >= 64 && target[i].motion_count < 68) {
					if (target[i].motion_count == 64)
						make_target_misail(target[i].x + 10, target[i].y + 18, 0, 6, 1);
					if (target[i].x < 144) {
						target[i].motion = 1;
						target[i].x -= 1;
					}
					if (target[i].x >= 144) {
						target[i].motion = 3;
						target[i].x += 1;
					}
					target[i].y += 1;
				}
				if (target[i].motion_count >= 68) {
					if (target[i].x < 144) {
						target[i].motion = 0;
						if (target[i].motion_count % 32 == 16)
							make_target_misail(target[i].x + 10, target[i].y + 18, 4, 4, 1);
						target[i].x -= 1;
					}
					if (target[i].x >= 144) {
						target[i].motion = 4;
						if (target[i].motion_count % 32 == 16)
							make_target_misail(target[i].x + 10, target[i].y + 18, -4, 4, 1);
						target[i].x += 1;
					}
					target[i].y += 1;
				}
			}
			if (target[i].kind == 4) {
				if (target[i].motion_count % 32 == 0)
					make_target_misail(target[i].x + 10, target[i].y + 18, -4, 4, 1);
				if (target[i].motion_count % 32 == 8)
					make_target_misail(target[i].x + 10, target[i].y + 18, 4, 4, 1);
				if (target[i].motion_count % 32 == 16)
					make_target_misail(target[i].x + 10, target[i].y + 18, -2, 4, 1);
				if (target[i].motion_count % 32 == 24)
					make_target_misail(target[i].x + 10, target[i].y + 18, 2, 4, 1);
				if (target[i].motion_count % 2 == 0) {
					target[i].motion += 1;
					if (target[i].motion > 5)
						target[i].motion = 0;
				}
				target[i].y += 2;
			}
			if (target[i].kind == 5) {
				if (target[i].motion_count >= 0 && target[i].motion_count < 96) {
					if (target[i].motion_count == 32 || target[i].motion_count == 64) {
						make_target_misail(target[i].x + 14, target[i].y + 26, -4, 4, 1);
						make_target_misail(target[i].x + 14, target[i].y + 26, 4, 4, 1);
					}
					target[i].y += 1;
				}
				if (target[i].motion_count >= 96 && target[i].motion_count < 100)
					target[i].motion = 1;
				if (target[i].motion_count >= 100 && target[i].motion_count < 104)
					target[i].motion = 2;
				if (target[i].motion_count >= 104 && target[i].motion_count < 108)
					target[i].motion = 3;
				if (target[i].motion_count >= 108 && target[i].motion_count < 112)
					target[i].motion = 4;
				if (target[i].motion_count >= 112) {
					target[i].motion = 5;
					if (target[i].motion_count == 112)
						make_target_misail(target[i].x + 14, target[i].y + 14, 0, 6, 1);
					target[i].y -= 2;
				}
			}
			if (target[i].kind == 6) {
				if (target[i].motion_count >= 0 && target[i].motion_count < 96) {
					if (target[i].motion_count == 32 || target[i].motion_count == 64) {
						make_target_misail(target[i].x + 14, target[i].y + 26, -4, 4, 1);
						make_target_misail(target[i].x + 14, target[i].y + 26, 0, 4, 1);
						make_target_misail(target[i].x + 14, target[i].y + 26, 4, 4, 1);
					}
					target[i].y += 1;
				}
				if (target[i].motion_count >= 96 && target[i].motion_count < 100)
					target[i].motion = 1;
				if (target[i].motion_count >= 100 && target[i].motion_count < 104)
					target[i].motion = 2;
				if (target[i].motion_count >= 104 && target[i].motion_count < 108)
					target[i].motion = 3;
				if (target[i].motion_count >= 108 && target[i].motion_count < 112)
					target[i].motion = 4;
				if (target[i].motion_count >= 112) {
					target[i].motion = 5;
					if (target[i].motion_count == 112) {
						make_target_misail(target[i].x + 14, target[i].y + 14, -2, 6, 1);
						make_target_misail(target[i].x + 14, target[i].y + 14, 2, 6, 1);
					}
					target[i].y -= 2;
				}
			}
			if (target[i].kind == 7) {
				if (target[i].motion_count >= 0 && target[i].motion_count < 48)
					target[i].y += 2;
				if (target[i].motion_count >= 48 && target[i].motion_count < 320) {
					if (target[i].motion_count % 48 == 8) {
						make_target_misail(target[i].x + 30, target[i].y + 58, -2, 2, 1);
						make_target_misail(target[i].x + 30, target[i].y + 58, 0, 2, 1);
						make_target_misail(target[i].x + 30, target[i].y + 58, 2, 2, 1);
					}
					if (target[i].motion_count % 48 == 32) {
						make_target_misail(target[i].x + 30, target[i].y + 58, -4, 4, 1);
						make_target_misail(target[i].x + 30, target[i].y + 58, 4, 4, 1);
					}
				}
				if (target[i].motion_count >= 320) {
					target[i].x -= 1;
					target[i].y += 1;
				}
			}
			if (target[i].kind == 8) {
				if (target[i].motion_count >= 0 && target[i].motion_count < 104)
					target[i].y += 1;
				if (target[i].motion_count >= 104 && target[i].motion_count < 320) {
					if (target[i].motion_count % 32 == 8) {
						make_target_misail(target[i].x + 30, target[i].y + 26, -2, 2, 1);
						make_target_misail(target[i].x + 30, target[i].y + 26, 0, 2, 1);
						make_target_misail(target[i].x + 30, target[i].y + 26, 2, 2, 1);
					}
				}
				if (target[i].motion_count >= 320) {
					target[i].x += 2;
					target[i].y -= 2;
				}
			}
			if (target[i].kind == 9) {
				if (target[i].motion == 0 && target[i].motion_count < 256) {
					if (target[i].x < 144) {
						target[i].x += 2;
						if (target[i].motion_count % 2 == 0)
							target[i].x -= 3;
					}
					if (target[i].x >= 144) {
						target[i].x -= 2;
						if (target[i].motion_count % 2 == 0)
							target[i].x += 3;
					}
				}
				if (target[i].motion == 0) {
					if (target[i].motion_count % 64 == 0)
						make_target_misail(target[i].x + 10, target[i].y + 20, -4, 4, 2);
					if (target[i].motion_count % 64 == 16)
						make_target_misail(target[i].x + 10, target[i].y + 20, 0, 4, 2);
					if (target[i].motion_count % 64 == 32)
						make_target_misail(target[i].x + 10, target[i].y + 20, 4, 4, 2);
					if (target[i].motion_count % 64 == 48)
						make_target_misail(target[i].x + 10, target[i].y + 20, 0, 4, 2);
				}
				if (game_roop % 4 == 0)
					target[i].y += 1;
			}
			if (target[i].kind == 10) {
				if (target[i].motion == 0 && target[i].motion_count < 128) {
					target[i].y += 2;
					if (target[i].motion_count % 2 == 0)
						target[i].y -= 3;
				}
				if (target[i].motion == 0) {
					if (target[i].motion_count % 32 == 0) {
						make_target_misail(target[i].x + 10, target[i].y + 20, -4, 6, 2);
						make_target_misail(target[i].x + 10, target[i].y + 20, 0, 6, 2);
						make_target_misail(target[i].x + 10, target[i].y + 20, 4, 6, 2);
					}
				}
				if (game_roop % 4 == 0)
					target[i].y += 1;
			}
			if (target[i].kind == 11) {
				if (target[i].motion == 0 && target[i].motion_count < 256)
					target[i].x -= 1;
				if (target[i].motion == 0) {
					for (k = 0; k < 3; k++) {
						if (target[i].motion_count % 64 == k * 16) {
							make_target_misail(target[i].x + 66, target[i].y + 15, -4 + (k * 4), 4, 1);
							make_target_misail(target[i].x + 121, target[i].y + 16, -4 + (k * 4), 4, 1);
							make_target_misail(target[i].x + 177, target[i].y + 15, -4 + (k * 4), 4, 1);
						}
					}
					if (target[i].motion_count % 64 == 0) {
						make_target_misail(target[i].x + 66, target[i].y + 15, -4, -6, 2);
						make_target_misail(target[i].x + 121, target[i].y + 16, 0, -6, 2);
						make_target_misail(target[i].x + 177, target[i].y + 15, 4, -6, 2);
					}
				}
				if (game_roop % 4 == 0 && stage_event_count != 0)
					target[i].y += 1;
			}
			if (target[i].kind == 12) {
				if (target[i].motion == 0)
					target[i].y += 1;
				if (target[i].motion == 0) {
					if (target[i].motion_count % 64 == 0) {
						for (k = 0; k < 2; k++) {
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), -4, -4, 2);
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), -4, 0, 2);
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), -4, 4, 2);
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), 4, -4, 2);
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), 4, 0, 2);
							make_target_misail(target[i].x + 10, target[i].y + 20 + (k * 32), 4, 4, 2);
						}
						make_target_misail(target[i].x + 10, target[i].y + 20, 0, -4, 2);
						make_target_misail(target[i].x + 10, target[i].y + 52, 0, 4, 2);
					}
				}
				if (target[i].motion == 1 && game_roop % 4 == 0)
					target[i].y += 1;
			}
			if (target[i].kind == 13) {
				if (target[i].motion_count < 60)
					target[i].y += 2;
				if (target[i].motion_count < 268 && target[i].motion == 1)
					target[i].y += 4;

				if (target[i].motion_count % 32 >= 0 && target[i].motion_count % 32 < 16)
					target[i].y += 1;
				if (target[i].motion_count % 32 >= 16 && target[i].motion_count % 32 < 32)
					target[i].y -= 1;

				if (((target[i].motion_count % 256 >= 0 && target[i].motion_count % 256 < 64) ||
					(target[i].motion_count % 256 >= 192 && target[i].motion_count % 256 < 256)) && target[i].motion == 1)
					target[i].x -= 2;
				if (((target[i].motion_count % 256 >= 64 && target[i].motion_count % 256 < 128) ||
					(target[i].motion_count % 256 >= 128 && target[i].motion_count % 256 < 192)) && target[i].motion == 1)
					target[i].x += 2;

				for (k = 0; k < 14; k++) {
					if (target[i].motion_count % 96 == k * 4 && target[i].y >= -32) {
						make_target_misail(target[i].x + 73, target[i].y + 70, -k + 7, 4, (k % 2) + 1);
						make_target_misail(target[i].x + 116, target[i].y + 70, k - 7, 4, (k % 2) + 1);
					}
				}
				if (target[i].motion_count % 192 == 64 && target[i].y >= -32) {
					make_target_misail(target[i].x + 74, target[i].y + 70, 0, 8, 2);
					make_target_misail(target[i].x + 116, target[i].y + 70, 0, 8, 2);
				}

				if (target[i].life <= 256 && target[i].motion == 0) {
					target[i].motion = 1;
					target[i].motion_count = 256;

					for (k = 0; k < 3; k++) {
						make_target_piece(target[i].x + 32 + (k * 70), target[i].y + 128, -4 + (k * 4), -4, 1);
						make_target_piece(target[i].x + 32 + (k * 70), target[i].y + 144, -4 + (k * 4), 4, 1);
						if (k == 0 || k == 2)
							make_target_boom(target[i].x + 32 + (k * 70), target[i].y + 128, 88, 16, 0);
					}
					play_sample(gamedata[SAMP_BOOM_03].dat, 255, 128, 1000, FALSE);

					for (k = 0; k < 64; k++)
						target_misail[k].life = 0;
				}
			}
			if (target[i].kind == 14) {
				if (target[i].motion == 0 && target[i].x <= -320)
					target[i].x += 2;
				if (target[i].motion == 1 && target[i].x <= 0)
					target[i].x += 2;

				if (target[i].y < 16)
					target[i].y += 1;

				if (target[i].motion == 0 && target[i].x >= -320) {
					for (k = 0; k < 14; k++) {
						if (target[i].motion_count % 128 == k * 8) {
							make_target_misail(target[i].x + 375, target[i].y + 27, -k + 7, 4, 2);
							make_target_misail(target[i].x + 409, target[i].y + 29, k - 7, 4, 1);
							make_target_misail(target[i].x + 449, target[i].y + 30, 0, 6, 2);
							make_target_misail(target[i].x + 481, target[i].y + 28, -k + 7, 4, 1);
							make_target_misail(target[i].x + 519, target[i].y + 31, k - 7, 4, 2);
						}
					}
				}
				if (target[i].motion == 1 && target[i].x >= 0) {
					for (k = 0; k < 4; k++) {
						if (target[i].motion_count % 16 == k * 4)
							make_target_misail(target[i].x + 99 + (k * 32) + 4, target[i].y + 28, -6 + (k * 2) + (k * 2), 6, 2);
					}
				}

				if (target[i].life <= 400 && target[i].motion == 0) {
					target[i].motion = 1;

					for (k = 0; k < 3; k++) {
						make_target_piece(target[i].x + 328 + (k * 80), target[i].y + 18, -4 + (k * 4), -4, 1);
						make_target_piece(target[i].x + 328 + (k * 80), target[i].y + 34, -4 + (k * 4), 4, 1);
						if (k == 0 || k == 2)
							make_target_boom(target[i].x + 328 + (k * 80), target[i].y + 18, 240, 32, 0);
					}
					play_sample(gamedata[SAMP_BOOM_04].dat, 255, 128, 1000, FALSE);

					for (k = 0; k < 64; k++)
						target_misail[k].life = 0;
				}
			}
			if (target[i].kind == 15) {
				if (target[i].motion == 0 && stage_count < -200) {
					if (target[i].motion_count % 128 >= 64) {
						target[i].y -= 2;
						if (target[i].motion_count % 2)
							target[i].y += 4;
					}
					for (k = 0; k < 14; k++) {
						if (target[i].motion_count % 64 == k * 4) {
							make_target_misail(target[i].x + 38, target[i].y + 48, -k + 7, 4, 1);
							make_target_misail(target[i].x + 80, target[i].y + 48, k - 7, 4, 1);
						}
						if (target[i].motion_count % 128 == k * 8) {
							make_target_misail(target[i].x + 36, target[i].y + 124, k - 7, 6, 2);
							make_target_misail(target[i].x + 82, target[i].y + 124, -k + 7, 6, 2);
						}
					}
				}

				if (target[i].y < -32 && stage_count >= -200)
					target[i].y += 1;
			}

			target[i].motion_count += 1;

			if (stage_event_count == 0 && target[i].kind <= 12) {
				if (stage_number == 0)
					target[i].y += 4;
				if (stage_number == 1)
					target[i].y += 1;
				if (stage_number == 2)
					target[i].y += 1;
				if (stage_number == 3)
					target[i].y += 4;
			}

			if ((target[i].x <= -128 || target[i].x >= 384 || target[i].y <= -128 || target[i].y >= 264) && target[i].kind <= 12)
				target[i].life = 0;
		}
		if (target_misail[i].life == 1) {
			if (target_misail[i].motion_count % 4) {
				target_misail[i].motion += 1;
				if (target_misail[i].motion > 3)
					target_misail[i].motion = 0;
			}
			target_misail[i].motion_count += 1;
			target_misail[i].x += target_misail[i].xp;
			target_misail[i].y += target_misail[i].yp;
			if (stage_event_count == 0) {
				if (stage_number == 0)
					target_misail[i].y += 4;
				if (stage_number == 1)
					target_misail[i].y += 1;
				if (stage_number == 2)
					target_misail[i].y += 1;
				if (stage_number == 3)
					target_misail[i].y += 4;
			}
			if (target_misail[i].x <= -32 || target_misail[i].x >= 320 || target_misail[i].y <= -32 || target_misail[i].y >= 200)
				target_misail[i].life = 0;
		}
		if (target_spark[i].life == 1) {
			if (target_spark[i].motion_count % 2)
				target_spark[i].motion += 1;
			target_spark[i].motion_count += 1;
			if (target_spark[i].motion_count > 4)
				target_spark[i].life = 0;
			if (stage_event_count == 0) {
				if (stage_number == 0)
					target_spark[i].y += 4;
				if (stage_number == 1)
					target_spark[i].y += 1;
				if (stage_number == 2)
					target_spark[i].y += 1;
				if (stage_number == 3)
					target_spark[i].y += 4;
			}
		}
		if (target_piece[i].life == 1) {
			if (target_piece[i].motion_count % 2)
				target_piece[i].motion += 1;
			target_piece[i].motion_count += 1;
			if (target_piece[i].kind == 1) {
				if (target_piece[i].motion > 5)
					target_piece[i].motion = 0;
				if (target_piece[i].motion_count >= 8)
					target_piece[i].life = 0;
			}
			if (target_piece[i].kind == 2) {
				if (target_piece[i].motion > 3)
					target_piece[i].motion = 0;
				if (target_piece[i].motion_count >= 12)
					target_piece[i].life = 0;
			}
			target_piece[i].x += target_piece[i].xp;
			target_piece[i].y += target_piece[i].yp;
			if (stage_event_count == 0) {
				if (stage_number == 0)
					target_piece[i].y += 4;
				if (stage_number == 1)
					target_piece[i].y += 1;
				if (stage_number == 2)
					target_piece[i].y += 1;
				if (stage_number == 3)
					target_piece[i].y += 4;
			}
		}
		if (target_boom[i].life == 1) {
			if (target_boom[i].motion_count % 2)
				target_boom[i].motion += 1;
			target_boom[i].motion_count += 1;
			if (target_boom[i].motion_count == 4 && target_boom[i].item > 0)
				make_target_item(target_boom[i].x + ((target_boom[i].sx / 2) - 16), target_boom[i].y + ((target_boom[i].sy / 2) - 8), target_boom[i].item);
			if (target_boom[i].motion_count > 8)
				target_boom[i].life = 0;
			if (stage_event_count == 0) {
				if (stage_number == 0)
					target_boom[i].y += 4;
				if (stage_number == 1)
					target_boom[i].y += 1;
				if (stage_number == 2)
					target_boom[i].y += 1;
				if (stage_number == 3)
					target_boom[i].y += 4;
			}
		}
		if (target_item[i].life == 1) {
			if (target_item[i].motion_count % 2)
				target_item[i].motion += 1;
			if (target_item[i].motion > 5)
				target_item[i].motion = 0;
			target_item[i].motion_count += 1;
			target_item[i].x += target_item[i].xp;
			target_item[i].y += target_item[i].yp;
			if (target_item[i].x <= -8 || target_item[i].x >= 296)
				target_item[i].xp = -target_item[i].xp;
			if (target_item[i].y <= -8 || target_item[i].y >= 192)
				target_item[i].yp = -target_item[i].yp;
		}
	}
}

void boom_sparker()
{
	int i;

	sparker_boom.life = 1;
	sparker_boom.sprite_number = BOOM_A_01;
	sparker_boom.motion = 0;
	sparker_boom.motion_count = 0;
	sparker_boom.x = sparker.x;
	sparker_boom.y = sparker.y;
	play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);

	sparker.life -= 1;
	for (i = 0; i < sparker.bomb / 2; i++)
		make_target_item(sparker_boom.x, sparker_boom.y + 8, 2);
	sparker.bomb = 2;
	if (sparker.life < 0)
		sparker.bomb = 0;
	sparker.start = 1;
	sparker.motion = 2;
	sparker.motion_count = 0;
	sparker.twincle = 1;
	for (i = 0; i < sparker.misail / 4; i++)
		make_target_item(sparker_boom.x, sparker_boom.y + 8, 1);
	sparker.misail = 2;
	sparker.misail_count = 0;
	sparker.x = 144;
	sparker.y = 200;
}

void boom_target(int number)
{
	int i;

	if (target[number].kind == 1 || target[number].kind == 2 ||
		target[number].kind == 3 || target[number].kind == 4) {
		make_target_piece(target[number].x + 8, target[number].y + 8, 2, 2, 2);
		if (target[number].kind == 1)
			make_target_boom(target[number].x, target[number].y, 24, 24, 1);
		else
			make_target_boom(target[number].x, target[number].y, 24, 24, 0);
		play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 5 || target[number].kind == 6) {
		make_target_piece(target[number].x + 12, target[number].y + 12, -2, -2, 2);
		make_target_piece(target[number].x + 12, target[number].y + 12, 2, 2, 1);
		make_target_boom(target[number].x, target[number].y, 32, 32, 0);
		play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 7) {
		for (i = 0; i < 2; i++) {
			make_target_piece(target[number].x + 28, target[number].y + 28, -2, -2 + (i * 4), 2);
			make_target_piece(target[number].x + 28, target[number].y + 28, 2, -2 + (i * 4), 1);
		}
		make_target_boom(target[number].x, target[number].y, 64, 64, 2);
		play_sample(gamedata[SAMP_BOOM_03].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 8) {
		make_target_piece(target[number].x + 28, target[number].y + 12, -2, 2, 1);
		make_target_piece(target[number].x + 28, target[number].y + 12, 2, 2, 1);
		make_target_piece(target[number].x + 28, target[number].y + 12, 0, -2, 2);
		make_target_boom(target[number].x, target[number].y, 64, 32, 0);
		play_sample(gamedata[SAMP_BOOM_03].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 9) {
		make_target_piece(target[number].x + 12, target[number].y + 6, -2, 2, 1);
		make_target_piece(target[number].x + 12, target[number].y + 6, 2, -2, 2);
		make_target_boom(target[number].x, target[number].y, 32, 18, 0);
		play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 10) {
		make_target_piece(target[number].x + 6, target[number].y + 12, -2, 2, 1);
		make_target_piece(target[number].x + 6, target[number].y + 12, 2, -2, 2);
		make_target_boom(target[number].x, target[number].y, 22, 32, 0);
		play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 11) {
		for (i = 0; i < 2; i++) {
			make_target_piece(target[number].x + 52, target[number].y + 8 + (i * 13), -2, -2 + (i * 2), 1);
			make_target_piece(target[number].x + 112, target[number].y + 8 + (i * 13), 0, -2 + (i * 2), 1);
			make_target_piece(target[number].x + 194, target[number].y + 8 + (i * 13), 2, -2 + (i * 2), 1);
		}
		make_target_boom(target[number].x, target[number].y, 224, 32, 0);
		play_sample(gamedata[SAMP_BOOM_02].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 12) {
		for (i = 0; i < 3; i++) {
			make_target_piece(target[number].x + 4, target[number].y + 16 + (i + 23), -2, -2 + (i * 2), 2);
			make_target_piece(target[number].x + 18, target[number].y + 16 + (i + 23), 2, -2 + (i * 2), 2);
		}
		make_target_boom(target[number].x, target[number].y, 24, 84, 0);
		play_sample(gamedata[SAMP_BOOM_03].dat, 255, 128, 1000, FALSE);
	}
	if (target[number].kind == 13) {
		for (i = 0; i < 3; i++) {
			make_target_piece(target[number].x + (i * 85), target[number].y, -4 + (i * 4), -4, 1);
			make_target_piece(target[number].x + (i * 85), target[number].y + 70, -4 + (i * 4), 0, 1);
			make_target_piece(target[number].x + (i * 85), target[number].y + 140, -4 + (i * 4), 4, 1);
		}
		make_target_boom(target[number].x, target[number].y, 201, 150, 0);
		play_sample(gamedata[SAMP_BOOM_04].dat, 255, 128, 1000, FALSE);

		for (i = 0; i < 64; i++)
			target_misail[i].life = 0;

		sparker.clear = 1;
		sparker.motion = 2;
	}
	if (target[number].kind == 14) {
		for (i = 0; i < 3; i++) {
			make_target_piece(target[number].x + 46 + (i * 124), target[number].y + 25, -4 + (i * 4), -4, 1);
			make_target_piece(target[number].x + 46 + (i * 124), target[number].y + 27, -4 + (i * 4), 0, 1);
			make_target_piece(target[number].x + 46 + (i * 124), target[number].y + 30, -4 + (i * 4), 4, 1);
		}
		make_target_boom(target[number].x + 46, target[number].y + 25, 256, 14, 0);
		play_sample(gamedata[SAMP_BOOM_04].dat, 255, 128, 1000, FALSE);

		for (i = 0; i < 64; i++)
			target_misail[i].life = 0;

		sparker.clear = 1;
		sparker.motion = 2;
	}
	if (target[number].kind == 15) {
		target[number].motion = 1;

		for (i = 0; i < 3; i++) {
			make_target_piece(target[number].x + 16 + (i * 48), target[number].y + 40, -4 + (i * 4), -4, 1);
			make_target_piece(target[number].x + 16 + (i * 48), target[number].y + 48, -4 + (i * 4), 0, 1);
			make_target_piece(target[number].x + 16 + (i * 48), target[number].y + 54, -4 + (i * 4), 4, 1);
		}
		make_target_boom(target[number].x + 16, target[number].y, 96, 128, 0);
		play_sample(gamedata[SAMP_BOOM_04].dat, 255, 128, 1000, FALSE);

		for (i = 0; i < 64; i++)
			target_misail[i].life = 0;

		sparker.clear = 1;
		sparker.motion = 2;
	}
}

void make_target_misail(int x, int y, int xp, int yp, int kind)
{
	int i;

	for (i = 0; i < 64; i++) {
		if (target_misail[i].life == 0) {
			target_misail[i].life = 1;
			target_misail[i].kind = kind;
			if (target_misail[i].kind == 1)
				target_misail[i].sprite_number = TARGET_MISAIL_A_01;
			if (target_misail[i].kind == 2)
				target_misail[i].sprite_number = TARGET_MISAIL_B_01;
			target_misail[i].motion = 0;
			target_misail[i].motion_count = 0;
			target_misail[i].x = x;
			target_misail[i].y = y;
			target_misail[i].xp = xp;
			target_misail[i].yp = yp;
			play_sample(gamedata[SAMP_SHOOT_01].dat, 56, 128, 1000, FALSE);

			break;
		}
	}
}

void make_target_spark(int x, int y)
{
	int i;

	for (i = 0; i < 64; i++) {
		if (target_spark[i].life == 0) {
			target_spark[i].life = 1;
			target_spark[i].sprite_number = SPARK_01;
			target_spark[i].motion = 0;
			target_spark[i].motion_count = 0;
			target_spark[i].x = x;
			target_spark[i].y = y;
			play_sample(gamedata[SAMP_BOOM_01].dat, 72, 128, 1000, FALSE);

			break;
		}
	}
}

void make_target_piece(int x, int y, int xp, int yp, int kind)
{
	int i;

	for (i = 0; i < 64; i++) {
		if (target_piece[i].life == 0) {
			target_piece[i].life = 1;
			target_piece[i].kind = kind;
			if (target_piece[i].kind == 1)
				target_piece[i].sprite_number = PIECE_A_01;
			if (target_piece[i].kind == 2)
				target_piece[i].sprite_number = PIECE_B_01;
			target_piece[i].motion = 0;
			target_piece[i].motion_count = 0;
			target_piece[i].x = x;
			target_piece[i].y = y;
			target_piece[i].xp = xp;
			target_piece[i].yp = yp;

			break;
		}
	}
}

void make_target_boom(int x, int y, int sx, int sy, int item)
{
	int i;

	for (i = 0; i < 64; i++) {
		if (target_boom[i].life == 0) {
			target_boom[i].life = 1;
			target_boom[i].sprite_number = BOOM_A_01;
			target_boom[i].motion = 0;
			target_boom[i].motion_count = 0;
			target_boom[i].x = x;
			target_boom[i].y = y;
			target_boom[i].sx = sx;
			target_boom[i].sy = sy;
			target_boom[i].item = item;

			break;
		}
	}
}

void make_target_item(int x, int y, int kind)
{
	int i;

	for (i = 0; i < 64; i++) {
		if (target_item[i].life == 0) {
			target_item[i].life = 1;
			target_item[i].kind = kind;
			if (target_item[i].kind == 1)
				target_item[i].sprite_number = ITEM_POWER_01;
			if (target_item[i].kind == 2)
				target_item[i].sprite_number = ITEM_BOMB_01;
			target_item[i].motion = 0;
			target_item[i].motion_count = 0;
			target_item[i].x = x;
			target_item[i].y = y;
			target_item[i].xp = (random() & 4) - 2;
			target_item[i].yp = (random() & 4) - 2;

			break;
		}
	}
}

int check_cross(int xa1, int xb1, int ya1, int yb1, int xa2, int xb2, int ya2, int yb2)
{
	if (xa1 < xb2 && xb1 < xa2 && ya1 < yb2 && yb1 < ya2)
		return 1;
	else
		return 0;
}

void check_cross_everyone()
{
	int i, k;

	for (i = 0; i < 64; i++) {
		if (target[i].life > 0) {
			if (check_cross(sparker.x + sparker.cx1, target[i].x + cross_xy_target[target[i].kind][0][0],
				sparker.y + sparker.cy1, target[i].y + cross_xy_target[target[i].kind][0][1],
				sparker.x + sparker.cx2, target[i].x + cross_xy_target[target[i].kind][0][2],
				sparker.y + sparker.cy2, target[i].y + cross_xy_target[target[i].kind][0][3]) && (target[i].kind <= 8 || target[i].kind > 12) &&
				sparker.twincle == 0 && sparker_bomb.life == 0 && sparker_boom.life == 0) {
				target[i].life -= 48;
				if (target[i].life <= 0) {
					target[i].life = 0;
					boom_target(i);
				}

				boom_sparker();
			}
			for (k = 0; k < 256; k++) {
				if (sparker_misail[k].life == 1 && target[i].life > 0) {
					if (check_cross(sparker_misail[k].x, target[i].x + cross_xy_target[target[i].kind][0][0],
						sparker_misail[k].y, target[i].y + cross_xy_target[target[i].kind][0][1],
						sparker_misail[k].x + 4, target[i].x + cross_xy_target[target[i].kind][0][2],
						sparker_misail[k].y + 11, target[i].y + cross_xy_target[target[i].kind][0][3])) {
						if (target[i].kind <= 8 || target[i].kind > 12) {
							sparker_misail[k].life = 0;
							make_target_spark(sparker_misail[k].x, sparker_misail[k].y);

							target[i].life -= 1;
							if (target[i].life <= 0) {
								target[i].life = 0;
								boom_target(i);
							}
						}
						if (target[i].kind > 8 && target[i].kind <= 12) {
							if (target[i].motion == 0) {
								sparker_misail[k].life = 0;
								make_target_spark(sparker_misail[k].x, sparker_misail[k].y);

								target[i].life -= 1;
								if (target[i].life <= 16) {
									target[i].life = 16;
									target[i].motion = 1;
									boom_target(i);
								}
							}
						}
					}
					if (check_cross(sparker_misail[k].x, target[i].x + cross_xy_target[target[i].kind][1][0],
						sparker_misail[k].y, target[i].y + cross_xy_target[target[i].kind][1][1],
						sparker_misail[k].x + 4, target[i].x + cross_xy_target[target[i].kind][1][2],
						sparker_misail[k].y + 11, target[i].y + cross_xy_target[target[i].kind][1][3]) && target[i].kind > 12 && target[i].motion == 0) {
						sparker_misail[k].life = 0;
						make_target_spark(sparker_misail[k].x, sparker_misail[k].y);

						target[i].life -= 1;
					}
				}
			}
		}

		if (sparker_bomb.life == 1) {
			if (target[i].life > 0) {
				if (target[i].kind <= 8 || target[i].kind > 12) {
					target[i].life -= 4;
					if (target[i].life <= 0) {
						target[i].life = 0;
						boom_target(i);
					}
				}
				if (target[i].kind > 8 && target[i].kind <= 12) {
					if (target[i].motion == 0) {
						target[i].life -= 4;
						if (target[i].life <= 16) {
							target[i].life = 16;
							target[i].motion = 1;
							boom_target(i);
						}
					}
				}
			}
			if (target_misail[i].life == 1)
				target_misail[i].life = 0;
		}

		if (target_misail[i].life == 1) {
			if (check_cross(sparker.x + sparker.cx1, target_misail[i].x + cross_xy_target_misail[target_misail[i].kind][0],
				sparker.y + sparker.cy1, target_misail[i].y + cross_xy_target_misail[target_misail[i].kind][1],
				sparker.x + sparker.cx2, target_misail[i].x + cross_xy_target_misail[target_misail[i].kind][2],
				sparker.y + sparker.cy2, target_misail[i].y + cross_xy_target_misail[target_misail[i].kind][3]) &&
				sparker.twincle == 0 && sparker_bomb.life == 0 && sparker_boom.life == 0) {
				target_misail[i].life = 0;

				boom_sparker();
			}
		}
		if (target_item[i].life == 1) {
			if (check_cross(sparker.x + sparker.cx1, target_item[i].x + cross_xy_item[target_item[i].kind][0],
				sparker.y + sparker.cy1, target_item[i].y + cross_xy_item[target_item[i].kind][1],
				sparker.x + sparker.cx2, target_item[i].x + cross_xy_item[target_item[i].kind][2],
				sparker.y + sparker.cy2, target_item[i].y + cross_xy_item[target_item[i].kind][3]) &&
				sparker_bomb.life == 0 && sparker_boom.life == 0) {
				target_item[i].life = 0;
				play_sample(gamedata[SAMP_ITEM].dat, 255, 128, 1000, FALSE);
				if (target_item[i].kind == 1) {
					sparker.misail += 2;
					if (sparker.misail > 8)
						sparker.misail = 8;
				}
				if (target_item[i].kind == 2)
					sparker.bomb += 1;
			}
		}
	}
}

void draw_everyone()
{
	int i, k;

	if (stage_count > 0)
		blit(stagedata[STAGE].dat, page1, 0, stage_count, 0, 0, SCREEN_W, SCREEN_H);
	if (stage_count <= 0 && stage_count > -200) {
		blit(stagedata[STAGE].dat, page1, 0, 0, 0, -stage_count, SCREEN_W, SCREEN_H);
		blit(stagedata[STAGE_BOSS].dat, page1, 0, 0, 0, -stage_count - SCREEN_H, SCREEN_W, SCREEN_H);
	}
	if (stage_count <= -200) {
		blit(stagedata[STAGE_BOSS].dat, page1, 0, 0, 0, -stage_count - SCREEN_H, SCREEN_W, SCREEN_H);
		blit(stagedata[STAGE_BOSS].dat, page1, 0, 0, 0, -stage_count - SCREEN_H * 2, SCREEN_W, SCREEN_H);
	}

	for (i = 0; i < 64; i++)
		if (target[i].life > 0 && (target[i].kind <= 8 || target[i].kind > 12) && target[i].kind != 14 && game_roop % 2)
			circlefill(page1, target[i].x + cross_xy_target[target[i].kind][0][2], target[i].y + cross_xy_target[target[i].kind][0][3], cross_xy_target[target[i].kind][0][2] / 8, 0);
	if (sparker.twincle == 0 && game_roop % 2) {
		if (sparker_bomb.life == 0)
			circlefill(page1, sparker.x + sparker.cx2, sparker.y + sparker.cy2, 4, 0);
		if (sparker_bomb.life == 1)
			circlefill(page1, sparker.x + sparker.cx2, sparker.y + sparker.cy2, 2, 0);
	}

	for (i = 0; i < 64; i++) {
		if (target[i].kind > 8 && target[i].kind <= 12) {
			if (target[i].life > 0) {
				draw_sprite(page1, gamedata[target[i].sprite_number + target[i].motion].dat, target[i].x, target[i].y);
				if (target[i].kind == 9) {
					if (target[i].motion == 0 && target[i].motion_count < 256) {
						if (target[i].x < 144) {
							if (target[i].motion_count % 2) {
								for (k = 0; k < 2; k++) {
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_01].dat, target[i].x + 4, target[i].y - 2 + (k * 18));
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_02].dat, target[i].x - 4, target[i].y - 2 + (k * 18));
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_03].dat, target[i].x - 6, target[i].y - 2 + (k * 18));
								}
							}
						}
						if (target[i].x >= 144) {
							if (target[i].motion_count % 2) {
								for (k = 0; k < 2; k++) {
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_01].dat, target[i].x + 18, target[i].y - 2 + (k * 18));
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_02].dat, target[i].x + 26, target[i].y - 2 + (k * 18));
									draw_sprite(page1, gamedata[TARGET_TANK_DUST_03].dat, target[i].x + 28, target[i].y - 2 + (k * 18));
								}
							}
						}
					}
				}
				if (target[i].kind == 10) {
					if (target[i].motion == 0 && target[i].motion_count < 128) {
						if (target[i].motion_count % 2) {
							for (k = 0; k < 2; k++) {
								draw_sprite(page1, gamedata[TARGET_TANK_DUST_01].dat, target[i].x - 2 + (k * 18), target[i].y - 2);
								draw_sprite(page1, gamedata[TARGET_TANK_DUST_02].dat, target[i].x - 2 + (k * 18), target[i].y - 6);
								draw_sprite(page1, gamedata[TARGET_TANK_DUST_03].dat, target[i].x - 2 + (k * 18), target[i].y - 12);
							}
						}
					}
				}
				if (target[i].kind == 11) {
					if (target[i].motion == 0 && target[i].motion_count < 256) {
						draw_sprite(page1, gamedata[TARGET_SHIP_A_FADO_01 + target[i].motion_count % 2].dat, target[i].x + 193, target[i].y - 8);
						draw_sprite(page1, gamedata[TARGET_SHIP_A_FADO_01 + target[i].motion_count % 2].dat, target[i].x + 193, target[i].y + 24);
					}
				}
				if (target[i].kind == 12) {
					if (target[i].motion == 0) {
						draw_sprite(page1, gamedata[TARGET_SHIP_B_FADO_01 + target[i].motion_count % 2].dat, target[i].x + 2, target[i].y - 32);
						draw_sprite(page1, gamedata[TARGET_SHIP_B_FADO_01 + target[i].motion_count % 2].dat, target[i].x + 20, target[i].y - 32);
					}
				}
			}
		}
	}
	for (i = 0; i < 64; i++) {
		if (target[i].kind > 12) {
			if (target[i].kind == 13 && stage_number == 0) {
				if (target[i].life > 0) {
					draw_sprite(page1, stagedata[target[i].sprite_number + target[i].motion].dat, target[i].x, target[i].y);
					for (k = 0; k < 3; k++) {
						if (target[i].y >= -48 + (k * 4) && target[i].y < -38 + (k * 4) + 4) {
							draw_sprite(page1, stagedata[BOSS_FIRE_01 + k].dat, target[i].x + 69, target[i].y + 62);
							draw_sprite(page1, stagedata[BOSS_FIRE_01 + k].dat, target[i].x + 112, target[i].y + 62);
						}
					}
					if (target[i].y >= -32) {
						draw_sprite(page1, stagedata[BOSS_FIRE_04].dat, target[i].x + 69, target[i].y + 62);
						draw_sprite(page1, stagedata[BOSS_FIRE_04].dat, target[i].x + 112, target[i].y + 62);
					}
				}
			}
			if (target[i].kind == 14 && stage_number == 1) {
				draw_sprite(page1, stagedata[target[i].sprite_number].dat, target[i].x, target[i].y);
				if (target[i].life > 0) {
					if (target[i].x < 0) {
						draw_sprite(page1, stagedata[BOSS_FIRE_01 + target[i].motion_count % 2].dat, target[i].x - 32, target[i].y);
						draw_sprite(page1, stagedata[BOSS_FIRE_01 + target[i].motion_count % 2].dat, target[i].x - 32, target[i].y + 48);
					}
					for (k = 0; k < 4; k++) {
						if (k < 2) {
							if (target[i].x <= -192)
								draw_sprite(page1, stagedata[BOSS_FIRE_03].dat, target[i].x + 99 + (k * 32), target[i].y + 24);
							if (target[i].x > -192)
								draw_sprite(page1, stagedata[BOSS_FIRE_04].dat, target[i].x + 99 + (k * 32), target[i].y + 24);
						}
						if (target[i].x <= -192)
							draw_sprite(page1, stagedata[BOSS_FIRE_03].dat, target[i].x + 196 + (k * 24), target[i].y + 24);
						if (target[i].x > -192)
							draw_sprite(page1, stagedata[BOSS_FIRE_04].dat, target[i].x + 196 + (k * 24), target[i].y + 24);
					}
				}
				if (target[i].life <= 400)
					draw_sprite(page1, stagedata[BOSS_02].dat, target[i].x + 324, target[i].y);
				if (target[i].life <= 0)
					draw_sprite(page1, stagedata[BOSS_02].dat, target[i].x + 47, target[i].y);
			}
			if (target[i].kind == 15 && stage_number == 2) {
				draw_sprite(page1, stagedata[target[i].sprite_number + target[i].motion].dat, target[i].x, target[i].y);
				if (target[i].motion == 0 && stage_count < -200) {
					if (target[i].y <= 0) {
						for (k = 0; k < 2; k++)
							draw_sprite(page1, stagedata[BOSS_FIRE_01].dat, target[i].x + 34 + (k * 42), target[i].y + 44);
					}
					if (target[i].y > 0) {
						for (k = 0; k < 2; k++)
							draw_sprite(page1, stagedata[BOSS_FIRE_02].dat, target[i].x + 34 + (k * 42), target[i].y + 44);
					}
					if (target[i].motion_count % 128 >= 64 && target[i].motion_count % 2 == 0) {
						draw_sprite(page1, stagedata[BOSS_FIRE_03].dat, target[i].x - 32, target[i].y + 112);
						draw_sprite(page1, stagedata[BOSS_FIRE_04].dat, target[i].x + 128, target[i].y + 112);
					}
				}
			}
		}
	}
	for (i = 0; i < 64; i++)
		if (target[i].kind <= 8)
			if (target[i].life > 0)
				draw_sprite(page1, gamedata[target[i].sprite_number + target[i].motion].dat, target[i].x, target[i].y);
	for (i = 0; i < 64; i++)
		if (target_misail[i].life == 1)
			draw_sprite(page1, gamedata[target_misail[i].sprite_number + target_misail[i].motion].dat, target_misail[i].x, target_misail[i].y);
	for (i = 0; i < 64; i++)
		if (target_spark[i].life == 1)
			draw_sprite(page1, gamedata[target_spark[i].sprite_number + target_spark[i].motion].dat, target_spark[i].x, target_spark[i].y);
	for (i = 0; i < 64; i++)
		if (target_piece[i].life == 1)
			draw_sprite(page1, gamedata[target_piece[i].sprite_number + target_piece[i].motion].dat, target_piece[i].x, target_piece[i].y);
	for (i = 0; i < 64; i++)
		if (target_boom[i].life == 1)
			stretch_sprite(page1, gamedata[target_boom[i].sprite_number + target_boom[i].motion].dat, target_boom[i].x, target_boom[i].y, target_boom[i].sx, target_boom[i].sy);
	for (i = 0; i < 64; i++)
		if (target_item[i].life == 1)
			draw_sprite(page1, gamedata[target_item[i].sprite_number + target_item[i].motion].dat, target_item[i].x, target_item[i].y);
	if (sparker_bomb.life == 1 && game_roop % 2)
		stretch_sprite(page1, gamedata[sparker_bomb.sprite_number + sparker_bomb.motion].dat, sparker_bomb.x, sparker_bomb.y, 320, 256);
	if (sparker_boom.life == 1 && sparker_boom.motion_count < 8)
		draw_sprite(page1, gamedata[sparker_boom.sprite_number + sparker_boom.motion].dat, sparker_boom.x, sparker_boom.y);
	for (i = 0; i < 256; i++)
		if (sparker_misail[i].life == 1)
			draw_sprite(page1, gamedata[SPARKER_MISAIL].dat, sparker_misail[i].x, sparker_misail[i].y);
	if (sparker_misail_sparker.life == 1)
		stretch_sprite(page1, gamedata[SPARKER_03].dat, sparker_misail_sparker.x + 8, sparker_misail_sparker.y + 8, 16, 16);
	if (sparker_boom.life == 0) {
		if (sparker_bomb.life == 0) {
			if (sparker.twincle == 0) {
				draw_sprite(page1, gamedata[SPARKER_01 + sparker.motion].dat, sparker.x, sparker.y);
				draw_sprite(page1, gamedata[SPARKER_PROPELLER_01 + sparker.propeller].dat, sparker.x + 14, sparker.y + 1);
			}
			if (sparker.twincle > 0) {
				if (sparker.twincle % 2) {
					draw_sprite(page1, gamedata[SPARKER_01 + sparker.motion].dat, sparker.x, sparker.y);
					draw_sprite(page1, gamedata[SPARKER_PROPELLER_01 + sparker.propeller].dat, sparker.x + 14, sparker.y + 1);
				}
			}
		}
		if (sparker_bomb.life == 1) {
			stretch_sprite(page1, gamedata[SPARKER_01 + sparker.motion].dat, sparker.x - (((40 + (sparker.motion - 5) * 8) / 2) - 16), sparker.y - (((40 + (sparker.motion - 5) * 8) / 2) - 16), 40 + (sparker.motion - 5) * 8, 40 + (sparker.motion - 5) * 8);
		}
	}
	if (sparker_misail_sparker.life == 0 && sparker.misail_count >= 32 && sparker.misail_count % 2)
		stretch_sprite(page1, gamedata[SPARKER_03].dat, sparker.x + 8, sparker.y, 16, 16);

	for (i = 0; i < sparker.life; i++)
		draw_sprite(page1, gamedata[ITEM_SPARKER].dat, i * 16, 16);
	for (i = 0; i < sparker.bomb; i++)
		draw_sprite(page1, gamedata[ITEM_BOMB].dat, i * 16, 184);
}

void clear_mater()
{
	int i;

	game_roop = 0;
	game_roop_switch = 0;

	sparker.start = 0;
	sparker.clear = 0;
	sparker.motion_count = 0;
	sparker.twincle = 0;
	for (i = 0; i < 256; i++)
		sparker_misail[i].life = 0;
	sparker_misail_sparker.life = 0;
	sparker_bomb.life = 0;
	sparker_boom.life = 0;

	stage_count = 800;
	stage_event_count = 0;
	stage_boss_flag = 0;

	for (i = 0; i < 64; i++) {
		target[i].life = 0;
		target_misail[i].life = 0;
		target_spark[i].life = 0;
		target_piece[i].life = 0;
		target_boom[i].life = 0;
		target_item[i].life = 0;
	}
}

void playing()
{
	clear(screen);

	set_palette(gamedata[SPARKER_PALETTE].dat);

	load_stagefile();

	clear_mater();

	sparker.start = 1;
	sparker.motion = 2;
	sparker.twincle = 1;
	sparker.x = 144;
	sparker.y = 200;

	while (1) {
		control_game_roop_speed();

		if (game_roop_switch == 1) {
			if (key[KEY_ESC]) {
				stop_midi();
				draw_title();
				playing();
			}
			if (key[KEY_ENTER] && key_start == 0) {
				clear_keybuf();
				draw_sprite(screen, gamedata[PAUSE].dat, 102, 90);
				while (!key[KEY_ENTER]);
				key_start = 1;
			}
			if (!key[KEY_ENTER] && key_start == 1)
				key_start = 0;
			if (key[KEY_F5]) {
				stop_midi();
				stage_number += 1;
				playing();
			}
			if (key[KEY_F6]) {
				sparker.life = 2;
				sparker.bomb = 2;
				sparker.misail = 8;
			}

			clear(page1);

			move_everyone();

			check_cross_everyone();

			draw_everyone();

			blit(page1, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

			if (sparker.life < 0 && sparker_boom.motion_count == 24) {
				stop_midi();
				draw_title();
				playing();
			}

			if (sparker.clear == 1 && sparker.y <= -64) {
				stage_number += 1;
				stop_midi();
				if (stage_number == 4)
					GameEnd();
				playing();
			}

			game_roop++;
			if (game_roop > 32000)
				game_roop = 0;

			game_roop_switch = 0;
		}
	}
}

void GameEnd()
{
	clear(page1);
	clear(page2);

	clear(screen);

	allegro_exit();
	printf("Thanks for playing! -ProYBY\n\n");
}

int main(void)
{
	openning();
	playing();

	return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif

#include <allegro.h>
#include "mappyal.h"



#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define WHITE makecol(255,255,255)
#define JUMPIT 800

//define the sprite structure
typedef struct SPRITE
{
	int dir, alive;
	int x, y;
	int width, height;
	int xspeed, yspeed;
	int xdelay, ydelay;
	int xcount, ycount;
	int curFrm, maxFrm;
	int frmCount, frmDelay;
}SPRITE;

//declare the bitmaps and sprites
BITMAP* player_image[3];
BITMAP* ai_image[3];
SPRITE* player;
SPRITE* ai;
BITMAP* buffer;
BITMAP* temp;
BITMAP* temp2;
SPRITE* ai2;

//set up variables 
bool instruct = false;
bool next = false;
bool contin = false;
bool aileft = false;
bool airight = false;
bool ai2left = false;
bool ai2right = false;
SAMPLE* sample;
//player
int height, width;
int n = 3;
int x, y;
int x1, y1;
int jump = 800;
int dir = 0;
int m;
int oldpy, oldpx;
int life = 5;
int mapxoff, mapyoff;
int score = 0;
//AI
int aix, aiy;
int ai2x, ai2y;
int aimove = 1;
//difficulty increase variable
int j = 0;


//tile grabber
BITMAP* grabframe(BITMAP* source,
	int width, int height,
	int startx, int starty,
	int columns, int frame)
{
	BITMAP* temp = create_bitmap(width, height);
	int x = startx + (frame % columns) * width;
	int y = starty + (frame / columns) * height;
	blit(source, temp, x, y, 0, 0, width, height);
	return temp;
}
BITMAP* grabframe2(BITMAP* source,
	int width, int height,
	int startx, int starty,
	int columns, int frame)
{
	BITMAP* temp2 = create_bitmap(width, height);
	int x1 = startx + (frame % columns) * width;
	int y1 = starty + (frame / columns) * height;
	blit(source, temp2, x1, y1, 0, 0, width, height);
	return temp2;
}

int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

//functions
//exit program
volatile int exit_program;
void progexit()
{
	exit_program = TRUE;
}
END_OF_FUNCTION(exit_program);


void setupmusic()
{
	//set up music
	int volume = 64;
	int pan = 128;
	int pitch = 1000;
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, "A");

	//load some music
	sample = load_sample("downtown.wav");
	play_sample(sample, volume, pan, pitch, true);
}

/////////////////////////////////////////////////////////////////////////
//Set up screen, set resolution
/////////////////////////////////////////////////////////////////////////

void setupscreen()
{
	int ret = set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
	if (ret != 0)
	{
		allegro_message(allegro_error);
	}
	textprintf_ex(screen, font, 5, 2, 15, -1,
		" JumpBlock - %dx%d - Press ESC to quit ",
		SCREEN_W, SCREEN_H);
	set_window_title("Jump Block!!");
}

bool instruction() {
	instruct = true;
	return instruct;
}


int main(void)
{
	allegro_init();
	install_keyboard();

	setupscreen();

	//Screen Menu
	while (!contin)
	{
		textprintf(screen, font, 25, 25, 10, "Welcome to Jump block, Press Space bar to continue.");
		textprintf(screen, font, 25, 45, 15, "Press Enter for instruction.");

		//Game Instruction if enter is pressed
		if (key[KEY_ENTER]) {
			instruction();
		}

		if (instruct == true) {
			rectfill(screen, 25, 25, SCREEN_H, SCREEN_W, 0);
			textprintf(screen, font, 25, 25, 15, "Help Bob to reach for the sky, ");
			textprintf(screen, font, 25, 45, 15, "by helping Bob get into the doors safely.");
			textprintf(screen, font, 25, 65, 15, "Press Space bar to jump");
			textprintf(screen, font, 25, 85, 15, "Press Left and Right to move left and right");
			rest(7000);
			rectfill(screen, 25, 25, SCREEN_H, SCREEN_W, 0);
			instruct = false;
		}

		if (key[KEY_ESC])
		{
			progexit();
			contin = true;
		}

		if (key[KEY_SPACE])
		{
			contin = true;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	//Game loop
	/////////////////////////////////////////////////////////////////////////

	set_color_depth(16);
	setupmusic();
	set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);

	//Load Bob
	temp = load_bitmap("blocks.bmp", NULL);
	for (m = 0; m < 3; m++)
		player_image[m] = grabframe(temp, 32, 32, 32, 0, 3, m);
	destroy_bitmap(temp);

	temp2 = load_bitmap("ai.bmp", NULL);
	for (m = 0; m < 3; m++)
		ai_image[m] = grabframe2(temp2, 32, 32, 32, 0, 3, m);
	destroy_bitmap(temp2);

	//Set character values
	player = (SPRITE*)malloc(sizeof(SPRITE));
	player->x = 10;
	player->y = 400;
	player->curFrm = 2;
	player->frmCount = 0;
	player->frmDelay = 3;
	player->maxFrm = 1;
	player->width = player_image[0]->w;
	player->height = player_image[0]->h;

	//first AI
	ai = (SPRITE*)malloc(sizeof(SPRITE));
	ai->x = 288;
	ai->y = 320;
	ai->curFrm = 1;
	ai->frmCount = 0;
	ai->frmDelay = 3;
	ai->maxFrm = 1;
	ai->width = ai_image[0]->w;
	ai->height = ai_image[0]->h;
	height = ai->height;
	width = ai->width;
	//2nd AI
	ai2 = (SPRITE*)malloc(sizeof(SPRITE));
	ai2->x = 300;
	ai2->y = 64;
	ai2->curFrm = 1;
	ai2->frmCount = 0;
	ai2->frmDelay = 3;
	ai2->maxFrm = 1;
	ai2->width = ai_image[0]->w;
	ai2->height = ai_image[0]->h;

	//load the Mappy file
	if (MapLoad((char*)"bgImg.fmp") != 0)
	{
		allegro_message("Can't find bgImg.fmp");
		return 1;
	}

	buffer = create_bitmap(WIDTH, HEIGHT);
	clear(buffer);

	while (!exit_program)
	{
		if (key[KEY_ESC])
		{
			progexit();
		}
		textprintf_ex(buffer, font, 10, 10, WHITE, -1, "Keys: Left, Right, Space bar");
		textprintf_ex(buffer, font, 10, 20, WHITE, -1, "Press Esc to quit the game");
		textprintf_ex(buffer, font, 10, 40, WHITE, -1, "Level: %d, life %d", score, life);

		//Testing purpose 
		//textprintf_ex(buffer, font, 10, 10, WHITE, -1, "mapxoff= %d, mapyoff= %d, oldpy= %d, oldpx= %d, currentFrame= %d, direction= %d ", mapxoff, mapyoff, oldpy, oldpx, player->curFrm, dir);
		//textprintf_ex(buffer, font, 10, 20, WHITE, -1, "frameCount= %d, frameDelay= %d, x= %d, y= %d, ai->x %d", player->frmCount, player->frmDelay, x, y, ai->x);
		//textprintf_ex(buffer, font, 10, 30, WHITE, -1, "ai->curFrm %d, ai->x %d, ai->width %d, ai->y %d, ai->height %d", ai->curFrm, ai->x, ai->width, ai->y, ai->height);
		//textprintf_ex(buffer, font, 10, 50, WHITE, -1, "player->x %d, player->x + width %d, player->y %d, player->y + height %d", player->x, player->x + width, player->y, player->y + width);

		blit(buffer, screen, 0, 0, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
		oldpy = player->y;
		oldpx = player->x;
		//1st ai x, y value
		aix = ai->x;
		aiy = ai->y;
		//2nd ai x, y value
		ai2x = ai2->x;
		ai2y = ai2->y;

		if (key[KEY_RIGHT])
		{
			if (oldpx < (SCREEN_W - 31))
			{
				dir = 2;
				player->x += 3;
				if (++player->frmCount > player->frmDelay)
				{
					player->frmCount = 0;
					if (++player->curFrm > player->maxFrm)
						player->curFrm = 1;
				}
			}
		}
		else if (key[KEY_LEFT])
		{
			if (oldpx > 1)
			{
				dir = 1;
				player->x -= 3;
				if (++player->frmCount > player->frmDelay)
				{
					player->frmCount = 0;
					player->curFrm = 0;
				}
			}
		}
		else player->curFrm = 2;

		//handle jumping
		if (jump == JUMPIT)
		{
			if (!collided(player->x + player->width / 2,
				player->y + player->height + 5))
				jump = 0;

			if (key[KEY_SPACE])
				jump = 22 - j;
		}
		else
		{
			player->y -= jump / 3;
			jump--;
		}

		if (jump < 0)
		{
			if (collided(player->x + player->width / 2,
				player->y + player->height))
			{
				jump = JUMPIT;
				while (collided(player->x + player->width / 2,
					player->y + player->height))
					player->y -= 2;
			}
		}

		//check for collided with foreground tiles
		if (!dir)
		{
			if (collided(player->x, player->y + player->height))
				player->x = oldpx;
		}
		else
		{
			if (collided(player->x + player->width,
				player->y + player->height))
				player->x = oldpx;
		}

		//if sprite walks into door game loops
		if (oldpy > 61 && oldpy < 64 && (oldpx > 598 && oldpx < 620))
		{
			//TODO: implement loop
			player->x = 10;
			player->y = 574;
			oldpx -= 5;
			score += 1;
			if (score == 2 || score == 3)
			{
				//increase difficulty
				j += 1;
			}
		}

		//update the map scroll position
		mapxoff = player->x + player->width / 2 - WIDTH / 2 + 10;
		mapyoff = player->y + player->height / 2 - HEIGHT / 2 + 10;


		//avoid moving beyond the map edge
		if (mapxoff < 30) mapxoff = 0;
		if (mapxoff > (mapwidth * mapblockwidth - WIDTH))
			mapxoff = mapwidth * mapblockwidth - WIDTH;
		if (mapyoff < 30)
			mapyoff = 0;
		if (mapyoff > (mapheight * mapblockheight - HEIGHT))
			mapyoff = (mapheight * mapblockheight - HEIGHT);

		//draw the background tiles
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, WIDTH, HEIGHT);

		//draw foreground tiles
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, WIDTH, HEIGHT, 0);


		//draw first ai sprite
		if (ai->x >= 288)
		{
			aileft = true;
			airight = false;
			ai->curFrm = 0;
		}
		if (ai->x <= 158)
		{
			airight = true;
			aileft = false;
			ai->curFrm = 2;
		}
		if (aileft == true)
		{
			ai->x -= 1 + j;
		}
		else if (airight == true)
		{
			ai->x += 1 + j;
		}

		//draw 2nd ai sprite 
		if (ai2->x >= 260)
		{
			ai2left = true;
			ai2right = false;
			ai2->curFrm = 0;
		}
		if (ai2->x <= 164 && aileft == true)
		{
			ai2right = true;
			ai2left = false;
			ai2->curFrm = 2;
		}
		if (ai2left == true)
		{
			ai2->x -= 1 + j;
		}
		else if (airight == true)
		{
			ai2->x += 1 + j;
		}
		draw_sprite(buffer, ai_image[ai->curFrm], (ai->x - mapxoff), (ai->y - mapyoff));
		draw_sprite(buffer, ai_image[ai2->curFrm], (ai2->x - mapxoff), (ai2->y - mapyoff));

		//detect player vs ai collision
		//1st AI 
		if (ai->x - 3 < (player->x) && player->x < (ai->x + width) + 3)
		{
			if (ai->y - 4 < (player->y) && player->y < (ai->y + height) + 4)
			{
				player->x = 10;
				player->y = 574;
				life -= 1;
			}
		}

		//2nd AI
		if (ai2->x - 3 < (player->x) && player->x < (ai2->x + width) + 3)
		{
			if (ai2->y - 4 < (player->y) && player->y < (ai2->y + height) + 4)
			{
				player->x = 10;
				player->y = 574;
				life -= 1;
			}
		}

		//testing purpose
		//rectfill(buffer, ai->x - mapxoff, ai->y - mapyoff, ai->x + ai->width - mapxoff, ai->y + ai->height - mapyoff, 0);
		//draw the player's sprite

		if (dir)
			draw_sprite(buffer, player_image[player->curFrm],
				(player->x - mapxoff), (player->y - mapyoff + 1));
		else
			draw_sprite_h_flip(buffer, player_image[player->curFrm],
				(player->x - mapxoff), (player->y - mapyoff));

		//blit the double buffer 
		vsync();
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, WIDTH - 1, HEIGHT - 1);
		release_screen();

		//pthread_mutex_unlock(&threadsafe);

		//while
	}

	for (m = 0; m < 3; m++)
		destroy_bitmap(player_image[m]);
	free(player);

	for (m = 0; m < 3; m++)
		destroy_bitmap(ai_image[m]);

	free(ai);
	free(ai2);

	//delete double buffer
	destroy_bitmap(buffer);
	destroy_sample(sample);

	//delete the Mappy level
	MapFreeMem();
	allegro_exit();

	return 0;
}
END_OF_MAIN()

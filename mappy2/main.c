#include <allegro.h>
#include "mappyal.h"
#include "player.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

BITMAP* buffer;

int mapxoff = 0, mapyoff = 0; //������ũ���� ���� x, y ��ǥ

int main(void)
{
	allegro_init(); //�˷��׷� ���� �ʱ�ȭ
	install_keyboard(); //Ű���� ��ġ
	set_color_depth(16);

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0); //640 * 480 �ػ�

	

	//���� �� ������ �ε�
	if (MapLoad((char*)"simple.fmp") != 0)
	{
		allegro_message("mappy map file load fail!!");
		return 1;
	}

	//�÷��̾� �ε�
	InitPlayer("guy.bmp");

	buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	clear(buffer);

	while (TRUE)
	{

		if (key[KEY_ESC])
		{
			break;
		}

		
		mapxoff = player->x + player->width / 2 - SCREEN_WIDTH / 2 + 10;
		mapyoff = player->y + player->height / 2 - SCREEN_HEIGHT / 2 + 10;

		//avoid moving beyond the map edge
		if (mapxoff < 0) mapxoff = 0;
		if (mapxoff > (mapwidth * mapblockwidth - SCREEN_WIDTH))
			mapxoff = mapwidth * mapblockwidth - SCREEN_WIDTH;
		if (mapyoff < 0)
			mapyoff = 0;
		if (mapyoff > (mapheight * mapblockheight - SCREEN_HEIGHT))
			mapyoff = mapheight * mapblockheight - SCREEN_HEIGHT;

		UpdatePlayer();

		//��׶��� Ÿ���� ���� �׸���.
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		//���׶��� Ÿ���� �׸���.
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 0);

		RenderPlayer(buffer, mapxoff, mapyoff);

		//ȭ����ۿ� �����Ѵ�.
		vsync();
		acquire_screen();
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
		release_screen();
	}

	destroy_bitmap(buffer);

	MapFreeMem(); //���� �� �����͸� �ı��Ѵ�.

	allegro_exit();

	return 0;
}
END_OF_MAIN()

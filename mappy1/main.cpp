#include <allegro.h>
#include "mappyal.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

BITMAP* buffer;
int mapxoff = 0, mapyoff = 0; //������ũ���� ���� x, y ��ǥ

int main(void)
{
	allegro_init(); //�˷��׷� ���� �ʱ�ȭ
	install_keyboard(); //Ű���� ��ġ

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0); //640 * 480 �ػ�

	//���� �� ������ �ε�
	if (MapLoad((char*)"simple.fmp") != 0)
	{
		allegro_message("mappy map file load fail!!");
		return 1;
	}

	buffer = create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	clear(buffer);

	while (TRUE)
	{
		if (key[KEY_ESC])
		{
			break;
		}	

		//��׶��� Ÿ���� ���� �׸���.
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//���׶��� Ÿ���� �׸���.
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		//ȭ����ۿ� �����Ѵ�.
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	}

	destroy_bitmap(buffer);

	MapFreeMem(); //���� �� �����͸� �ı��Ѵ�.
	
	allegro_exit();

	return 0;
}
END_OF_MAIN()

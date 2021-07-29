#include <allegro.h>
#include "mappyal.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

BITMAP* buffer;
int mapxoff = 0, mapyoff = 0; //오프스크린을 위한 x, y 좌표

int main(void)
{
	allegro_init(); //알레그로 엔진 초기화
	install_keyboard(); //키보드 설치

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0); //640 * 480 해상도

	//매피 맵 데이터 로드
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

		//백그라운드 타일을 먼저 그린다.
		MapDrawBG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		//포그라운드 타일을 그린다.
		MapDrawFG(buffer, mapxoff, mapyoff, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

		//화면버퍼에 블리팅한다.
		blit(buffer, screen, 0, 0, 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1);
	}

	destroy_bitmap(buffer);

	MapFreeMem(); //매피 맵 데이터를 파괴한다.
	
	allegro_exit();

	return 0;
}
END_OF_MAIN()

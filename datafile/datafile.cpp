#include <allegro.h>
#include <stdlib.h> 
#include "image.h"

volatile long g_speed_counter = 0; //속도 카운터

void increment_speed_counter() //속도 카운터 값을 증가시키는 카운터 함수
{
	g_speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter); //타이머 함수가 끝난다는 것을 명시적으로 설정

typedef struct tag_OBJECT_INFO //오브젝트 정보 구조체
{
	int posX; //X 좌표
	int posY; //Y 좌표
	int width; //너비
	int height;//높이
}OBJECT_INFO;

void ProcessKey();
void RenderScene(BITMAP* buffer, BITMAP* image1, BITMAP* image2);
void DrawBoundingBox(BITMAP* buffer, OBJECT_INFO* pObject);
void CheckCollision();
void CheckBoundingBox();

int g_show_bbox = FALSE;
int g_collision = FALSE;

OBJECT_INFO g_object1;
OBJECT_INFO g_object2;

int main(int argc, char* argv[])
{
	allegro_init();
	install_keyboard();
	install_timer(); //타이머 시스템을 초기화한다

	LOCK_VARIABLE(g_speed_counter);
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //초당 타이머 함수 호출 수(BEAT PER SECOND)

	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);

	BITMAP* buffer = create_bitmap(640, 480);

	DATAFILE* pdatafile = load_datafile("datafile.dat");

	if (pdatafile == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Datafile Load Fail!");
		exit(EXIT_FAILURE);
	}

	BITMAP* imagePlane1 = (BITMAP*)pdatafile[sample_image1].dat; //첫번째 오브젝트를 위한 이미지
	BITMAP* imagePlane2 = (BITMAP*)pdatafile[sample_image2].dat; //두번째 오브젝트를 위한 이미지

	if (imagePlane1 == NULL || imagePlane2 == NULL)
	{
		set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
		allegro_message("Object Image Load Fail!!");
		exit(EXIT_FAILURE);
	}
	

	//오브젝트 정보 초기화
	g_object1.posX = 0;
	g_object1.posY = 0;
	g_object1.width = imagePlane1->w;
	g_object1.height = imagePlane1->h;

	g_object2.posX = 100;
	g_object2.posY = 100;
	g_object2.width = imagePlane2->w;
	g_object2.height = imagePlane2->h;

	while (!key[KEY_ESC])
	{
		//오브젝트의 좌표를 갱신한다.
		while (g_speed_counter > 0)
		{
			ProcessKey();
			g_speed_counter--;
		}

		//두 오브젝트가 충돌되었는지 확인한다
		CheckCollision();

		//바운딩박스를 출력 여부를 결정한다.
		CheckBoundingBox();

		//화면을 렌더링한다.
		RenderScene(buffer, imagePlane1, imagePlane2);
	}

	destroy_bitmap(buffer);
	unload_datafile(pdatafile);
	return 0;
}

void ProcessKey()
{
	if (key[KEY_LEFT])
		g_object1.posX--;

	if (key[KEY_RIGHT])
		g_object1.posX++;

	if (key[KEY_DOWN])
		g_object1.posY++;

	if (key[KEY_UP])
		g_object1.posY--;

	if (key[KEY_A])
		g_object2.posX--;

	if (key[KEY_D])
		g_object2.posX++;

	if (key[KEY_S])
		g_object2.posY++;

	if (key[KEY_W])
		g_object2.posY--;
}

void RenderScene(BITMAP* buffer, BITMAP* image1, BITMAP* image2)
{
	draw_sprite(buffer, image1, g_object1.posX, g_object1.posY);
	draw_sprite(buffer, image2, g_object2.posX, g_object2.posY);

	if (g_show_bbox)
	{
		DrawBoundingBox(buffer, &g_object1);
		DrawBoundingBox(buffer, &g_object2);
	}

	if (g_collision)
	{
		textprintf_ex(buffer, font, 0, 0, makecol(0, 0, 0), -1, "Collision detected.");
	}

	blit(buffer, screen, 0, 0, 0, 0, 640, 480);
	clear_to_color(buffer, makecol(255, 255, 255));
}

void CheckCollision()
{
	g_collision = TRUE;


	if (g_object1.posY + g_object1.height < g_object2.posY)
	{
		g_collision = FALSE;
	}
	else if (g_object1.posY > g_object2.posY + g_object2.height)
	{
		g_collision = FALSE;
	}
	else if (g_object1.posX + g_object1.width < g_object2.posX)
	{
		g_collision = FALSE;
	}
	else if (g_object1.posX > g_object2.posX + g_object2.width)
	{
		g_collision = FALSE;
	}
}

void CheckBoundingBox()
{
	if (keypressed() && key[KEY_SPACE])
	{
		if (g_show_bbox)
			g_show_bbox = FALSE;
		else
			g_show_bbox = TRUE;

	}
}

void DrawBoundingBox(BITMAP* buffer, OBJECT_INFO* pObject)
{
	line(buffer, pObject->posX, pObject->posY, pObject->posX + pObject->width, pObject->posY, makecol(255, 0, 0));
	line(buffer, pObject->posX, pObject->posY, pObject->posX + pObject->width, pObject->posY + pObject->height, makecol(255, 0, 0));
	line(buffer, pObject->posX, pObject->posY + pObject->height, pObject->posX + pObject->width, pObject->posY + pObject->height, makecol(255, 0, 0));
	line(buffer, pObject->posX, pObject->posY, pObject->posX, pObject->posY + pObject->height, makecol(255, 0, 0));
	line(buffer, pObject->posX + pObject->width, pObject->posY, pObject->posX + pObject->width, pObject->posY + pObject->height, makecol(255, 0, 0));
}

END_OF_MAIN()



	

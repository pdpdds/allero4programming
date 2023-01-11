#include <allegro.h> 

void ProcessKey();
void RenderBall(BITMAP* pBitmap, BITMAP* pBall);

//공의 x, y좌표. 초기좌표는 320, 200으로 설정
int g_xPos = 320;
int g_yPos = 200;

int main(int argc, char* argv[])
{
	allegro_init(); 
	install_keyboard(); 
	set_color_depth(16); 
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); //화면해상도는 640 * 480

	//공 이미지를 로드한다.
	BITMAP* pBall = NULL;  
	pBall = load_bitmap("ball.bmp", NULL);

	BITMAP* buffer = NULL; //후면 버퍼를 생성한다.
	buffer = create_bitmap(640, 480); 

	while (!key[KEY_ESC]) 
	{
		ProcessKey(); //키 입력을 처리한다.
		RenderBall(buffer, pBall); //더블 버퍼링을 사용해서 화면을 갱신한다.
	}

	destroy_bitmap(pBall); //공이미지를 파괴한다
	destroy_bitmap(buffer); //후면버퍼를 파괴한다.
	return 0;
}

void ProcessKey() //공 위치 갱신
{
	if (key[KEY_RIGHT])
	{
		g_xPos++;
	}
	else if (key[KEY_LEFT])
	{
		g_xPos--;
	}
	else if (key[KEY_UP])
	{
		g_yPos--;
	}
	else if (key[KEY_DOWN])
	{
		g_yPos++;
	}
}

void RenderBall(BITMAP* pBitmap, BITMAP* pBall) //화면 갱신
{
	draw_sprite(pBitmap, pBall, g_xPos, g_yPos);
	blit(pBitmap, screen, 0, 0, 0, 0, 640, 480);
	clear_bitmap(pBitmap);
}

END_OF_MAIN() 
#include <allegro.h> 

volatile long g_speed_counter = 0; //속도 카운터

void increment_speed_counter() //속도 카운터 값을 증가시키는 카운터 함수
{
	g_speed_counter++;
}
END_OF_FUNCTION(increment_speed_counter); //타이머 함수가 끝난다는 것을 명시적으로 설정

void ProcessKey();
void RenderBall(BITMAP* pBitmap, BITMAP* pBall);

//공의 x, y좌표. 초기좌표는 320, 200으로 설정
int g_xPos = 320;
int g_yPos = 200;

int main(int argc, char* argv[])
{
	allegro_init(); 
	install_keyboard(); 
	install_timer(); //타이머 시스템을 초기화한다

	LOCK_VARIABLE(g_speed_counter); 
	LOCK_FUNCTION(increment_speed_counter);

	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //초당 타이머 함수 호출 수(BEAT PER SECOND)

	
	set_color_depth(16); 
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0); 

	BITMAP* pBall;
	pBall = load_bitmap("ball.bmp", NULL);

	BITMAP* buffer; 

	buffer = create_bitmap(640, 480); 

	while (!key[KEY_ESC])
	{

		//g_speed_counter 값이 0이 될때까지 공의 좌표를 갱신한다.
		//타이머 호출 수 빈도에 따라 g_speed_counter 값이 커지면 커질 수록
		//공의 이동속도는 빨라질 것이다.
		while (g_speed_counter > 0) 
		{
			ProcessKey();
			g_speed_counter--;
		}

		RenderBall(buffer, pBall); //더블 버퍼링을 사용해서 화면을 갱신한다.
	} 

	
	destroy_bitmap(pBall);
	destroy_bitmap(buffer);
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
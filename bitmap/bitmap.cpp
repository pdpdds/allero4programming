#include <allegro.h>

int main(int argc, char* argv[])
{
	allegro_init();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
	BITMAP* pImage = NULL;

	pImage = load_bitmap("image.bmp", NULL);

	blit(pImage, screen, 0, 0, 0, 0, 200, 200);

	readkey();

	destroy_bitmap(pImage);

	return 0;
}

END_OF_MAIN()

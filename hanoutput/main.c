#include "main.h"
#include <allegro.h>

char menu_text[10][256] = 
{
	"한글출력",
	"English Font",
	0, 
};

#define DAT_ENG_FNT                      0        /* DATA */
#define DAT_HAN_FNT                      1        /* DATA */

void load_data_callback(DATAFILE* dat)
{

}

int IsExist(AL_CONST char* filename)
{
	ASSERT(filename);
	return file_exists(filename, FA_ARCH | FA_RDONLY, NULL);
}

void err(char* errmsg)
{
	set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	allegro_message("%s", errmsg);
	exit(1);
}

void load_kssm_font()
{
	DATAFILE* font_data = NULL;
	font_data = load_datafile_callback("FONT.DAT", load_data_callback);
	if (font_data) {
		al_load_buff_hanfont(font_data[DAT_HAN_FNT].dat);
		al_load_buff_engfont(font_data[DAT_ENG_FNT].dat);
	}
	else {
		err("Couldn't load font resources!");
	}

	unload_datafile(font_data);
	font_data = NULL;
}

int main(void)
{
	/* 알레그로 엔진의 초기화*/
	if (allegro_init() != 0)
		return 1;

	load_kssm_font();

	BITMAP* screen2; //더블 버퍼링

	/* 키보드 핸들러 설정 */
	install_keyboard();

#ifdef _WIN32
	if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 320, 200, 0, 0) != 0) {
#else
	/* 320x200으로 그래픽 모드 설정 */
	if (set_gfx_mode(GFX_AUTODETECT, 320, 200, 0, 0) != 0) {
#endif
		if (set_gfx_mode(GFX_SAFE, 320, 200, 0, 0) != 0) {
			set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
			allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
			return 1;
		}
	}

	screen2 = create_bitmap(screen->w, screen->h); //Set up double buffer
	if (!screen2) {
		allegro_message("Failed to create double buffer. Out of memory?");
		exit(1);
	}

	/* 컬러 팔레트 설정*/
	set_palette(desktop_palette);

	/* 화면을 흰색으로 설정한다 */
	clear_to_color(screen, makecol(255, 255, 255));


	List* menu_list = NULL;
	list_new(&menu_list);

	for (int i = 0; i < 2; i++)
	{
		char* text = menu_text[i];
		if (text == NULL) break;

		char* kssm_str = malloc(strlen(text) + 1);
		han_conv(KS2TG, text, kssm_str);
		list_add(menu_list, kssm_str);
	}

	int pady = 70;
	int menu_height = list_size(menu_list) * 16;


	while (!key[KEY_ESC]) {

		clear_bitmap(screen2);

		for (int i = 0; i < (int)list_size(menu_list); i++)
		{
			void* v = 0;
			list_get_at(menu_list, i, (void*)&v);
			char* text = (char*)v;

			int x = SCREEN_W / 2;
			int y = (SCREEN_H / 2) - (menu_height / 2) + (i * 16) + pady;
			int text_color = makecol(255, 0, 0);

			al_han_textout_centre_ex(screen2, text, x, y, text_color);
		}

		blit(screen2, screen, 0, 0, 0, 0, screen->w, screen->h);

		rest(1);
	}

	list_remove_all_cb(menu_list, free);
	list_destroy(menu_list);

	return 0;
}

#ifdef _WIN32
END_OF_MAIN()
#endif
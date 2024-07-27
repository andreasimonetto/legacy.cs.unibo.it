#include "kk_scene.h"

int main(void)
{
ITEM scene;

	/*** Costruisce l'ambiente (attributi, luci, ...) ***/
	build_environment();

	/*** Costruisce la scena per il fotogramma 0 ***/
	scene = build_scene_frame(0, "Knick_Knack");

	/*** Salva la scena ottenuta ***/
	save_scene("Knick_Knack.md", "Knick_Knack_Scene", scene);
	return 0;
}


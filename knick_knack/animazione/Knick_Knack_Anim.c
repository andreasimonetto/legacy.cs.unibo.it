#include "kk_scene.h"

int main(void)
{
ITEM scene;
unsigned i;

	/*** Costruisce l'ambiente (attributi, luci, ...) ***/
	build_environment();

	/*** Per ogni fotogramma i dell'animazione ***/
	for(i = 0; i < Anim_Frames; i++) {
		/*** Costruisce la scena per il fotogramma i ***/
		scene = build_scene_frame(i, "Knick_Knack");
		/*** Salva il fotogramma ottenuto ***/
		save_scene_ani(Anim_Frames, "Knick_Knack_Anim.ani", "Knick_Knack_Anim", "Knick_Knack", scene);
	}
	return 0;
}


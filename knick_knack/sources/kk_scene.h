#ifndef _KNICK_KNACK_SCENE_
#define _KNICK_KNACK_SCENE_
#include "descriptor/descriptor.h"

/* Vettore di 3 float */
typedef float vect3f [3];

#define M 1.225

/* Numero di fotogrammi dell'animazione */
const unsigned Anim_Frames = 42;

/* Attributi */
extern ITEM sfondo_attr, corpo_attr, occhi_attr, bocca_attr, bottoni_attr,
	sopracciglia_attr, naso_attr, cravatta_attr, cappello_attr, tavolo_attr,
	cupola_attr, cupola_fronte_attr, cupola_retro_attr, igloo_attr, 
	lampada_attr, bollicine_attr;

/* Funzioni di costruzione della scena */
void build_environment(void);
ITEM build_scene_frame(unsigned frameno, const char *scene_name);
ITEM build_static_scene(void);
ITEM build_knick_knack(unsigned frameno);
ITEM build_cupola(unsigned frameno);
ITEM build_igloo(unsigned frameno);

#endif


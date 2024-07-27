#ifndef _UPLEZZ_H_
#define _UPLEZZ_H_
#include "mre/alpha.h"
#include "mre/image.h"

/* Titolo, larghezza e altezza della finestra di gioco */
#define WINDOW_TITLE  "UPLEZZ"
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

/* Distanza del timer dal bordo (superiore) della finestra */
#define TIMER_Y 10
/* Distanza della "mappa" dal bordo (sinistro) */
#define GRID_X 10
/* Dimensione di una cella */
#define CELL_SIZE 14.0

/* Immagine dell'help-online */
extern mre::image_t *helpOnlineImg;
/* Immagine della schermata di caricamento */
extern mre::image_t *splash1Img, *splash2Img;

/* Handler eventi */
void mouseButtonPressHandle(int button, int state, int x, int y);
void mouseButtonReleaseHandle(int button, int state, int x, int y);
void mouseMotionHandle(int state, int x, int y);
void keyPressHandle(int key, int state, int x, int y);
void keyReleaseHandle(int key, int state, int x, int y);

#endif


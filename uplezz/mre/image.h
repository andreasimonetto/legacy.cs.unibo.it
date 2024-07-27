#ifndef _MRE_IMAGE_
#define _MRE_IMAGE_

namespace mre {

/** Tipo immagine */
typedef struct {
	unsigned width;
	unsigned height;
	unsigned bytes_per_pixel;
	unsigned char pixel_data[0];
} image_t;

/** Rende un'immagine 2D in una certa posizione 
	\param[in] image Puntatore all'immagine da rendere
	\param[in] x Ascissa in cui posizionare l'immagine
	\param[in] y Ordinata in cui posizionare l'immagine
*/
void image_render(const image_t *image, unsigned x = 0, unsigned y = 0);

/** Ritorna il colore di un pixel.
	\param[in] image Puntatore all'immagine
	\param[in] x Ascissa del pixel
	\param[in] y Ordinata del pixel
	
*/
inline unsigned image_getpixel(image_t *image, unsigned x, unsigned y) {
	return image->pixel_data[image->bytes_per_pixel * (x + y * image->width)];
}

/** Setta il colore di un pixel.
	\param[in] image Puntatore all'immagine da modificare
	\param[in] x Ascissa del pixel da modificare
	\param[in] y Ordinata del pixel da modificare
	\param[in] col Colore del pixel in formato 0xAARRGGBB (esadecimale)
*/
inline void image_setpixel(image_t *image, unsigned x, unsigned y, unsigned col) {
	unsigned char *pixel_data = image->pixel_data + image->bytes_per_pixel * (x + y * image->width);
	if(image->bytes_per_pixel == 4) {
		*((unsigned*)pixel_data) = col;
	}
	else {
		pixel_data[0] = col & 0xFF;
		if(image->bytes_per_pixel > 1)
			pixel_data[1] = (col & 0xFF00) >> 16;
		if(image->bytes_per_pixel > 2)
			pixel_data[2] = (col & 0xFF0000) >> 24;
	}
}

};

#endif


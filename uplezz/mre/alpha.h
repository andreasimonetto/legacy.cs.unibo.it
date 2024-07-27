#ifndef _MRE_ALPHA_
#define _MRE_ALPHA_
#include "mre/image.h"

namespace mre {

/** Larghezza di un carattere alfanumerico. */
#define ALPHA_WIDTH  15
/** Altezza di un carattere alfanumerico. */
#define ALPHA_HEIGHT 21

/** Vettore di caratteri alfanumerici. */
extern image_t *alpha_images[];

/** Enumerazione dei caratteri. */
enum AlphaImages {
	/** Cifra 0 */
	ALPHA0,
	/** Cifra 1 */
	ALPHA1,
	/** Cifra 2 */
	ALPHA2,
	/** Cifra 3 */
	ALPHA3,
	/** Cifra 4 */
	ALPHA4,
	/** Cifra 5 */
	ALPHA5,
	/** Cifra 6 */
	ALPHA6,
	/** Cifra 7 */
	ALPHA7,
	/** Cifra 8 */
	ALPHA8,
	/** Cifra 9 */
	ALPHA9,
	/** Carattere duepunti */
	ALPHA_COLON
};

};
#endif


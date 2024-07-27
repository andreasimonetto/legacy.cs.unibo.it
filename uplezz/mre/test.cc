#include "mre/RenderEngine"
#include "mre/SceneFactory"
#include "mre/SceneManager"

#define WINDOW_TITLE "MRE TEST"
#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480

using namespace mre;
using namespace sys;

// Gestione della tastiera: richiede all'engine di uscire quando viene rilasciato il tasto ESC.
void keyReleaseHandle(int key, int state, int x, int y)
{
	if(key == XK_Escape)
		RenderEngine::getInstance()->postQuit();
}

int main(int argc, char *argv[])
{
Light *lite;
Sphere *sphere;
Camera *camera;

	// Inizializza la finestra principale
	RenderEngine::getInstance()->initMainWindow(argc, argv, WINDOW_TITLE, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	// Crea una camera, la posiziona e la setta come corrente nella scena
	camera = SceneFactory::getInstance()->createCamera("DefaultCamera");
	camera->position[2] = -6.0;
	SceneManager::getInstance()->setCurrentCamera(camera);

	// Crea una luce, la posiziona e l'aggiunge alla scena
	lite = SceneFactory::getInstance()->createLight("DefaultLight");
	lite->setPosition(-1.0, 0.7071, -0.7071);
	SceneManager::getInstance()->addLight(lite);

	// Crea una sfera di raggio 2.5
	sphere = SceneFactory::getInstance()->createSphere(2.5);
	SceneManager::getInstance()->addObject(sphere);

	// Imposta il gestore dell'evento "tasto rilasciato"
	RenderEngine::getInstance()->setKeyReleaseHandler(keyReleaseHandle);

	// Inizia il main loop
	return RenderEngine::getInstance()->mainLoop();
}


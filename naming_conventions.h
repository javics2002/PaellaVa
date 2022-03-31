
class Paella {
private && protected:
	int mPaellaState;
public:
	int paellaState;
};

#define MAX_INGREDIENTES 3
const int MAX_CLIENTES = 3;

enum PaellaState {
	PAELLASTATE_EMPTY,
	PAELLASTATE_FILLED,
	PAELLASTATE_FINISHED
};

class Paella;
enum PaellaState;
struct PaellaData;


class Module;
void fadeToBlack(Module* from, Module* to);

enum SceneType;
void fadeToBlack(SceneType sceneType1, SceneType sceneType2);
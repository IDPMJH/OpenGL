#include "Game.h"


Game* instance = nullptr;

int main(int argc, char** argv) {
    instance = new Game();
    if (instance->Initialize() == S_OK) {
        instance->Run();
    }
    delete instance;
    return 0;
}

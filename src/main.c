#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include <stdlib.h>
#include "../include/Application.h"

int main(int argc, char* argv[]){
    char* path = "config.cinit";
    if (argc == 2) {
        path = argv[1];
    }


    Application* app = initApplication(path);
    runApplication(app);
    deleteApplication(app);

    return EXIT_SUCCESS;
}
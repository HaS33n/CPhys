#include <SFML/Audio.h>
#include <SFML/Graphics.h>

#include <stdlib.h>
#include "../include/Application.h"

int main(void){

    Application* app = initApplication("config.cinit");
    runApplication(app);
    deleteApplication(app);

    return EXIT_SUCCESS;
}
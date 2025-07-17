#include "../include/Application.h"
#include <stdlib.h>
#include <stdio.h>

Application* initApplication(const char* path){
	Application* app = malloc(sizeof(Application));
	if (app == NULL) {
		printf("Memory Fail\n");
		exit(EXIT_FAILURE);
	}

	/*
	* TODO: implement parsing from config file here
	*/

	const sfVideoMode mode = { 700, 700, 32 };
	app->window = sfRenderWindow_create(mode, "SFML window", sfClose, NULL); //add ability to resize later
	if (app->window == NULL) {

		printf("Failed to create Window\n");
		exit(EXIT_FAILURE);
	}
	sfRenderWindow_setVerticalSyncEnabled(app->window, 1);


	const sfVector2u bounds = { mode.width, mode.height };
	app->world = createWorld(3, 10.f, 1.f, 1.f, bounds);

	return app;
}

void runApplication(Application* app){
	sfRenderWindow* window = app->window;
	World* world = app->world;

	sfEvent event;
	sfClock* clock = sfClock_create();

	while (sfRenderWindow_isOpen(window)){

		while (sfRenderWindow_pollEvent(window, &event)){
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);
		}

		updateWorld(world,sfClock_restart(clock));

		sfRenderWindow_clear(window, sfBlack);
		drawWorld(window, world);
		sfRenderWindow_display(window);
	}

}

void deleteApplication(Application* app){
	sfRenderWindow_destroy(app->window);
	deleteWorld(app->world);
	free(app);
}

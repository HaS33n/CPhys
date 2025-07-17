#include "../include/Application.h"
#include <stdlib.h>
#include <stdio.h>
#include "../include//VectorUtils.h"

Application* initApplication(const char* path){
	Application* app = malloc(sizeof(Application));
	if (app == NULL) {
		perror("Memory Fail");
		exit(EXIT_FAILURE);
	}

	Config* config = parseInputFromPath(path);

	const sfVideoMode mode = { config->phys_area_size.x, config->phys_area_size.y, 32 };
	app->window = sfRenderWindow_create(mode, "CPhys", sfClose, NULL);
	if (app->window == NULL) {

		perror("Failed to create Window");
		exit(EXIT_FAILURE);
	}
	sfRenderWindow_setVerticalSyncEnabled(app->window, 1);

	app->world = createWorld(config);

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

//This function assumes that file has correct format of information
//if not, then program will probably segfualt -> I have a handler for that B)
Config* parseInputFromPath(const char* path){
	Config* cfg = malloc(sizeof(Config));
	if (cfg == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}


	FILE* cfg_file = fopen(path, "r");
	if (cfg_file == NULL) {
		perror("Failure when reading config file\n");
		exit(EXIT_FAILURE);
	}

	//todo FIX small buffer size
	char buffer[1000]; //static size buffer works fine with this read system. There is no valid data, that will be bigger than 100 bytes.

	//read first 8 env entries
	float floats[5];
	sfVector2f vectors[4];

	
	for (int i = 0; i < 5; i++) {//for floats
		fgets(buffer, 1000, cfg_file);
		clearLine(buffer);
		if (strlen(buffer) == 0) { //clear line or comment
			i--;
			continue;
		}
		floats[i] = strtof(buffer,NULL);
	}

	
	for (int i = 0; i < 4; i++) {//for vecs
		fgets(buffer, 1000, cfg_file);
		clearLine(buffer);
		if (strlen(buffer) == 0) { //clear line or comment
			i--;
			continue;
		}
		vectors[i] = strtov2f(buffer);
	}

	//read all defined bodies
	bodyDEF* defs = malloc(sizeof(bodyDEF) * (int)floats[0]);
	if (defs == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < (int)floats[0]; i++) {
		fgets(buffer, 1000, cfg_file);
		clearLine(buffer);
		if (strlen(buffer) == 0) { //clear line or comment
			i--;
			continue;
		}
		defs[i] = defineBodyFromStr(buffer);
	}
	
	cfg->num_defined_bodies = (int)floats[0];
	cfg->num_bodies = (int)floats[1];
	cfg->grav_accel = floats[2];
	cfg->pixels_per_meter = floats[3];
	cfg->collision_perfection_coef = floats[4];

	const sfVector2u v = { vectors[0].x, vectors[0].y };
	cfg->phys_area_size = v;
	cfg->mRange = vectors[1];
	cfg->vRange = vectors[2];
	cfg->rRange = vectors[3];

	cfg->definitions = defs;


	fclose(cfg_file);
	return cfg;
}

static void clearLine(char* line) {
	int size = strlen(line);

	for (int i = 0; i < size; i++) {
		if (line[i] == '#' || line[i] == '\n') {
			line[i] = '\0';
			return;
		}
	}
}

bodyDEF defineBodyFromStr(const char* str){
	sfVector2f pos;
	sfVector2f vel;
	float r, m;
	if (sscanf(str, "%f,%f,%f,%f,%f,%f", &pos.x, &pos.y, &r, &vel.x, &vel.y, &m) != 6) {
		perror("badly defined body");
		exit(EXIT_FAILURE);
	}
	bodyDEF ret = { m,r,vel,pos };

	return ret;
}


#include "../include/Application.h"
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

//-------------------------------------------------------------
//auxiliary funcs for parsing
static void parseFloatVal(const char* src, void* dest) {
	float* ptr = (float*)(dest);
	*ptr = strtof(src, NULL);
}

static void parseVecVal(const char* src, void* dest) {
	sfVector2f* ptr = (sfVector2f*)(dest);
	*ptr = strtov2f(src);
}

static void parseBdyVal(const char* src, void* dest) {
	bodyDEF* ptr = (bodyDEF*)(dest);
	*ptr = defineBodyFromStr(src);
}
//----------------------------------------------------------------

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

	//(optional) TODO FIX small buffer size problem with big comments
	char buffer[1000];
	float floats[5];
	sfVector2f vectors[4];

	readNLines(cfg_file, buffer, sizeof(buffer), floats, sizeof(float), 5, parseFloatVal);
	readNLines(cfg_file, buffer, sizeof(buffer), vectors, sizeof(sfVector2f), 4, parseVecVal);

	bodyDEF* defs = malloc(sizeof(bodyDEF) * (int)floats[0]);
	if (defs == NULL) {
		perror("Memory Fail\n");
		exit(EXIT_FAILURE);
	}
	readNLines(cfg_file, buffer, sizeof(buffer), defs, sizeof(bodyDEF), (int)floats[0], parseBdyVal);

	//TODO compile result in function
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

	if (!checkCFG(cfg)) {
		perror("input out of range");
		exit(EXIT_FAILURE);
	}

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

bool checkCFG(const Config* cfg){
	bool res = true;


	res &= (cfg->num_bodies >= LIMITS_ENTITIES.x && cfg->num_bodies <= LIMITS_ENTITIES.y);
	res &= (cfg->num_defined_bodies >= LIMITS_ENTITIES.x && cfg->num_defined_bodies <= LIMITS_ENTITIES.y);
	res &= (cfg->grav_accel >= LIMITS_GRAVITY.x && cfg->grav_accel <= LIMITS_GRAVITY.y);
	res &= (cfg->collision_perfection_coef >= LIMITS_COLCOEFF.x && cfg->collision_perfection_coef <= LIMITS_COLCOEFF.y);
	res &= (cfg->grav_accel >= LIMITS_GRAVITY.x && cfg->grav_accel <= LIMITS_GRAVITY.y);
	res &= (cfg->mRange.x >= LIMITS_MRANGE.x && cfg->mRange.y <= LIMITS_MRANGE.y);
	res &= (cfg->phys_area_size.x >= LIMITS_RESOLUTION.x && cfg->phys_area_size.y >= LIMITS_RESOLUTION.x);

	const unsigned int min = cfg->phys_area_size.x > cfg->phys_area_size.y ? cfg->phys_area_size.y : cfg->phys_area_size.x;
	res &= (cfg->pixels_per_meter >= LIMITS_PPM.x && cfg->pixels_per_meter <= min);
	res &= (cfg->vRange.x >= LIMITS_VRANGE.x && cfg->vRange.y <= min);
	res &= (cfg->rRange.x >= LIMITS_RRANGE.x && cfg->rRange.y <= min);


	const int n = cfg->num_defined_bodies;
	for (int i = 0; i < n; i++) {
		bodyDEF* def = cfg->definitions + i;
		res &= (def->mass >= LIMITS_MRANGE.x && def->mass <= LIMITS_MRANGE.y);
		res &= (def->velocity.x >= LIMITS_VRANGE.x && def->velocity.y <= min);
		res &= (def->radius >= LIMITS_RRANGE.x && def->radius <= min);
		res &= (def->position.x >= 0 && def->position.x <= cfg->phys_area_size.x && def->position.y >= 0 && def->position.y <= cfg->phys_area_size.y);
	}

	return res;
}

void readNLines(FILE* file, char* buffer, const int buffer_size, void* dest, size_t type_size, const int n_lines, void(*fptr)(const char*, void* dest)) {
	for (int i = 0; i < n_lines; i++) {
		fgets(buffer, buffer_size, file);
		clearLine(buffer);
		if (strlen(buffer) == 0) { //clear line or comment
			i--;
			continue;
		}

		//hack
		char* ptr = (char*)(dest);
		ptr += i * type_size;
		fptr(buffer, ptr);
	}
}


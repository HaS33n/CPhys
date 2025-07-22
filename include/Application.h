#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.h>
#include <stdbool.h>
#include <SFML/System.h>
#include "World.h"
#include <stdlib.h>
#include <stdio.h>

// -1 means that there is a special rule for it
// no need to use enums tho
#define LIMITS_ENTITIES (sfVector2i){0,20}
#define LIMITS_GRAVITY (sfVector2f){0,100}
#define LIMITS_PPM (sfVector2i){1,-1}
#define LIMITS_COLCOEFF (sfVector2f){0,1}
#define LIMITS_RESOLUTION (sfVector2i){100,-1}
#define LIMITS_MRANGE (sfVector2f){0.1,1000}
#define LIMITS_VRANGE (sfVector2f){0,-1}
#define LIMITS_RRANGE (sfVector2f){1,-1}

typedef struct {
	sfVertexArray* vertical;
	sfVertexArray* horizontal;
} Grid;

typedef struct{
	
	sfRenderWindow* window;
	World* world;
}Application;

Application* initApplication(const char* path);
void runApplication(Application* app);
void deleteApplication(Application* app);

static Config* parseInputFromPath(const char* path);
static bodyDEF defineBodyFromStr(const char* str);
static bool checkCFG(const Config* cfg); //TODO: add checks for place swapped values
static void readNLines(FILE* file, char* buffer, const int buffer_size, void* dest, size_t type_size, const int n_lines, void (*fptr)(const char*, void* dest));

Grid* createGrid(sfVector2u bounds);
void deleteGrid(Grid* grd);
void drawGrid(sfRenderWindow* target, const World* wrld, Grid* grd);

#endif

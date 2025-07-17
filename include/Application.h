#ifndef APPLICATION_H
#define APPLICATION_H

#include <SFML/Graphics.h>
#include <SFML/System.h>
#include "World.h"


typedef struct{
	
	sfRenderWindow* window;
	World* world;
}Application;

Application* initApplication(const char* path);
void runApplication(Application* app);
void deleteApplication(Application* app);

static Config* parseInputFromPath(const char* path);
static void clearLine(char* line);
static bodyDEF defineBodyFromStr(const char* str);
#endif

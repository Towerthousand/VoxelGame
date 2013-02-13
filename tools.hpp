#ifndef TOOLS_H
#define TOOLS_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <cmath>

extern int SCRWIDTH; //1366
extern int SCRHEIGHT; //768
#define WINDOW_TITLE "VoxelGame"
#define CONTEXT_SETTINGS_OPENGL sf::ContextSettings(32,32,0,2,0)
#define CHUNKWIDTH 16 //blocks in x and z coords
#define CHUNKHEIGHT 128 //blocks in y coords
#define WORLDSIZE 5 //worldsize in CHUNKSxCHUNKS
#define SEALEVEL 64

#endif

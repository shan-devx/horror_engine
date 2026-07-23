#ifndef GUI_H
#define GUI_H

struct Player;
struct EngineContext;
struct Asset;


void camera_init(EngineContext &e, Player &p);
void asset_init(EngineContext &e);
void viewport_init();
void viewport_menu(EngineContext &e, Player &p);
void assets_menu(EngineContext &e);
void world_menu(EngineContext &e);
void properties_menu(EngineContext &e);

#endif

#ifndef PERSIS_HPP
#define PERSIS_HPP

#include <vector>
#include <engine.hpp>

void save(std::vector<Asset> &world_obj);
std::vector<Asset> load();
void load_asset_model(std::vector<Asset> &world_obj);

#endif

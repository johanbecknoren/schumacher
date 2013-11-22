#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H
#include "material.h"
#include "utils.h"
#include <vector>

class MaterialManager {
public:
	MaterialManager();
	static Material* get(const material_t &m) { return MaterialManager::_materials.at(m);}
	static std::string asString(const Material &m);
private:
	static std::vector<Material*> _materials;
};

#endif

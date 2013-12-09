#include "materialmanager.h"

MaterialManager::MaterialManager() {
	if(MaterialManager::_materials.empty()) {
	MaterialManager::_materials.resize(int(NUM_MATERIALS));
	MaterialManager::_materials.at(GLASS) = new Material();
	MaterialManager::_materials.at(GLASS)->setMaterialType(GLASS);
	MaterialManager::_materials.at(GLASS)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(GLASS)->setRefractionIndex(1.52f);
	MaterialManager::_materials.at(GLASS)->setOpacity(0.05f);
	MaterialManager::_materials.at(GLASS)->setSpecular(1.0f);
	MaterialManager::_materials.at(GLASS)->setAbsorbtion(0.f);

	MaterialManager::_materials.at(MARBLE) = new Material();
	MaterialManager::_materials.at(MARBLE)->setMaterialType(MARBLE);
	MaterialManager::_materials.at(MARBLE)->setDiffuseColor(glm::vec3(1.0f, 1.0f,0.0f));
	MaterialManager::_materials.at(MARBLE)->setSpecular(0.8f);
	MaterialManager::_materials.at(MARBLE)->setAbsorbtion(0.75f);

	MaterialManager::_materials.at(LIGHT) = new Material();
	MaterialManager::_materials.at(LIGHT)->setMaterialType(LIGHT);
	MaterialManager::_materials.at(LIGHT)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(LIGHT)->setEmission(glm::vec3(6.0f));

	MaterialManager::_materials.at(MIRROR) = new Material();
	MaterialManager::_materials.at(MIRROR)->setMaterialType(MIRROR);
	MaterialManager::_materials.at(MIRROR)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(MIRROR)->setAbsorbtion(0.1f);
	MaterialManager::_materials.at(MIRROR)->setSpecular(1.0f);

	MaterialManager::_materials.at(STONE) = new Material();
	MaterialManager::_materials.at(STONE)->setMaterialType(STONE);
	MaterialManager::_materials.at(STONE)->setDiffuseColor(glm::vec3(0.8f));

	MaterialManager::_materials.at(CORNELL_CEIL) = new Material();
	MaterialManager::_materials.at(CORNELL_CEIL)->setMaterialType(CORNELL_CEIL);
	MaterialManager::_materials.at(CORNELL_CEIL)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(CORNELL_CEIL)->setAbsorbtion(1.f);

	MaterialManager::_materials.at(CORNELL_FLOOR) = new Material();
	MaterialManager::_materials.at(CORNELL_FLOOR)->setMaterialType(CORNELL_FLOOR);
	MaterialManager::_materials.at(CORNELL_FLOOR)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(CORNELL_FLOOR)->setAbsorbtion(1.f);

	MaterialManager::_materials.at(CORNELL_BACK) = new Material();
	MaterialManager::_materials.at(CORNELL_BACK)->setMaterialType(CORNELL_BACK);
	MaterialManager::_materials.at(CORNELL_BACK)->setDiffuseColor(glm::vec3(1.0f));
	MaterialManager::_materials.at(CORNELL_BACK)->setAbsorbtion(1.f);

	MaterialManager::_materials.at(CORNELL_RIGHT) = new Material();
	MaterialManager::_materials.at(CORNELL_RIGHT)->setMaterialType(CORNELL_RIGHT);
	MaterialManager::_materials.at(CORNELL_RIGHT)->setDiffuseColor(glm::vec3(192.f/255.f,216.f/255.f,144.f/255.f));
	MaterialManager::_materials.at(CORNELL_RIGHT)->setAbsorbtion(1.f);

	MaterialManager::_materials.at(CORNELL_LEFT) = new Material();
	MaterialManager::_materials.at(CORNELL_LEFT)->setMaterialType(CORNELL_LEFT);
	MaterialManager::_materials.at(CORNELL_LEFT)->setDiffuseColor(glm::vec3(194.f/255.f,63.f/255.f,56.f/255.f));
	MaterialManager::_materials.at(CORNELL_LEFT)->setAbsorbtion(1.f);

	MaterialManager::_materials.at(CORNELL_TALL_BOX) = new Material();
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setMaterialType(CORNELL_TALL_BOX);
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setDiffuseColor(glm::vec3(145.f/255.f,167.f/255.f,204.f/255.f));
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setAbsorbtion(1.f);
	}
}

std::string MaterialManager::asString(const Material &m) {
	switch(m.getMaterialType()) {
	case STONE:
		return "Stone";

	case GLASS:
		return "Glass";

	case MARBLE:
		return "Marble";

	case LIGHT:
		return "Light";

	case MIRROR:
		return "Mirror";

	case CORNELL_CEIL:
		return "CORNELL_CEIL";

	case CORNELL_BACK:
		return "CORNELL_BACK";

	case CORNELL_RIGHT:
		return "CORNELL_RIGHT";
		
	case CORNELL_LEFT:
		return "CORNELL_LEFT";

	case CORNELL_FLOOR:
		return "CORNELL_FLOOR";

	default:
		return "NoStr";
		
	}
}

std::vector<Material*> MaterialManager::_materials = std::vector<Material*>();

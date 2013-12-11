#include "materialmanager.h"

MaterialManager::MaterialManager() {
	if(MaterialManager::_materials.empty()) {
	MaterialManager::_materials.resize(int(NUM_MATERIALS));
	MaterialManager::_materials.at(GLASS) = new Material();
	MaterialManager::_materials.at(GLASS)->setMaterialType(GLASS);
	MaterialManager::_materials.at(GLASS)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(GLASS)->setRefractionIndex(real(1.52));
	MaterialManager::_materials.at(GLASS)->setOpacity(real(0.05));
	MaterialManager::_materials.at(GLASS)->setSpecular(real(1.0));
	MaterialManager::_materials.at(GLASS)->setAbsorbtion(real(0));

	MaterialManager::_materials.at(MARBLE) = new Material();
	MaterialManager::_materials.at(MARBLE)->setMaterialType(MARBLE);
	MaterialManager::_materials.at(MARBLE)->setDiffuseColor(Vec3(real(1.0), real(1.0),real(0.0)));
	MaterialManager::_materials.at(MARBLE)->setSpecular(real(1.0));
	MaterialManager::_materials.at(MARBLE)->setAbsorbtion(real(0.75));

	MaterialManager::_materials.at(LIGHT) = new Material();
	MaterialManager::_materials.at(LIGHT)->setMaterialType(LIGHT);
	MaterialManager::_materials.at(LIGHT)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(LIGHT)->setEmission(Vec3(real(6.0)));

	MaterialManager::_materials.at(MIRROR) = new Material();
	MaterialManager::_materials.at(MIRROR)->setMaterialType(MIRROR);
	MaterialManager::_materials.at(MIRROR)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(MIRROR)->setAbsorbtion(real(0.0));
	MaterialManager::_materials.at(MIRROR)->setSpecular(real(1.0));

	MaterialManager::_materials.at(STONE) = new Material();
	MaterialManager::_materials.at(STONE)->setMaterialType(STONE);
	MaterialManager::_materials.at(STONE)->setDiffuseColor(Vec3(real(0.8)));

	MaterialManager::_materials.at(CORNELL_CEIL) = new Material();
	MaterialManager::_materials.at(CORNELL_CEIL)->setMaterialType(CORNELL_CEIL);
	MaterialManager::_materials.at(CORNELL_CEIL)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(CORNELL_CEIL)->setAbsorbtion(real(1));

	MaterialManager::_materials.at(CORNELL_FLOOR) = new Material();
	MaterialManager::_materials.at(CORNELL_FLOOR)->setMaterialType(CORNELL_FLOOR);
	MaterialManager::_materials.at(CORNELL_FLOOR)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(CORNELL_FLOOR)->setAbsorbtion(real(1));

	MaterialManager::_materials.at(CORNELL_BACK) = new Material();
	MaterialManager::_materials.at(CORNELL_BACK)->setMaterialType(CORNELL_BACK);
	MaterialManager::_materials.at(CORNELL_BACK)->setDiffuseColor(Vec3(real(1.0)));
	MaterialManager::_materials.at(CORNELL_BACK)->setAbsorbtion(real(1));

	MaterialManager::_materials.at(CORNELL_RIGHT) = new Material();
	MaterialManager::_materials.at(CORNELL_RIGHT)->setMaterialType(CORNELL_RIGHT);
	MaterialManager::_materials.at(CORNELL_RIGHT)->setDiffuseColor(Vec3(real(192)/real(255),real(216)/real(255),real(144)/real(255)));
	MaterialManager::_materials.at(CORNELL_RIGHT)->setAbsorbtion(real(1));

	MaterialManager::_materials.at(CORNELL_LEFT) = new Material();
	MaterialManager::_materials.at(CORNELL_LEFT)->setMaterialType(CORNELL_LEFT);
	MaterialManager::_materials.at(CORNELL_LEFT)->setDiffuseColor(Vec3(real(194.)/real(255),real(63)/real(255),real(56)/real(255)));
	MaterialManager::_materials.at(CORNELL_LEFT)->setAbsorbtion(real(1));

	MaterialManager::_materials.at(CORNELL_TALL_BOX) = new Material();
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setMaterialType(CORNELL_TALL_BOX);
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setDiffuseColor(Vec3(real(145)/real(255),real(167)/real(255),real(204)/real(255)));
	MaterialManager::_materials.at(CORNELL_TALL_BOX)->setAbsorbtion(real(1));
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

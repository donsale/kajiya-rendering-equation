#pragma once
#include "brdf.h"

namespace kajiya {
class Material {
public:
	float refractive_index;
	enum material_type { glass, diffuse, mirror, light };
	material_type type;
	Brdf brdf;
};
} // namespace kajiya
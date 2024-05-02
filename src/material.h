#pragma once
#include "brdf.h"
#include "vec3.h"

namespace kajiya {
class Material {
public:
	float refractive_index;
	enum material_type { glass, diffuse, mirror, light };
	material_type type;
	Vec3 brdf;
	Vec3 emittance;
};
} // namespace kajiya
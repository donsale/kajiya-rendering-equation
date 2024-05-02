#pragma once
#include "brdf.h"

namespace kajiya {
class material {
public:
	float refractive_index;
	enum material_type { glass, diffuse, mirror };
	material_type type;
	Brdf brdf;
};
} // namespace kajiya
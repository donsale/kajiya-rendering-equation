#pragma once

namespace whitted {
class material {
public:
	float refractive_index;
	enum material_type { glass, diffuse, mirror };
	material_type type;
};
} // namespace whitted

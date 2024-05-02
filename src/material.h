#pragma once

namespace kajiya {
class material {
public:
	float refractive_index;
	enum material_type { glass, diffuse, mirror };
	material_type type;
};
} // namespace kajiya

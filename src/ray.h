#pragma once

#include "vec3.h"

namespace kajiya {
class ray {
public:
	vec3 origin, direction;
	float refractive_index;

	ray(vec3 o, vec3 d, float index = 1)
		: origin(o), direction(d), refractive_index(index){};

	vec3 value(float t) const {
		return origin + direction * t;
	}
};
} // namespace kajiya

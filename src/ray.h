#pragma once

#include "vec3.h"

namespace kajiya {
class Ray {
public:
	Vec3 origin, direction;
	float refractive_index;

	Ray(Vec3 o, Vec3 d, float index = 1)
		: origin(o), direction(d), refractive_index(index){};

	Vec3 value(float t) const {
		return origin + direction * t;
	}
};
} // namespace kajiya

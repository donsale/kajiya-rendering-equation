#pragma once

#include "material.h"
#include "ray.h"
#include "vec3.h"
#include <optional>

namespace kajiya {
class hittable {
public:
	virtual std::optional<vec3> intersect(ray &r) const = 0;
	virtual vec3 normal(vec3 &intersection_point) const = 0;
	virtual material mat() const						= 0;
	virtual color get_color(vec3 &point) const			= 0;
};

} // namespace kajiya
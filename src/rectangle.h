#include "color.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include <iostream>
#include <optional>
namespace whitted {
class triangle {
public:
	vec3 p1, p2, p3, n;

	triangle(vec3 p1, vec3 p2, vec3 p3, vec3 n) : p1(p1), p2(p2), p3(p3), n(n) {
	}

	std::optional<vec3> intersect(ray &r) const {
		// G = p1
		// n = n
		// A = r.origin
		// B = r.direction
		float ray_parallel_with_plane_indicator = vec3::dot(r.direction, n);

		// Ray is parallel
		if (ray_parallel_with_plane_indicator < 0.0005 &&
			ray_parallel_with_plane_indicator > -0.0005) {

			return std::nullopt;
		}

		float t =
			vec3::dot((p1 - r.origin), n) / ray_parallel_with_plane_indicator;

		if (t > 0) {
			vec3 intersection_point = r.value(t);

			vec3 c1 = vec3::cross(intersection_point - p1, p2 - p1);
			vec3 c2 = vec3::cross(intersection_point - p2, p3 - p2);
			vec3 c3 = vec3::cross(intersection_point - p3, p1 - p3);

			float d1 = vec3::dot(c1, n);
			float d2 = vec3::dot(c2, n);
			float d3 = vec3::dot(c3, n);

			if ((d1 < 0 && d2 < 0 && d3 < 0) || (d1 > 0 && d2 > 0 && d3 > 0))
				return intersection_point;
			else
				return std::nullopt;
		}

		return std::nullopt;
	}
};

class rectangle : public hittable {
public:
	vec3 p1, p2, p3, p4, n;
	color col;
	triangle t1, t2;
	material m;

	rectangle(vec3 p1, vec3 p2, vec3 p3, vec3 p4, vec3 n, color c)
		: p1(p1), p2(p2), p3(p3), p4(p4), n(n), col(c),
		  t1(triangle(p1, p2, p3, n)), t2(triangle(p2, p3, p4, n)) {
	}

	std::optional<vec3> intersect(ray &r) const {
		auto t1_result = t1.intersect(r);

		if (!t1_result.has_value()) {
			return t2.intersect(r);
		}

		return t1_result;
	}

	material mat() const {
		return m;
	}

	vec3 normal(vec3 &point) const {
		return n;
	}

	color get_color(vec3 &point) const {
		return col;
	}
};
} // namespace whitted

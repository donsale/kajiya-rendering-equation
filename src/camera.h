#pragma once

#include "util.h"
#include "vec3.h"
#include <cmath>

namespace kajiya {
class Camera {
public:
	Vec3 position;
	float screen_width;
	float screen_height;
	float focal_length;

	Vec3 x_axis;
	Vec3 y_axis;
	Vec3 z_axis;

	Camera(Vec3 p, float w, float h, float fl, Vec3 forward, Vec3 up)
		: position(p), screen_width(w), screen_height(h), focal_length(fl),
		  z_axis(forward.unit()), y_axis(up.unit()) {

		x_axis = Vec3::cross(y_axis, z_axis);
	}

	Vec3 focus() {
		return Vec3(position - z_axis * focal_length);
	}

	void zoom(float a) {
		focal_length *= a;
	}

	void rotate_x_deg(float a) {
		a			= pi / 180 * a;
		float cos_a = std::cos(a);
		float sin_a = std::sin(a);
		Vec3 new_y_axis(y_axis.x, cos_a * y_axis.y - sin_a * y_axis.z,
						sin_a * y_axis.y + cos_a * y_axis.z);
		Vec3 new_z_axis(z_axis.x, cos_a * z_axis.y - sin_a * z_axis.z,
						sin_a * z_axis.y + cos_a * z_axis.z);
		y_axis = new_y_axis;
		z_axis = new_z_axis;
	}

	void rotate_y_deg(float a) {
		a			= pi / 180 * a;
		float cos_a = std::cos(a);
		float sin_a = std::sin(a);
		Vec3 new_x_axis(cos_a * x_axis.x + sin_a * x_axis.z, x_axis.y,
						-sin_a * x_axis.x + cos_a * x_axis.z);
		Vec3 new_z_axis(cos_a * z_axis.x + sin_a * z_axis.z, z_axis.y,
						-sin_a * z_axis.x + cos_a * z_axis.z);
		x_axis = new_x_axis;
		z_axis = new_z_axis;
	}

	void rotate_z_deg(float a) {
		a			= pi / 180 * a;
		float cos_a = std::cos(a);
		float sin_a = std::sin(a);
		Vec3 new_x_axis(cos_a * x_axis.x - sin_a * x_axis.y,
						sin_a * x_axis.x + cos_a * x_axis.y, x_axis.z);
		Vec3 new_y_axis(cos_a * y_axis.x - sin_a * y_axis.y,
						sin_a * y_axis.x + cos_a * y_axis.y, y_axis.z);
		x_axis = new_x_axis;
		y_axis = new_y_axis;
	}
};
} // namespace kajiya

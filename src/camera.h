#pragma once

#include "vec3.h"

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
		  z_axis(forward.unit()), y_axis(up.unit()),
		  x_axis(Vec3::cross(y_axis, z_axis).unit()) {
	}

	Vec3 focus() {
		return Vec3(position - z_axis * focal_length);
	}
};
} // namespace kajiya

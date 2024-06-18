#pragma once

#include <map>

namespace kajiya {
class Brdf {
public:
	std::map<int, float> reflectance_spectrum;
};
} // namespace kajiya
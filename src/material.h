#pragma once
#include "spectrum.h"
#include "vec3.h"

namespace kajiya {
class Material {
public:
	enum material_type { white, red, green, light };
	material_type type;
	Spectrum reflectance;
	Spectrum emittance;

	static Material get_white() {
		Material white;
		white.type = Material::white;

		white.reflectance.spectrum[400 - 360] = 0.343;
		white.reflectance.spectrum[404 - 360] = 0.445;
		white.reflectance.spectrum[408 - 360] = 0.551;
		white.reflectance.spectrum[412 - 360] = 0.624;
		white.reflectance.spectrum[416 - 360] = 0.665;
		white.reflectance.spectrum[420 - 360] = 0.687;
		white.reflectance.spectrum[424 - 360] = 0.708;
		white.reflectance.spectrum[428 - 360] = 0.723;
		white.reflectance.spectrum[432 - 360] = 0.715;
		white.reflectance.spectrum[436 - 360] = 0.710;
		white.reflectance.spectrum[440 - 360] = 0.745;
		white.reflectance.spectrum[444 - 360] = 0.758;
		white.reflectance.spectrum[448 - 360] = 0.739;
		white.reflectance.spectrum[452 - 360] = 0.767;
		white.reflectance.spectrum[456 - 360] = 0.777;
		white.reflectance.spectrum[460 - 360] = 0.765;
		white.reflectance.spectrum[464 - 360] = 0.751;
		white.reflectance.spectrum[468 - 360] = 0.745;
		white.reflectance.spectrum[472 - 360] = 0.748;
		white.reflectance.spectrum[476 - 360] = 0.729;
		white.reflectance.spectrum[480 - 360] = 0.745;
		white.reflectance.spectrum[484 - 360] = 0.757;
		white.reflectance.spectrum[488 - 360] = 0.753;
		white.reflectance.spectrum[492 - 360] = 0.750;
		white.reflectance.spectrum[496 - 360] = 0.746;
		white.reflectance.spectrum[500 - 360] = 0.747;
		white.reflectance.spectrum[504 - 360] = 0.735;
		white.reflectance.spectrum[508 - 360] = 0.732;
		white.reflectance.spectrum[512 - 360] = 0.739;
		white.reflectance.spectrum[516 - 360] = 0.734;
		white.reflectance.spectrum[520 - 360] = 0.725;
		white.reflectance.spectrum[524 - 360] = 0.721;
		white.reflectance.spectrum[528 - 360] = 0.733;
		white.reflectance.spectrum[532 - 360] = 0.725;
		white.reflectance.spectrum[536 - 360] = 0.732;
		white.reflectance.spectrum[540 - 360] = 0.743;
		white.reflectance.spectrum[544 - 360] = 0.744;
		white.reflectance.spectrum[548 - 360] = 0.748;
		white.reflectance.spectrum[552 - 360] = 0.728;
		white.reflectance.spectrum[556 - 360] = 0.716;
		white.reflectance.spectrum[560 - 360] = 0.733;
		white.reflectance.spectrum[564 - 360] = 0.726;
		white.reflectance.spectrum[568 - 360] = 0.713;
		white.reflectance.spectrum[572 - 360] = 0.740;
		white.reflectance.spectrum[576 - 360] = 0.754;
		white.reflectance.spectrum[580 - 360] = 0.764;
		white.reflectance.spectrum[584 - 360] = 0.752;
		white.reflectance.spectrum[588 - 360] = 0.736;
		white.reflectance.spectrum[592 - 360] = 0.734;
		white.reflectance.spectrum[596 - 360] = 0.741;
		white.reflectance.spectrum[600 - 360] = 0.740;
		white.reflectance.spectrum[604 - 360] = 0.732;
		white.reflectance.spectrum[608 - 360] = 0.745;
		white.reflectance.spectrum[612 - 360] = 0.755;
		white.reflectance.spectrum[616 - 360] = 0.751;
		white.reflectance.spectrum[620 - 360] = 0.744;
		white.reflectance.spectrum[624 - 360] = 0.731;
		white.reflectance.spectrum[628 - 360] = 0.733;
		white.reflectance.spectrum[632 - 360] = 0.744;
		white.reflectance.spectrum[636 - 360] = 0.731;
		white.reflectance.spectrum[640 - 360] = 0.712;
		white.reflectance.spectrum[644 - 360] = 0.708;
		white.reflectance.spectrum[648 - 360] = 0.729;
		white.reflectance.spectrum[652 - 360] = 0.730;
		white.reflectance.spectrum[656 - 360] = 0.727;
		white.reflectance.spectrum[660 - 360] = 0.707;
		white.reflectance.spectrum[664 - 360] = 0.703;
		white.reflectance.spectrum[668 - 360] = 0.729;
		white.reflectance.spectrum[672 - 360] = 0.750;
		white.reflectance.spectrum[676 - 360] = 0.760;
		white.reflectance.spectrum[680 - 360] = 0.751;
		white.reflectance.spectrum[684 - 360] = 0.739;
		white.reflectance.spectrum[688 - 360] = 0.724;
		white.reflectance.spectrum[692 - 360] = 0.730;
		white.reflectance.spectrum[696 - 360] = 0.740;
		white.reflectance.spectrum[700 - 360] = 0.737;

		return white;
	}

	static Material get_red() {
		Material red;
		red.type = Material::red;

		red.reflectance.spectrum[400 - 360] = 0.040;
		red.reflectance.spectrum[404 - 360] = 0.046;
		red.reflectance.spectrum[408 - 360] = 0.048;
		red.reflectance.spectrum[412 - 360] = 0.053;
		red.reflectance.spectrum[416 - 360] = 0.049;
		red.reflectance.spectrum[420 - 360] = 0.050;
		red.reflectance.spectrum[424 - 360] = 0.053;
		red.reflectance.spectrum[428 - 360] = 0.055;
		red.reflectance.spectrum[432 - 360] = 0.057;
		red.reflectance.spectrum[436 - 360] = 0.056;
		red.reflectance.spectrum[440 - 360] = 0.059;
		red.reflectance.spectrum[444 - 360] = 0.057;
		red.reflectance.spectrum[448 - 360] = 0.061;
		red.reflectance.spectrum[452 - 360] = 0.061;
		red.reflectance.spectrum[456 - 360] = 0.060;
		red.reflectance.spectrum[460 - 360] = 0.062;
		red.reflectance.spectrum[464 - 360] = 0.062;
		red.reflectance.spectrum[468 - 360] = 0.062;
		red.reflectance.spectrum[472 - 360] = 0.061;
		red.reflectance.spectrum[476 - 360] = 0.062;
		red.reflectance.spectrum[480 - 360] = 0.060;
		red.reflectance.spectrum[484 - 360] = 0.059;
		red.reflectance.spectrum[488 - 360] = 0.057;
		red.reflectance.spectrum[492 - 360] = 0.058;
		red.reflectance.spectrum[496 - 360] = 0.058;
		red.reflectance.spectrum[500 - 360] = 0.058;
		red.reflectance.spectrum[504 - 360] = 0.056;
		red.reflectance.spectrum[508 - 360] = 0.055;
		red.reflectance.spectrum[512 - 360] = 0.056;
		red.reflectance.spectrum[516 - 360] = 0.059;
		red.reflectance.spectrum[520 - 360] = 0.057;
		red.reflectance.spectrum[524 - 360] = 0.055;
		red.reflectance.spectrum[528 - 360] = 0.059;
		red.reflectance.spectrum[532 - 360] = 0.059;
		red.reflectance.spectrum[536 - 360] = 0.058;
		red.reflectance.spectrum[540 - 360] = 0.059;
		red.reflectance.spectrum[544 - 360] = 0.061;
		red.reflectance.spectrum[548 - 360] = 0.061;
		red.reflectance.spectrum[552 - 360] = 0.063;
		red.reflectance.spectrum[556 - 360] = 0.063;
		red.reflectance.spectrum[560 - 360] = 0.067;
		red.reflectance.spectrum[564 - 360] = 0.068;
		red.reflectance.spectrum[568 - 360] = 0.072;
		red.reflectance.spectrum[572 - 360] = 0.080;
		red.reflectance.spectrum[576 - 360] = 0.090;
		red.reflectance.spectrum[580 - 360] = 0.099;
		red.reflectance.spectrum[584 - 360] = 0.124;
		red.reflectance.spectrum[588 - 360] = 0.154;
		red.reflectance.spectrum[592 - 360] = 0.192;
		red.reflectance.spectrum[596 - 360] = 0.255;
		red.reflectance.spectrum[600 - 360] = 0.287;
		red.reflectance.spectrum[604 - 360] = 0.349;
		red.reflectance.spectrum[608 - 360] = 0.402;
		red.reflectance.spectrum[612 - 360] = 0.443;
		red.reflectance.spectrum[616 - 360] = 0.487;
		red.reflectance.spectrum[620 - 360] = 0.513;
		red.reflectance.spectrum[624 - 360] = 0.558;
		red.reflectance.spectrum[628 - 360] = 0.584;
		red.reflectance.spectrum[632 - 360] = 0.620;
		red.reflectance.spectrum[636 - 360] = 0.606;
		red.reflectance.spectrum[640 - 360] = 0.609;
		red.reflectance.spectrum[644 - 360] = 0.651;
		red.reflectance.spectrum[648 - 360] = 0.612;
		red.reflectance.spectrum[652 - 360] = 0.610;
		red.reflectance.spectrum[656 - 360] = 0.650;
		red.reflectance.spectrum[660 - 360] = 0.638;
		red.reflectance.spectrum[664 - 360] = 0.627;
		red.reflectance.spectrum[668 - 360] = 0.620;
		red.reflectance.spectrum[672 - 360] = 0.630;
		red.reflectance.spectrum[676 - 360] = 0.628;
		red.reflectance.spectrum[680 - 360] = 0.642;
		red.reflectance.spectrum[684 - 360] = 0.639;
		red.reflectance.spectrum[688 - 360] = 0.657;
		red.reflectance.spectrum[692 - 360] = 0.639;
		red.reflectance.spectrum[696 - 360] = 0.635;
		red.reflectance.spectrum[700 - 360] = 0.642;
		return red;
	}

	static Material get_green() {
		Material green;
		green.type = Material::green;

		green.reflectance.spectrum[400 - 360] = 0.092;
		green.reflectance.spectrum[404 - 360] = 0.096;
		green.reflectance.spectrum[408 - 360] = 0.098;
		green.reflectance.spectrum[412 - 360] = 0.097;
		green.reflectance.spectrum[416 - 360] = 0.098;
		green.reflectance.spectrum[420 - 360] = 0.095;
		green.reflectance.spectrum[424 - 360] = 0.095;
		green.reflectance.spectrum[428 - 360] = 0.097;
		green.reflectance.spectrum[432 - 360] = 0.095;
		green.reflectance.spectrum[436 - 360] = 0.094;
		green.reflectance.spectrum[440 - 360] = 0.097;
		green.reflectance.spectrum[444 - 360] = 0.098;
		green.reflectance.spectrum[448 - 360] = 0.096;
		green.reflectance.spectrum[452 - 360] = 0.101;
		green.reflectance.spectrum[456 - 360] = 0.103;
		green.reflectance.spectrum[460 - 360] = 0.104;
		green.reflectance.spectrum[464 - 360] = 0.107;
		green.reflectance.spectrum[468 - 360] = 0.109;
		green.reflectance.spectrum[472 - 360] = 0.112;
		green.reflectance.spectrum[476 - 360] = 0.115;
		green.reflectance.spectrum[480 - 360] = 0.125;
		green.reflectance.spectrum[484 - 360] = 0.140;
		green.reflectance.spectrum[488 - 360] = 0.160;
		green.reflectance.spectrum[492 - 360] = 0.187;
		green.reflectance.spectrum[496 - 360] = 0.229;
		green.reflectance.spectrum[500 - 360] = 0.285;
		green.reflectance.spectrum[504 - 360] = 0.343;
		green.reflectance.spectrum[508 - 360] = 0.390;
		green.reflectance.spectrum[512 - 360] = 0.435;
		green.reflectance.spectrum[516 - 360] = 0.464;
		green.reflectance.spectrum[520 - 360] = 0.472;
		green.reflectance.spectrum[524 - 360] = 0.476;
		green.reflectance.spectrum[528 - 360] = 0.481;
		green.reflectance.spectrum[532 - 360] = 0.462;
		green.reflectance.spectrum[536 - 360] = 0.447;
		green.reflectance.spectrum[540 - 360] = 0.441;
		green.reflectance.spectrum[544 - 360] = 0.426;
		green.reflectance.spectrum[548 - 360] = 0.406;
		green.reflectance.spectrum[552 - 360] = 0.373;
		green.reflectance.spectrum[556 - 360] = 0.347;
		green.reflectance.spectrum[560 - 360] = 0.337;
		green.reflectance.spectrum[564 - 360] = 0.314;
		green.reflectance.spectrum[568 - 360] = 0.285;
		green.reflectance.spectrum[572 - 360] = 0.277;
		green.reflectance.spectrum[576 - 360] = 0.266;
		green.reflectance.spectrum[580 - 360] = 0.250;
		green.reflectance.spectrum[584 - 360] = 0.230;
		green.reflectance.spectrum[588 - 360] = 0.207;
		green.reflectance.spectrum[592 - 360] = 0.186;
		green.reflectance.spectrum[596 - 360] = 0.171;
		green.reflectance.spectrum[600 - 360] = 0.160;
		green.reflectance.spectrum[604 - 360] = 0.148;
		green.reflectance.spectrum[608 - 360] = 0.141;
		green.reflectance.spectrum[612 - 360] = 0.136;
		green.reflectance.spectrum[616 - 360] = 0.130;
		green.reflectance.spectrum[620 - 360] = 0.126;
		green.reflectance.spectrum[624 - 360] = 0.123;
		green.reflectance.spectrum[628 - 360] = 0.121;
		green.reflectance.spectrum[632 - 360] = 0.122;
		green.reflectance.spectrum[636 - 360] = 0.119;
		green.reflectance.spectrum[640 - 360] = 0.114;
		green.reflectance.spectrum[644 - 360] = 0.115;
		green.reflectance.spectrum[648 - 360] = 0.117;
		green.reflectance.spectrum[652 - 360] = 0.117;
		green.reflectance.spectrum[656 - 360] = 0.118;
		green.reflectance.spectrum[660 - 360] = 0.120;
		green.reflectance.spectrum[664 - 360] = 0.122;
		green.reflectance.spectrum[668 - 360] = 0.128;
		green.reflectance.spectrum[672 - 360] = 0.132;
		green.reflectance.spectrum[676 - 360] = 0.139;
		green.reflectance.spectrum[680 - 360] = 0.144;
		green.reflectance.spectrum[684 - 360] = 0.146;
		green.reflectance.spectrum[688 - 360] = 0.150;
		green.reflectance.spectrum[692 - 360] = 0.152;
		green.reflectance.spectrum[696 - 360] = 0.157;
		green.reflectance.spectrum[700 - 360] = 0.159;
		return green;
	}

	static Material get_light() {
		Material light;
		light.type = Material::light;

		// Linear interpolation lambda function
		auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };

		for (int i = 400; i <= 500; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(0, 8.0, (i - 400.f) / (100.f));
		}

		for (int i = 500; i <= 600; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(8.0, 15.6, (i - 500.f) / 100.f);
		}

		for (int i = 600; i <= 700; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(15.6, 18.4, (i - 600.f) / 100.f);
		}

		for (int i = 700; i <= 830; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(18.4, 18.4, (i - 700.f) / 130.f);
		}

		// for (int i = 0; i < Spectrum::number_of_frequenices; i++) {
		// 	printf("%2.6f\n", light.emittance.spectrum[i]);
		// }

		return light;
	}

	static Material get_flashlight() {
		Material light;
		light.type = Material::light;

		// Linear interpolation lambda function
		auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };

		light.emittance.spectrum[360 - 360] = 6;
		light.emittance.spectrum[408 - 360] = 4;
		light.emittance.spectrum[470 - 360] = 100;
		light.emittance.spectrum[500 - 360] = 83;
		light.emittance.spectrum[700 - 360] = 65;

		for(int i = 360; i <= 408; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(6, 4, (i - 360.f) / (48.f));
		}

		for(int i = 408; i <= 460; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(4, 100, (i - 408.f) / (52.f));
		}
		
		for (int i = 470; i <= 500; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(100, 83, (i - 470.f) / 30.f);
		}
		
		for (int i = 500; i <= 700; i++) {
			light.emittance.spectrum[i - 360] =
				lerp(83, 65, (i - 500.f) / 200.f);
		}

		// speculation beyond 700
		//for (int i = 700; i <= 830; i++) {
		//	light.emittance.spectrum[i - 360] =
		//		lerp(65, 60, (i - 700.f) / 130.f);
		//}
		
		light.emittance.normalize();

		return light;
	}
};
} // namespace kajiya

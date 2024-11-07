#include "../include/pch.h"
#include "../include/color.h"
#include "../include/color32.h"

namespace shir0GL {
#ifdef SOGL_EXPORT
	color color::BLACK = color(0, 0, 0);
	color color::RED = color(1, 0, 0);
	color color::YELLOW = color(1, 1, 0);
	color color::GREEN = color(0, 1, 0);
	color color::CYAN = color(0, 1, 1);
	color color::BLUE = color(0, 0, 1);
	color color::MAGENTA = color(1, 0, 1);
	color color::WHITE = color(1, 1, 1);
#endif

	color::color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	color color::lerp(color const& a, color const& b, float const& t) {
		return a + (b - a) * t;
	}
	color color::saturate(color const& a) {
		color c(a);
		c.saturate();
		return c;
	}

	void color::saturate() {
		r = std::max(0.0f, std::min(r, 1.0f));
		g = std::max(0.0f, std::min(g, 1.0f));
		b = std::max(0.0f, std::min(b, 1.0f));
		a = std::max(0.0f, std::min(a, 1.0f));
	}

	color operator+(color const& a, color const& b) {
		return color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
	}
	color color::operator+=(color const& b) {
		*this = *this + b;
		return *this;
	}

	color operator-(color const& a, color const& b) {
		return color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
	}
	color color::operator-=(color const& b) {
		*this = *this - b;
		return *this;
	}

	color operator*(color const& a, float const& b) {
		color c = a;
		c.r *= b;
		c.g *= b;
		c.b *= b;
		c.a *= b;

		return c;
	}
	color color::operator*=(float const& b) {
		*this = *this * b;
		return *this;
	}

	color::operator color32() {
		color32 c;
		c.r = r * 255;
		c.g = g * 255;
		c.b = b * 255;
		c.a = a * 255;
		c.saturate();

		return c;
	}
}
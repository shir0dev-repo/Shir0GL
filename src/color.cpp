#include <algorithm>

#include "color.h"
#include "color32.h"


namespace sogl {
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

	color::color(const color& c) {
		*this = c;
	}

	color& color::operator=(const color& c) {
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;

		return *this;
	}

	color color::lerp(const color& a, const color& b, const float& t) {
		color c = b - a;
		c = c + (b - a);
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

	color color::operator+(color const& b) const {
		color c = *this;
		c.r += b.r;
		c.g += b.g;
		c.b += b.b;
		c.a += b.a;

		return c;
	}
	color& color::operator+=(color const& b) {
		this->r += b.r;
		this->g += b.g;
		this->b += b.b;
		this->a += b.a;

		return *this;
	}

	color color::operator-(const color& b) const {
		color c = *this;
		c.r -= b.r;
		c.g -= b.g;
		c.b -= b.b;
		c.a -= b.a;

		return c;
	}

	color& color::operator-=(const color& b) {
		this->r -= b.r;
		this->g -= b.g;
		this->b -= b.b;
		this->a -= b.a;

		return *this;
	}

	color color::operator*(float const& scalar) const {
		color c = *this;
		c.r *= scalar;
		c.g *= scalar;
		c.b *= scalar;
		c.a *= scalar;

		return c;
	}

	color& color::operator*=(float const& scalar) {
		this->r *= scalar;
		this->g *= scalar;
		this->b *= scalar;
		this->a *= scalar;

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
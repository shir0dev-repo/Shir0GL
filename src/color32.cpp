#include <algorithm>

#include <sogl/color>

namespace sogl {
#ifdef SOGL_EXPORT
	color32 color32::BLACK = color32(0, 0, 0);
	color32 color32::RED = color32(255, 0, 0);
	color32 color32::YELLOW = color32(255, 255, 0);
	color32 color32::GREEN = color32(0, 255, 0);
	color32 color32::CYAN = color32(0, 255, 255);
	color32 color32::BLUE = color32(0, 0, 255);
	color32 color32::MAGENTA = color32(255, 0, 255);
	color32 color32::WHITE = color32(255, 255, 255);
#endif

	color32::color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	color32 color32::lerp(color32 const& a, color32 const& b, float const& t) {
		return a + (b - a) * t;
	}

	color32 color32::saturate(color32 const& a) {
		color32 c;
		c.r = std::max(0, std::min((int)a.r, 255));
		c.g = std::max(0, std::min((int)a.g, 255));
		c.b = std::max(0, std::min((int)a.b, 255));
		c.a = std::max(0, std::min((int)a.a, 255));

		return c;
	}

	void color32::saturate() {
		r = std::max(0, std::min((int)r, 255));
		g = std::max(0, std::min((int)g, 255));
		b = std::max(0, std::min((int)b, 255));
		a = std::max(0, std::min((int)a, 255));
	}

	color32 color32::operator+(const color32& b) const {
		color32 c = *this;
		c.r += b.r;
		c.g += b.g;
		c.b += b.b;
		c.a += b.a;

		return c;
	}
	color32& color32::operator+=(const color32& c) {
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}

	color32 color32::operator-(const color32& b) const {
		color32 c = *this;
		c.r -= b.r;
		c.g -= b.g;
		c.b -= b.b;
		c.a -= b.a;

		return c;
	}
	color32& color32::operator-=(const color32& c) {
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;

		return *this;
	}

	color32 color32::operator*(const float& scalar) const {
		color32 c = a;
		c.r *= scalar;
		c.g *= scalar;
		c.b *= scalar;
		c.a *= scalar;

		return c;
	}
	color32& color32::operator*=(const float& scalar) {
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;
		return *this;
	}

	color32::operator color() {
		return color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
}
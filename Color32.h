#pragma once

namespace shir0GL {
	typedef unsigned char uint8_t;
	struct color;

	struct color32 {
		static color32 BLACK;
		static color32 RED;
		static color32 YELLOW;
		static color32 GREEN;
		static color32 CYAN;
		static color32 BLUE;
		static color32 MAGENTA;
		static color32 WHITE;

		uint8_t r, g, b, a;

		inline color32(uint8_t = 0, uint8_t = 0, uint8_t = 0, uint8_t = 255);

		static color32 lerp(color32 const&, color32 const&, float const&);

		static color32 saturate(color32 const&);
		void saturate();

		friend color32 operator+(color32 const& a, color32 const& b);
		color32 operator+=(color32 const& b);

		friend color32 operator-(color32 const& a, color32 const& b);
		color32 operator-=(color32 const& b);

		friend color32 operator*(color32 const& a, float const& b);
		color32 operator*=(float const& b);

		operator color();

	private:
		static color32 min;
		static color32 max;
	};

	color32 color32::BLACK = color32(0, 0, 0);
	color32 color32::RED = color32(255, 0, 0);
	color32 color32::YELLOW = color32(255, 255, 0);
	color32 color32::GREEN = color32(0, 255, 0);
	color32 color32::CYAN = color32(0, 255, 255);
	color32 color32::BLUE = color32(0, 0, 255);
	color32 color32::MAGENTA = color32(255, 0, 255);
	color32 color32::WHITE = color32(255, 255, 255);

	color32 color32::min = color32(0, 0, 0, 0);
	color32 color32::max = color32(255, 255, 255, 255);
}
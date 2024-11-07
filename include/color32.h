#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace shir0GL {
	typedef unsigned char uint8_t;
	struct color;

	struct SOGL_API color32 {
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
}
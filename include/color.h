#pragma once

#ifdef SOGL_EXPORT
#define SOGL_API __declspec(dllexport)
#else
#define SOGL_API __declspec(dllimport)
#endif

namespace shir0GL {
	struct color32;

	struct SOGL_API color {
		static color BLACK;
		static color RED;
		static color YELLOW;
		static color GREEN;
		static color CYAN;
		static color BLUE;
		static color MAGENTA;
		static color WHITE;

		float r, g, b, a;

		color(float = 0.0f, float = 0.0f, float = 0.0f, float = 1.0f);

		static color lerp(color const&, color const&, float const&);

		static color saturate(color const&);
		void saturate();

		friend color operator+(color const& a, color const& b);
		color operator+=(color const& b);

		friend color operator-(color const& a, color const& b);
		color operator-=(color const& b);

		friend color operator*(color const& a, float const& b);
		color operator*=(float const& b);

		operator color32();
	};


	
}
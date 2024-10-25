#pragma once

namespace shir0GL {
	struct color32;

	struct color {
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


	color color::BLACK = color(0, 0, 0);
	color color::RED = color(1, 0, 0);
	color color::YELLOW = color(1, 1, 0);
	color color::GREEN = color(0, 1, 0);
	color color::CYAN = color(0, 1, 1);
	color color::BLUE = color(0, 0, 1);
	color color::MAGENTA = color(1, 0, 1);
	color color::WHITE = color(1, 1, 1);
}
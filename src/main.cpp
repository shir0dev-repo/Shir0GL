#include <vec3f.inl>
#include <quat.inl>
#include <matrix4f.inl>
using namespace sogl;

int main() {
	vec3f v(0, 25, 0);
	quat q(vec3f(90, 0, 0));
	matrix4f m;
	m.setRotation(q);
	vec3f v1 = (q * v);
	vec3f v2 = (m * v);

	return 0;
}
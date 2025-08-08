#pragma once

namespace sogl {
	template <typename T1, typename T2>
	struct pair {
		T1 first;
		T2 second;

		inline pair(T1 first, T2 second) {
			this->first = first;
			this->second = second;
		}
	};
}
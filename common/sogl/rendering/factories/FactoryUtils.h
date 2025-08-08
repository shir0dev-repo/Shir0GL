#pragma once

namespace sogl {
	struct FactoryUtils {
		static void CreateAlias(const char* alias, const char* defaultAlias, uint32_t appendedSize, char*& resolvedAlias) {
			if (strcmp(alias, "") == 0) {
				char* aliasUsed = new char[16];
				snprintf(aliasUsed, 16, "%s %lu\0", defaultAlias, appendedSize);
				resolvedAlias = aliasUsed;
			}
			else {
				resolvedAlias = const_cast<char*>(alias);
			}
		}
	};
}
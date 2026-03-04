#include "utils.h"

string new_string(char *begin, char *end) {
	return (string){.begin=begin, .end=end};
}

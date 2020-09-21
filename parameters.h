#ifndef __PARAMETERS_H
#define __PARAMETERS_H
#include "datastring.h"
#include <cstdint>

// This is a parser for the parameters that come from a client's command.
class parameters
{
	public:
	parameters();
	static const int MAX_PARAMETERS = 10;
	datastring string_parameters[MAX_PARAMETERS];
	int64_t long_parameters[MAX_PARAMETERS];
	int parameter_count;
	datastring get_string_parameter(datastring &input,bool &success);
	int64_t get_long_parameter(datastring &input,bool &success);
	void string_parameter(datastring &input,bool &success);
	void long_parameter(datastring &input,bool &success);
};

#endif

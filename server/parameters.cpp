#include "parameters.h"

parameters::parameters()
{
	int loop;
	for(loop=0;loop<parameters::MAX_PARAMETERS;loop++) {
		long_parameters[loop] = 0;
	}
	parameter_count = 0;
}
int64_t parameters::get_long_parameter(datastring &input,bool &success)
{
	int64_t output = 0;
	char ch;
	while (input.length > 0) {
		ch = input.data[0];
		input.remove_bytes_from_front(1);
		if ((ch >='0') && (ch <= '9')) {
			output = output * 10 + ch - '0';
			continue;
		}
		if (ch == ',') {
			break;
		}
		success = false;
		break;
	}
	return output;
}
datastring parameters::get_string_parameter(datastring &input,bool &success)
{
	int64_t length;
	datastring output;
	
	// Get the length. The string looks like 4,test6,number
	length = get_long_parameter(input,success);
	if (length > input.length) {
		success = false;
	}
	if (success) {
		output = input.substr(0,(int)length);
		input.remove_bytes_from_front((int)length);
	}
	return output;
}
void parameters::string_parameter(datastring &input,bool &success)
{
	if (success) {
		string_parameters[parameter_count++] = get_string_parameter(input,success);	
	}
	return;
}
void parameters::long_parameter(datastring &input,bool &success)
{
	if (success) {
		long_parameters[parameter_count++] = get_long_parameter(input,success);
	}
	return;
}

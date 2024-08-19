#include "client_checker.h"

int	check_char(std::string str)
{
	for (u_int16_t i = 0; i < str.length(); i++)
	{
		if (str[i] >= 'A' && str[i] <= '}')
			i++;
		else 
			return 1;
	}
	return 0;
}

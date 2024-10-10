#include "../hanghim.hpp"

HangHim::HangHim(std::string type, std::vector<std::string> params) : _type(type) , _players(params)
{
	_chanName = _type + generateChanId();
	std::cout << this << std::endl;
	std::cout << _chanName << std::endl;
	std::cout << "HangHim game created" << std::endl;
}

HangHim::~HangHim(){}

// HangHim::HangHim(const HangHim &myHangHim)
// {
// }

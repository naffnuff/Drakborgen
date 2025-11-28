#pragma once

#define STRING(x) #x
#define TO_STRING(x) STRING(x)
#define THROW throw std::runtime_error(__FILE__ ":" TO_STRING(__LINE__))

class System
{
};


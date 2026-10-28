#pragma once

#include <iostream>
#include <cstdlib>

#ifdef NDEBUG
#define RUXON_ASSERT(x) do { (void)sizeof(x); } while(0)
#define RUXON_ASSERT_MSG(x, msg) do { (void)sizeof(x); (void)sizeof(msg); } while(0)
#else
#define RUXON_ASSERT(x) do { if(!(x)) { std::cerr << "Assertion failed: " #x << " at " << __FILE__ << ":" << __LINE__ << std::endl; std::abort(); } } while(0)
#define RUXON_ASSERT_MSG(x, msg) do { if(!(x)) { std::cerr << "Assertion failed: " #x << " (" << msg << ") at " << __FILE__ << ":" << __LINE__ << std::endl; std::abort(); } } while(0)
#endif

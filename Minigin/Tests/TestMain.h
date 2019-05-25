#pragma once

#include <iostream>
#include "../catch.hpp"

#include "TestBasicComponents.h"
#include "TestInputHandling.h"

namespace Catch {
	std::ostream& cout() {
		return std::cerr;
	}
	std::ostream& clog() {
		return std::cerr;
    }
    std::ostream& cerr() {
		return std::cerr;
    }
}

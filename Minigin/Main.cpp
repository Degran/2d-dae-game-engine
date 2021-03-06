#include "MiniginPCH.h"
#pragma comment(lib,"xinput.lib")

#define CATCH_CONFIG_NOSTDOUT
#define CATCH_CONFIG_RUNNER

// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#include "SDL.h"
#include "Minigin.h"
#include <iostream>
#include "catch.hpp"
#include "Tests/TestMain.h"

#pragma warning( push )  
#pragma warning( disable : 4100 )  
int main(int argc, char* argv[]) {
#pragma warning( pop )
	Catch::Session().run(argc, argv);
	std::cout << "Something meaningless" << std::endl;
	std::cerr << "Something error-y" << std::endl;

	dae::Minigin engine;
	engine.Run();
    return 0;
}


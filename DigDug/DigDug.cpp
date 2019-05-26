// DigDug.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <windows.h>
#include "targetver.h"
#include <iostream>

#include "FygarFactory.h"
#include "Game.h"

int main()
{
// 	std::cerr << "Something error-y" << std::endl;
	kmo::Game digDug;
	digdug::FygarFactory factory{ digDug };
	factory.CreateFygar();
	digDug.Loop();
}

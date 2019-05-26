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
    std::cout << "Hello World!\n"; 
	kmo::Game digDug;
	digdug::FygarFactory factory{ digDug.m_sceneManager };
}

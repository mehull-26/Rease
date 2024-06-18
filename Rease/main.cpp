// Read documentation/Readme for more details on each function

#include <Windows.h>
#include <iostream>
#include "Rease.h"

using namespace Rez;
using namespace D2D1;
int main()
{
	// Creates and shows Window with 4 parameters
	WindowCreator("MAIN", 800, 800);

	// Changes the background color ( DEFAULT : BLACK )
	SetBGcolor(Color(0, 0, 0, 1));

		while (Running) {
			// Handles the Events such as quit event and other keyboard inputs etc
			EventHandler();

			// If WM_QUIT message received, exit the loop and application
			if (Event.message == WM_QUIT) {
				Running = false;
			}

			// Checks if the corresponding key is down, uses ASCII values and other keynames same as in Win32API (see MSDN)
			if (IsKeyDown('D'))
			{
				std::cout << "Pressed D\n";
			}

			// Additional Input functions, read documentation for more details on implementation

			/*if (MouseButtonDown("Left"))
			{
				std::cout << ClickCoord.x << "L" << ClickCoord.y << "\n";
			}
			if (MouseButtonDown("right"))
			{
				std::cout << ClickCoord.x << "R" << ClickCoord.y << "\n";
			}
			if (MouseButtonDown("Middle"))
			{
				std::cout << ClickCoord.x << "M" << ClickCoord.y << "\n";
			}*/
			
			//std::cout << CurrentMouseCoord.x<<" "<<CurrentMouseCoord.y<<"\n";

			// Renders all the shapes and images that have been drawn till yet on the window's client area
			Render();

		}

		// Call it once yoir application is done to destroy the resources and free the allocated memory
		Destroy();
}
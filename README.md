# Rease
Rease or RenderEase is a graphic library based on DIRECT2D and Wind32 API. IT offers you various methods and some class structures to help you enable render/draw various shapes. There are few things to see before working on any Rease project - 

**i) Goto project properties -> Linker -> System -> Subsystem.** Check that it is "Console (/SUBSYSTEM:CONSOLE)". Although it is bydefault that only. But incase you would want to use the " Windows (/SUBSYSTEM:WINDOWS)" subsystem , you would have to change int main() to int WinMain(.....) with a bunch of parameters. You can go through that on the MSDN documentation.
> https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain

**ii) Goto project properties -> Linker -> Input -> Additional dependencies.** And add "windowscodecs.lib","d2d1.lib" to them. They might come in handy while using the direct2D components or your project might throw errors.

iii) Also to make sure, you do not try to access Rease class methods/elements until you have enough supervision to understand them. And all the user code is meant to be written in ***main.cpp.***

Let us go through each of the methods a bit more deeply so you can implement them on ease.

### 1) Window Creation
Rease provies you a high level easy to use method for creation and showing of window. It takes up 4 parameters i.e 
> 	void WindowCreator ( string title, int width, int height, DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME );

a) Title - It is of string type and gives the title to the default window.

b) Width & Height - As their name suggests, they initliaze the window with the entered width and height.

c) DWORD style - It is WIN32 API based paramter. Although it gives some deafult values, but incase you desire some other styles you can pass on it as a parameter of your choice. And for multiple styles, seprate them with a **" | "**. Check MSDN documentation on [DWORD style](https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles) for more information.

### 2) Creating Shapes and other Graphic functions 

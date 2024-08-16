# Rease
Rease or RenderEase is a graphic library based on DIRECT2D and Win32 API. IT offers you various methods and some class structures to help you enable render/draw various shapes. There are few things to see before working on any Rease project - 

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

#### a) CreateEllipse
Provides a method to draw an ellipse shape on screen.

>	void CreateEllipse(Vector2 Center, float radiusX, float radiusY, Color color, bool fill, float stroke = 1);

a) **Center**: Specifies the center coordinates of the ellipse.

b) **radiusX & radiusY**: Define the horizontal and vertical radii of the ellipse.

c) **Color**: Determines the color of the ellipse.

d) **fill**: If true, fills the ellipse; if false, outlines it.

e) **stroke**: Optional parameter for the outline thickness (default is 1).

#### b) CreatePolygon
Draws a polygon shape with a specified number of sides and side length.

>	void CreatePolygon(Vector2 TopLeftPt, int sides, int sidelength, Color color, float stroke = 1.0f);

a) **TopLeftPt**: Position of the top-left corner or starting point of the polygon.

b) **sides**: Number of sides or vertices of the polygon.

c) **sidelength**: Length of each side of the polygon.

d) **Color**: Color of the polygon.

e) **stroke**: Optional parameter for the outline thickness (default is 1).

#### c) CreateLine
Draws a straight line between two given points.

>	void CreateLine(Vector2 pt1, Vector2 pt2, Color color, float stroke = 1.0f);

a) **pt1 & pt2**: Endpoints of the line segment.

b) **Color**: Color of the line.

c) **stroke**: Optional parameter for the line thickness (default is 1).

#### d) CreateGeometry
Draws a custom geometry defined by an array of points.

>	void CreateGeometry(int ElementCount, D2D1_POINT_2F PointArr[], Color color, bool fill, float stroke = 1.0f);

a) **ElementCount**: Number of points in the `PointArr` array.

b) **PointArr**: Array of `D2D1_POINT_2F` points defining the geometry. Just use the specified data type, for more details on it, [click here](https://learn.microsoft.com/en-us/windows/win32/direct2d/d2d1-point-2f).

c) **Color**: Color of the geometry.

d) **fill**: If true, fills the geometry; if false, outlines it.

e) **stroke**: Optional parameter for the outline thickness (default is 1).

#### e) AddImage
Adds an image at a specified position with opacity and scale parameters.

>	void AddImage(Vector2 topLeftPoint, std::string Path, float opacity, float scale);

a) **topLeftPoint**: Position of the top-left corner where the image will be placed.

b) **Path**: File path or identifier of the image file.

c) **opacity**: Opacity level of the image (0.0 to 1.0, where 0.0 is fully transparent and 1.0 is fully opaque).

d) **scale**: Scale factor for resizing the image.

### 3) Input Handling

#### a) IsKeyDown
Checks if a specific key is currently pressed and returns a bool.

>	bool IsKeyDown(unsigned int key);

a) **key**: Unsigned integer representing the key identifier.

#### b) MouseButtonDown
Checks if a mouse button is currently pressed and returns a bool.

>	bool MouseButtonDown(std::string button);

a) **button**: String specifying the mouse button ("left", "right", etc.).

#### c) ClickCoord
Stores the coordinates of the last mouse click

#### d) CurrentMouseCoord
Stores the current coordinates of the mouse pointer.


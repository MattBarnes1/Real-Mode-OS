/////////////////////
//Window Manager
////////////////////

struct GUIObject
{
	short borderwidth;
	int x;
	int y;
	int width;
	int height;
	struct GUIObject *Children;
	unsigned short layer;
	struct GUIObject *Parent;
};

struct Button {
	struct GUIObject *MyGUI;
	char (*isMouseOver)(int, int);
	void (*OnMouseClick)(void);

};



struct Window {
	short BorderThickness;
	struct GUIObject *MyGUI;
	char draggable;
	void(*OnDraw)(void);
	char (*OnClick)(int, int);
	void(*OnLoad)(void);
};

struct LayerDrawer
{
	struct Window *MyWindow;
	struct LayerDrawer *NextWindow;
	struct LayerDrawer *PrevWindow;
};


struct LayerDrawer *LayerPtr;





/////////////////
//Desktop Load-in
/////////////////


void StartWindowManager()
{
	LayerPtr = (struct LayerDrawer *)malloc(sizeof(struct Window));
	LayerPtr->MyWindow = CreateWindow(0, 0, 1000, 44, 0);
}

void DrawWindows()
{
	struct LayerDrawer *Iterator = LayerPtr;
	do
	{		
		DrawWindow(Iterator->MyWindow);	
		Iterator = Iterator->NextWindow;	
	}	
	while(Iterator != 0);
}

struct Window *CreateWindow(int x, int y, int width, int height, unsigned short layer)
{
	struct GUIObject *myObject = (struct GUIObject *)malloc(sizeof(struct GUIObject));
	struct Window *ReturnWindow = (struct Window *)malloc(sizeof(struct Window));
	ReturnWindow->MyGUI = myObject;
	myObject->width = width;
	myObject->y = y;
	myObject->x = x;
	myObject->height = height;
	myObject->layer = layer;
	return ReturnWindow;
}

void DestroyWindow(struct Window *myWindow)
{
	if (myWindow == LayerPtr->MyWindow && myWindow != 0)
	{
		free(myWindow->MyGUI);
		free(myWindow);
	}
}

void DrawWindow(struct Window *myWindow)
{
	printString("\nDrawing X:\0", 0);
	writeInt(myWindow->MyGUI->x, 0);
	printString("\nDrawing Y:\0", 0); 
	writeInt(myWindow->MyGUI->y, 0);
	printString("\nDrawing Width:\0", 0);
	writeInt(myWindow->MyGUI->width, 0);
	printString("\nDrawing Heigth:\0", 0);
	writeInt(myWindow->MyGUI->height, 0);

	DrawSquare(0xff, myWindow->MyGUI->x, myWindow->MyGUI->y, myWindow->MyGUI->width, myWindow->MyGUI->height);
	if (myWindow->BorderThickness != 0)
	{
		DrawSquare(0xe, myWindow->MyGUI->x, myWindow->MyGUI->y, myWindow->BorderThickness, myWindow->MyGUI->height);
		DrawSquare(0xe, myWindow->MyGUI->x, myWindow->MyGUI->y, myWindow->MyGUI->width, myWindow->BorderThickness);
		DrawSquare(0xe, myWindow->MyGUI->x, myWindow->MyGUI->y + myWindow->MyGUI->height - myWindow->BorderThickness, myWindow->MyGUI->width, myWindow->BorderThickness);
		DrawSquare(0xe, myWindow->MyGUI->x + myWindow->MyGUI->width - myWindow->BorderThickness, myWindow->MyGUI->y, myWindow->BorderThickness, myWindow->MyGUI->height);
	}

}

#include <windows.h>
#include <fstream>
#include <CommDlg.h>
#include <vector>
#include "Point.h"
#include "Line.h"

#include <gl/gl.h>
#include <gl/glut.h>

using namespace std;


vector<Point> points;
vector<Line> lines;

#define POINT_MODE 1
#define LINE_MODE  2

int DRAWING_MODE;
bool isDrawing;

int width = 600;
int height = 600;
/**
Draw all points, which we clicked on the screen.
*/
void drawPoints()
{
	glColor3f(0.0,0.0,0.0);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	for(unsigned i = 0; i < points.size(); i ++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();
}
/**
Draw all lines, which we drawn with mouse.
*/
void drawLines()
{
	for(unsigned i = 0; i < lines.size(); i ++)
	{
		glBegin(GL_LINES);
			glColor3f(0.0,0.0,0.0);
			glLineWidth(5.0);
			glVertex2f(lines[i].start.x, lines[i].start.y);
			glVertex2f(lines[i].end.x, lines[i].end.y);
		glEnd();
	}
}
/**
Open file dialog, returns empty string if user cancels the operation.
*/
string openFile()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	HWND hwnd;              // owner window
	HANDLE hf;              // file handle
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
	ofn.lpstrFilter = "Text\0*.t\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&ofn)==TRUE);
		//hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
		//0, (LPSECURITY_ATTRIBUTES) NULL,
		//OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		//(HANDLE) NULL);
	return ofn.lpstrFile;
}
/**
Sets the logical coordinate system we will use to specify
our drawings.
*/
void SetTransformations() {
	//set up the logical coordinate system of the window: [-100, 100] x [-100, 100]
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}
/**
Handles the paint event. This event is triggered whenever
our displayed graphics are lost or out-of-date.
ALL rendering code should be written here.
*/
void OnDisplay() {
	//set the background color to white
	glClearColor(1, 1, 1, 1);
	//fill the whole color buffer with the clear color
	glClear(GL_COLOR_BUFFER_BIT);
	SetTransformations();

	drawPoints();
	drawLines();
	//force previously issued OpenGL commands to begin
	//execution
	glFlush();
}
/**
Handles the mouse press event. This event is triggered whenever
we click with mouse.
Button: indicates which mouse button was clicked left, right or middle
State:  indicates wether the mouse button is up or down.
X & Y:  are the mouse coordinates, they are sometimes incorrect
when the window is scaled.
(This can be fixed by updating the gluOrtho2D to match the window size.)
*/
void processMouse(int button, int state, int x, int y)
{
	float wx = x;
	float wy = height - y;
	//cout<<wx<<" "<<wy<<endl;
	if(button==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(DRAWING_MODE == POINT_MODE)
		{
			points.push_back(Point(wx,wy));
		}
		if(DRAWING_MODE == LINE_MODE)
		{
			if(isDrawing == false)
			{
				lines.push_back(Line(Point(wx,wy),Point(wx,wy)));
				isDrawing = true;
			}
		}
		OnDisplay();
	}
	if(button==GLUT_LEFT_BUTTON && state == GLUT_UP && DRAWING_MODE==LINE_MODE&&isDrawing==true)
		isDrawing = false;
}
/**
Handles the mouse move event. This event is triggered whenever we move with
the mouse while we are pressing at any of its buttons.
X & Y: Mouse coordinates
*/
void processMouseMove(int x, int y)
{
	float wx = x;
	float wy = height - y;
	if(isDrawing)
	{
		lines[lines.size()-1].end = Point(wx,wy);
		OnDisplay();
	}
}
/**
Handles the keyboard press event. This event is triggered whenever any keyboard
key is pressed.
key: the key pressed
X & Y: mouse location in window relative coordinates when the key was pressed.
*/
void processKeyboard(unsigned char key, int x, int y)
{
	if(key == ' ')
	{
		points.clear();
		lines.clear();
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
	}
}
/**
Handles resizing of the window. This event is triggered whenever the
window is resized such as by user interaction.
*/
void reshape(int newWidth, int newHeight)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, newWidth, 0, newHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, newWidth, newHeight);
	width = newWidth, height = newHeight;
}
void selectDrawingMode(int choice)
{
	if(choice==0)
		DRAWING_MODE = POINT_MODE;
	if(choice==1)
		DRAWING_MODE = LINE_MODE;
}
//check
void openFileType(int choice)
{
	MessageBox(NULL,"Please make sure from file format before choosing it","Warning",MB_OK);
	string fileName;
	fileName = openFile();
	ifstream input(fileName);
	if(!input)
	{
		MessageBox(NULL,"No File Selected","MSG",MB_OK);
		//return;
	}
	
	if(choice == 0)//Points
	{
		Point temp;
		int entries = 0;
		while(!input.eof())
		{
			input>>temp.x>>temp.y;
			if(/*NOT OK*/0)
			{
				//MessageBox(NULL,"File Finshed or Error Detected","MSG",MB_OK);
				return;
			}
			entries++;
			points.push_back(temp);
		}
		glFlush();
	}
	
}
void mainMenu(int choice){}
void runAlgorithms(int choice)
{
}
void initMenus()
{
	int drawingSubmenu, openFileSubmenu, algorithmsSubmenu;
	
	drawingSubmenu = glutCreateMenu(selectDrawingMode);
	glutAddMenuEntry("Point", 0);
	glutAddMenuEntry("Line", 1);

	openFileSubmenu = glutCreateMenu(openFileType);
	glutAddMenuEntry("Points",0);
	glutAddMenuEntry("Lines", 1);

	convexHullSubmenu = glutCreateMenu(algorithmsSubmenu);


	glutCreateMenu(mainMenu);
	glutAddSubMenu("Drawing Mode", drawingSubmenu);
	glutAddSubMenu("Convex Hull", convexHullSubmenu);
	//glutAddSubMenu("Import",openFileSubmenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
/**
Creates the main window, registers event handlers, and
initializes OpenGL stuff.
*/
void InitGraphics(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	//Create an 600x600 window with its top-left corner at pixel (100, 100)
	glutInitWindowPosition(100, 100); //pass (-1, -1) for Window-Manager defaults
	glutInitWindowSize(width, height);
	glutCreateWindow("Geometry Package");
	//OnDisplay will handle the paint event
	glutReshapeFunc(reshape);
	glutDisplayFunc(OnDisplay);
	glutMouseFunc(processMouse);
	glutMotionFunc(processMouseMove);
	glutKeyboardFunc(processKeyboard);
	initMenus();
	//initially drawing mode is points
	DRAWING_MODE = POINT_MODE;
	isDrawing = false;
	glutMainLoop();
}

int main(int argc, char* argv[]) {
	InitGraphics(argc, argv);
	return 0;
}
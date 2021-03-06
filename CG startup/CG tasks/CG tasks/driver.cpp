#include <windows.h>
#include <CommDlg.h>
#include <stdlib.h>//for malloc
#include <stdarg.h>

#include "AlgorithmFactory.h"

#include <gl/gl.h>
#include <gl/glut.h>

static vector<Point> inPoints, outPoints;
static vector<Line>  inLines , outLines;

#define POINT_MODE 1
#define LINE_MODE  2

#define POINT_SIZE 3

#define DEFAULT_POINT_COLOR_R 0.0
#define DEFAULT_POINT_COLOR_G 0.0
#define DEFAULT_POINT_COLOR_B 0.0

#define DEFAULT_LINE_COLOR_R 0.0
#define DEFAULT_LINE_COLOR_G 0.0
#define DEFAULT_LINE_COLOR_B 0.0

#define OUT_POINT_COLOR_R 1.0
#define OUT_POINT_COLOR_G 0.0
#define OUT_POINT_COLOR_B 0.0

#define OUT_LINE_COLOR_R 0.0
#define OUT_LINE_COLOR_G 1.0
#define OUT_LINE_COLOR_B 0.0


int DRAWING_MODE;
bool isDrawing;

int width = 600;
int height = 600;

int Point::drawID = 1;
int Line::drawID  = 1;

void writePoint(const Point& p)
{
	//Not to be drawn point
	if(!p.drawID)return;
	char str[20];
	glRasterPos2f(p.x+POINT_SIZE, p.y+POINT_SIZE);
	sprintf(str,"%d", p.pointDrawID);
	for(unsigned i = 0; i < strlen(str); i ++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}

void drawPoints(const vector<Point>& points, float r, float g, float b)
{
	//glClearColor(1,1,1,1);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	glColor3f(r,g,b);
	glPointSize(POINT_SIZE);
	glBegin(GL_POINTS);
	for(unsigned i = 0; i < points.size(); i ++)
		glVertex2f(points[i].x, points[i].y);
	glEnd();

	for(unsigned i = 0; i < points.size(); i ++)
	{
		glColor3f(r, g, b);
		writePoint(points[i]);
	}
	glFlush();
}

void drawLines(const vector<Line>& lines, float r, float g, float b)
{
	for(unsigned i = 0; i < lines.size(); i ++)
	{
		glBegin(GL_LINES);
		glColor3f(r,g,b);
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

	drawPoints(inPoints,DEFAULT_POINT_COLOR_R,DEFAULT_POINT_COLOR_G,DEFAULT_POINT_COLOR_B);
	drawLines(inLines,DEFAULT_LINE_COLOR_R,DEFAULT_LINE_COLOR_G,DEFAULT_LINE_COLOR_B);
	//force previously issued OpenGL commands to begin execution
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
			inPoints.push_back(Point(wx,wy,1));
		}
		if(DRAWING_MODE == LINE_MODE)
		{
			if(isDrawing == false)
			{
				inLines.push_back(Line(Point(wx,wy),Point(wx,wy)));
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
		inLines[inLines.size()-1].end = Point(wx,wy);
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
		inPoints.clear();
		inLines.clear();
		outPoints.clear();
		outLines.clear();
		glClearColor(1,1,1,1);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		Point::drawID=0;
	}
	if(key == 27)//Escape
	{
		OnDisplay();
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
			inPoints.push_back(temp);
		}
		glFlush();
	}

}
void mainMenu(int choice){}
void runAlgorithms(int choice)
{
	Algorithm* algorithm = AlgorithmFactory::createAlgorithm(algorithmsNames[choice]);
	algorithm->run(inPoints,inLines,outPoints,outLines);
	//MessageBox(NULL,"DONE ya kbeer :D","Warning",MB_OK);
	drawPoints(outPoints,OUT_POINT_COLOR_R,OUT_POINT_COLOR_G,OUT_POINT_COLOR_B);
	drawLines(outLines,OUT_LINE_COLOR_R,OUT_LINE_COLOR_G,OUT_LINE_COLOR_B);
	glFlush();
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

	algorithmsSubmenu = glutCreateMenu(runAlgorithms);

	for(unsigned i = 0; i < ALGORITHMS_NUM; i ++)
		glutAddMenuEntry(algorithmsNames[i].c_str(),i);

	glutCreateMenu(mainMenu);
	glutAddSubMenu("Drawing Mode", drawingSubmenu);
	glutAddSubMenu("Run Algorithm", algorithmsSubmenu);
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
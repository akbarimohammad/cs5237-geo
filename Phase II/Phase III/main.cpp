
#define WIN32

#include "math.h"
#include <iostream>
#include <fstream>

#include <cstdio>
#include <fstream>
#include <strstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <queue>


#include "Circle.h"

#include "DenaulayTri.h"
#include "GL\glut.h"

using namespace std;

const double PI = 3.1415926536;
const double sqrt3 = 1.732050808F;

// Initial size of graphics window on your screen.
const int WIDTH  = 700; // in pixels
const int HEIGHT = 700; //

// Current size of window (will change when you resize window)
int width  = WIDTH;
int height = HEIGHT;

// Bounds of viewing frustum, maintain uniform scaling.
double viewWindowLeft =  -500;
double viewWindowRight  = 500;
double viewWindowBottom =  -350;
double viewWindowTop  = 350;

// increment for  zoom
const double ZoomSTEP = 100;
const double zoomFactor = 1.03;

const double PanSTEP = 10;


bool file_loaded = false;


//these structure store all points and triangles
/*Trist trist;

PointSetArray notinsidepsa;*/
int progress=1;
int last_pIndex;
DenaulayTri DenaulayTriangulation;

//Trist worksetTrist;
PointSetArray newInsertedPsa;

int circumcir1 = -1, circumcir2 = -1, circumcir3 = -1, circumcir4 = -1;
OrTri illegalOriTri= -1;

OrTri tempOriTri1= -1,tempOriTri2= -1,tempOriTri3 = -1;

bool showAnimatiion = true;

int longDelay = 1200;
int shortDelay = longDelay / 2;


void drawAPoint(double x,double y, float red = 0, float green = 0, float blue = 0, float opaque = 0)
{
	
	glPointSize(5);
	glBegin(GL_POINTS);
	//glColor3f(0,0,0);
	glColor4f(red,green,blue, opaque);
	glVertex2d(x,y);
	glEnd();
	glPointSize(1);
}

void drawALine(double x1,double y1, double x2, double y2,  float red = 0, float green = 0, float blue = 1, float opaque = 0)
{
		glPointSize(1);
		glBegin(GL_LINE_LOOP);
		//glColor3f(0,0,1);
		glColor4f(red,green,blue, opaque);
			glVertex2d(x1,y1);
			glVertex2d(x2,y2);
		glEnd();
		glPointSize(1);
}

void drawATriangle(double x1,double y1, double x2, double y2, double x3, double y3, float red = 0, float green = 0.5, float blue = 0, float opaque = 0)
{
		glBegin(GL_POLYGON);
		//glColor3f(0,0.5,0);
		glColor4f(red,green,blue, opaque);
			glVertex2d(x1,y1);
			glVertex2d(x2,y2);
			glVertex2d(x3,y3);
		glEnd();

}

void glCircle3i(GLint x, GLint y, GLint radius) { 
    
	float angle; 
	glBegin(GL_LINE_LOOP);
	glColor3f(1.0,0.5,0);
	for(int i = 0; i < 100; i++) { 
        angle = i*2*PI/100; 
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius)); 
    } 

	glEnd();

   
}  

void drawCircle(double x1,double y1, double x2, double y2, double x3, double y3)
{
	Circle circ(x1,y1, x2, y2, x3, y3);
	glCircle3i(circ.m_Center_x, circ.m_Center_y, circ.m_dRadius);

	
}

void drawCircumscribeCircleForTriangle(OrTri t)
{

	if ( t == -1) return;

		int tmppi1,tmppi2, tmppi3;
		DenaulayTriangulation.trist.getVertexIdx(t,tmppi1,tmppi2,tmppi3);
		LongInt x1,y1,x2,y2,x3,y3;
		
		if((tmppi1 > 0)  && (tmppi2> 0) && (tmppi3>0))
		{
			DenaulayTriangulation.psa.getPoint(tmppi1,x1,y1);
			DenaulayTriangulation.psa.getPoint(tmppi2,x2,y2);
			DenaulayTriangulation.psa.getPoint(tmppi3,x3,y3);
			
			drawCircle(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue());
		}

}

void drawOriTri(OrTri t){
		
	if ( t == -1) return;

		int tmppi1,tmppi2, tmppi3;
		DenaulayTriangulation.trist.getVertexIdx(t,tmppi1,tmppi2,tmppi3);
		LongInt x1,y1,x2,y2,x3,y3;
		DenaulayTriangulation.psa.getPoint(tmppi1,x1,y1);
		DenaulayTriangulation.psa.getPoint(tmppi2,x2,y2);
		DenaulayTriangulation.psa.getPoint(tmppi3,x3,y3);
		drawATriangle(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue(), 0.5,0,0);
		//drawCircle(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue());
}

void drawTrist(){
	//contain all drawing code.
	//get data from global variables trist & psa
	/**/


	for(int i= 0; i<DenaulayTriangulation.trist.noTri(); i++)
	{
		int pi1,pi2, pi3;
		
		bool tri = DenaulayTriangulation.trist.getVertexIdx(i * 8, pi1, pi2, pi3);
		bool leaf = DenaulayTriangulation.trist.isLeaf(i * 8);
		//????????????????
		if(tri && (pi1 > 0) && (pi2 > 0) && (pi3 > 0) ){
			LongInt x1, y1, x2, y2, x3, y3;
		
			DenaulayTriangulation.psa.getPoint(pi1,x1,y1);
			DenaulayTriangulation.psa.getPoint(pi2,x2,y2);
			DenaulayTriangulation.psa.getPoint(pi3,x3,y3);

			drawATriangle(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue());
		
			////////////////////////////////////////////
			drawALine(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue());
			drawALine(x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue());
			drawALine(x1.doubleValue(),y1.doubleValue(), x3.doubleValue(),y3.doubleValue());
		
			//DEBUG STUFF
			//drawCircle(x1.doubleValue(),y1.doubleValue(), x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue());

		}
		

		//DEBUG STUFF
		//cout<<"triangle at->"<<x1.doubleValue()<< " "<< y1.doubleValue()<< " "<< x2.doubleValue() << " " <<y2.doubleValue() << " " <<x3.doubleValue()<< " "<<y3.doubleValue()<< endl;
	}

	//for(int i= 1; i<=DenaulayTriangulation.psa.noPt(); i++)
	for(int i= 1; i< progress; i++)
	{
		LongInt x,y;
		DenaulayTriangulation.psa.getPoint(i,x,y);
		double tx = x.doubleValue();
		double ty = y.doubleValue();
		drawAPoint(tx,ty, 0, 0, 0, 0.6);
		
		//just for debugging purpose
		//cout<<"point at->"<<tx<<"," <<ty<<endl;
	}

	for(int i= 1; i<=newInsertedPsa.noPt(); i++)
	{
		LongInt x,y;
		newInsertedPsa.getPoint(i,x,y);
		double tx = x.doubleValue();
		double ty = y.doubleValue();
		drawAPoint(tx,ty, 0.6, 0, 0, 0.7);

		//just for debugging purpose
		//cout<<"point at->"<<tx<<"," <<ty<<endl;
	}

	drawCircumscribeCircleForTriangle(circumcir1);
	drawCircumscribeCircleForTriangle(circumcir2);
	drawCircumscribeCircleForTriangle(circumcir3);
	drawCircumscribeCircleForTriangle(circumcir4);

	if(illegalOriTri > -1)	//draw illegal edge
	{
		//OrTri neighbour = DenaulayTriangulation.trist.enext(illegalOriTri) ;
		int pi1,pi2, pi3;
		bool temp = DenaulayTriangulation.trist.getVertexIdx(illegalOriTri, pi1, pi2, pi3);

		if((pi1 > 0) && (pi2 > 0) && (pi3 > 0))
		{
		
			LongInt x1, y1, x2, y2, x3, y3;
		
			DenaulayTriangulation.psa.getPoint(pi1,x1,y1);
			DenaulayTriangulation.psa.getPoint(pi2,x2,y2);
			DenaulayTriangulation.psa.getPoint(pi3,x3,y3);
			drawALine(x2.doubleValue(),y2.doubleValue(), x3.doubleValue(),y3.doubleValue(),0.7,0.0,0.0);
		}


		//drawOriTri(illegalOriTri);
	}

	//glutSwapBuffers();
	
	
	
}

void setView()
{

	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D( viewWindowLeft, viewWindowRight, viewWindowBottom, viewWindowTop);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

void display(void)
{

	setView();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();


	// draw your output here (erase the following 3 lines)
	drawTrist();
	//drawTristDeluany();

	glPopMatrix();
	glutSwapBuffers ();
}

void reshape (int w, int h)
{
	width = w;
	height = h;
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);
	
	/*glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,w,h,0);  
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

}

void pan(unsigned char direction)
{
   // for zoom in reduces viewWindow size and for zoom out increases it.

	if (direction == 's') {	//move left
			viewWindowLeft -= PanSTEP; viewWindowRight -= PanSTEP;
						
		}
		else if (direction == 'd') {
			viewWindowLeft += PanSTEP; viewWindowRight += PanSTEP;
						
		}
		else if (direction == 'x') {
			viewWindowBottom -= PanSTEP; viewWindowTop -= ZoomSTEP;
			
		}
		else if (direction == 'e') {
			viewWindowBottom += PanSTEP; viewWindowTop += PanSTEP;
			
		}

}

void zoom(unsigned char direction)
{
   // for zoom in reduces viewWindow size and for zoom out increases it.

	

	if (direction == '+') {
		//if(viewWindowRight- viewWindowLeft <  3* ZoomSTEP) 	return;
			viewWindowLeft += ZoomSTEP; viewWindowRight -= ZoomSTEP;
			viewWindowBottom += ZoomSTEP; viewWindowTop -= ZoomSTEP;
			
		}
		else if (direction == '-') {
			viewWindowLeft -= ZoomSTEP; viewWindowRight += ZoomSTEP;
			viewWindowBottom -= ZoomSTEP; viewWindowTop += ZoomSTEP;
			
		}

}

void resetView()
{
	viewWindowLeft =  -500;
	viewWindowRight  = 500;
	viewWindowBottom =  -350;
	viewWindowTop  = 350;

}

void GetOGLPos(int x, int y, int& wx, int& wy, int& wz)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
 
	wx = posX;
	wy = posY;
	wz = posZ;
    return;
}

void init(void)
{
	glClearColor (1.0,1.0,1.0, 1.0);
	glShadeModel(GL_FLAT);
}

void legalizeEdge2(OrTri tri)
{
	if(! DenaulayTriangulation.checkLegal(tri))
	{
		OrTri tri1, tri2;
		
		illegalOriTri = tri;
		display();
		Sleep(longDelay);

		DenaulayTriangulation.flipEdge(tri,tri1, tri2);


		display();
		Sleep(shortDelay);
		illegalOriTri = -1;

		circumcir1 = tri1;
		circumcir2 = tri2;
		circumcir3 = -1;
		circumcir4 = -1;
		display();
		Sleep(shortDelay);



		legalizeEdge2(tri1);
		legalizeEdge2(tri2);
	}

}

void insertPoint(int pIndex) {
		
	OrTri tri;
	bool boundary;
	
	tri= DenaulayTriangulation.findPoint(pIndex,boundary);

	OrTri tri1, tri2, tri3,tri4;

	// If boundary ==false  (not on the boundry)
	// else delete 2 triangles and create 4 triangles

	if(boundary){
		DenaulayTriangulation.insertPoint(pIndex,tri,tri1,tri2,tri3, tri4);

		circumcir1 = tri1;
		circumcir2 = tri2;
		circumcir3 = tri3;
		circumcir4 = tri4;
		display();
		Sleep(longDelay);

		DenaulayTriangulation.legalizeEdge(tri1);
		DenaulayTriangulation.legalizeEdge(tri2);
		DenaulayTriangulation.legalizeEdge(tri3);
		DenaulayTriangulation.legalizeEdge(tri4);
		

	}
	else{
		DenaulayTriangulation.insertPoint(pIndex,tri,tri1,tri2,tri3);

		circumcir1 = tri1;
		circumcir2 = tri2;
		circumcir3 = tri3;
		display();
		Sleep(longDelay);

		legalizeEdge2(tri1);
		legalizeEdge2(tri2);
		legalizeEdge2(tri3);


		//DenaulayTriangulation.legalizeEdge(tri1);
		//DenaulayTriangulation.legalizeEdge(tri2);
		//DenaulayTriangulation.legalizeEdge(tri3);

		
	}

	

}

void delaunayComputation()
{
	
	SYSTEMTIME st;

	cerr << endl;
	cerr << "delaunay Triangulation By Incremental method:  " << endl;

	GetLocalTime(&st);
	int start = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;
	cerr << "Start: " << start << endl;

	for(; progress <= last_pIndex ; progress++)
	{
		
		insertPoint(progress);
		display();
		//Sleep(delay);

	}

	circumcir1 = -1;
	circumcir2 = -1;
	circumcir3 = -1;
	circumcir4 = -1;
	illegalOriTri = -1;

	GetLocalTime(&st);
	int end = (((st.wHour*60+st.wMinute)*60)+st.wSecond)*1000+st.wMilliseconds;
	cerr << "End: " << end << endl;
	cerr << "Elapsed Time(ms): " << end-start << endl;


	
}





void readFile(){

	string line_noStr;

	string line;   // each line of the file
	string command;// the command of each line
	string numberStr; // for single LongInt operation
	
	int delay=-1;
	ifstream inputFile("input.txt",ios::in);


	if(inputFile.fail()){
		cerr << "Error: Cannot read input file \"" << "input.txt" << "\"";
		return;
	}

	if(file_loaded)
	{
		//remove all points and triangles;
		DenaulayTriangulation.psa.eraseAllPoints();
		DenaulayTriangulation.trist.delAllTri();
		progress=1;
	}

	

	while(inputFile.good()){

		getline(inputFile,line);
		if(line.empty()) {
			command = "";
			continue; 
		}// in case the line has nothing in it

		stringstream linestream(line);

		linestream >> line_noStr;
		linestream >> command;         // get the command

		
		if(!command.compare("IP")){
			linestream >> numberStr;
			LongInt x(numberStr);
			linestream >> numberStr;
			LongInt y(numberStr);
			last_pIndex=DenaulayTriangulation.psa.addPoint(x,y);
				
			}	//if(!command.compare("IP")){
		else if (!command.compare("CD")){
			delaunayComputation();
			display();
		}		 //else if (!command.compare("CD")){
		else{
			cerr << "Exception: Wrong input command" << endl;
		}

		
	}

}

void writeFile()
{
	//write to  ��input.txt�� get data from global variables trist & psa
	double instCount = 0; // Instruction count
	ofstream outputFile("input.txt",ios::out); // output file

	//outputFile<<instCount<<": DY 2"<<endl;
	instCount++;

	// obtain individual point data from psa and writing the AP command
	for(int i= 1; i<=DenaulayTriangulation.psa.noPt(); i++)
	{
		LongInt x,y;
		DenaulayTriangulation.psa.getPoint(i,x,y);
		double dx = x.doubleValue();
		double dy = y.doubleValue();
		outputFile<<std::setw(4) << std::setfill('0') << instCount << ": IP " << dx << " " << dy << endl;
		instCount++;
	}

	outputFile<<std::setw(4) << std::setfill('0') << instCount << ": CD " << endl;


	outputFile.close();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
		case 'r':
		case 'R':
			readFile();
			file_loaded = true;
		break;

		case 'w':
		case 'W':
			writeFile();
		break;

		case 'Q':
		case 'q':
			exit(0);
		break;

		case 'O':
		case 'o':
			zoom('-');
			//glutPostRedisplay();
			break;

		case 'I':
		case 'i':
			zoom('+');
			//glutPostRedisplay();
			break;

		case 'S':
		case 's':
			pan('s');
			//glutPostRedisplay();
			break;
		case 'D':
		case 'd':
			pan('d');
			//glutPostRedisplay();
			break;
		case 'E':
		case 'e':
			pan('e');
			//glutPostRedisplay();
			break;
		case 'X':
		case 'x':
			pan('x');
			//glutPostRedisplay();
			break;
		case 'N':
		case 'n':
			resetView();
			break;

		case 'C':
		case 'c':

			int start = DenaulayTriangulation.psa.noPt();
			LongInt x,y;
			for (int i = 1; i<= newInsertedPsa.noPt(); i++)
			{
				newInsertedPsa.getPoint(i,x,y);
				last_pIndex = DenaulayTriangulation.psa.addPoint(x,y);
				insertPoint(last_pIndex);
			}
			newInsertedPsa.eraseAllPoints();
			break;
			
		


		//default:
		//break;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)//the point and triangles have to be inserted to the trist & psa
{
	/*button: GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON */
        
	/*state: GLUT_UP or GLUT_DOWN */
	enum
	{
		MOUSE_LEFT_BUTTON = 0,
		MOUSE_MIDDLE_BUTTON = 1,
		MOUSE_RIGHT_BUTTON = 2,
		MOUSE_SCROLL_UP = 3,
		MOUSE_SCROLL_DOWN = 4
	};

	if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))
	{
		int wx, wy, wz;
		GetOGLPos(x,y, wx, wy, wz);
		//int pIndex= DenaulayTriangulation.psa.addPoint(wx,wy);
		int intri_result = -1;
		bool outsidePoint = true;

		int ipIndex = newInsertedPsa.addPoint(wx,wy);


	} //if((button == MOUSE_RIGHT_BUTTON)&&(state == GLUT_UP))

	glutPostRedisplay();
}

void generate_test_input(int num, int bound)
{
	
	//write to  ��input.txt�� get data from global variables trist & psa
	double instCount = 0; // Instruction count
	ofstream outputFile("input.txt",ios::out); // output file

	
  /* initialize random seed: */
  srand ( time(NULL) );

  

  // obtain individual point data from psa and writing the AP command
	for(int i= 1; i<=num; i++)
	{
		double dx = -bound/2 + rand() % bound + 1;
		double dy = -bound/2 + rand() % bound + 1;
		outputFile<<std::setw(4) << std::setfill('0') << instCount << ": IP " << dx << " " << dy << endl;
		instCount++;
	}

	outputFile<<std::setw(4) << std::setfill('0') << instCount << ": CD " << endl;


	outputFile.close();
}

int main(int argc, char **argv)
{
	//generate_test_input(100, 400);
	//return 0;


	width = 1000; height = 800;
	cout<<"CS5237 Phase III"<< endl<< endl;
	cout << "Right mouse click: OT operation"<<endl;
	cout << "Q: Quit" <<endl;
	cout << "R: Read in control points from \"input.txt\"" <<endl;
	cout << "W: Write control points to \"input.txt\"" <<endl;
	cout << "Zoom Functions: I: Zoom in, O: Zoom Out" <<endl;
	cout << "E,S,D,X: Scroll Up, Left, Right, Down, Respectively." <<endl;
	cout << "N: Reset the view to the original position if you lost the direction." <<endl;
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (width, height);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS5237 Phase III");
	init ();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
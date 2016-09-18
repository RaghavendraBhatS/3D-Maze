#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#define row 17      	//maze size
#define col 17     	//these have to be odd numbers
#define cubeSize 30.0  	//size of cubes
#define halfCube cubeSize/2
#define bound 4 	//bounding box value/2
#define pi 3.14159
#define tree 1
#define grass 2
#define cone 3
#define cylinder 4
#define light 5
#define lamp 6


GLfloat camdist=302,theta=79,thetar,alpha=0,alphar;
GLfloat grey[]={0.3, 0.3, 0.3, 1.0};
GLfloat black[]={0, 0, 0, 1.0};
GLfloat green[]={0, 0.3, 0, 1.0};
GLfloat darkestgreen[]={0, 0.2, 0, 1.0};
GLfloat lightyellow[]={0.8, 0.8, 0.5, 1.0};
GLfloat brown[]={0.25, 0.0, 0.0, 1.0};
GLfloat darkbrown[]={0.10, 0.0, 0.0, 1.0};
GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat red[] = { 1.0, 0, 0, 1.0 };
GLfloat lightgreen[]={0, 0.5, 0, 1.0};
GLdouble posX=cubeSize+halfCube,posZ=halfCube;
int maze[row][col]; 	//the maze matrix
long start_time, end_time, elapsed;
int width,height;
int day=1;
int scene=0,lights=0;


//Functions for generating and drawing the maze
void generateMaze();
void recursion(int r,int c);
void shuffle(int directions[4]);
void drawMaze();
void createList();

//Functions for everything else
void Keys(unsigned char key,int x,int y);
void myReshape(int w,int h);
void display();
void move();
void drawkey(const char* str, GLfloat x, GLfloat y);

//functions for lighting
void Lighting();
void init();

void generateMaze()
{
	int i,j;
	for(i=0;i<row;i++)
		for(j=0;j<col;j++)
			maze[i][j]=1;	//set all to 1
	srand(time(NULL));
	recursion(1,1);
	maze[1][0]=0;			//starting opening top left
	maze[row-2][col-1]=0;		//ending opening botton right
}

void recursion(int r,int c)
{
	int i;
	int directions[4]={1,2,3,4};
	shuffle(directions);
	for(i=0;i<4;i++)
	{
		switch(directions[i])
		{
		 case 1: // Up
             //　Whether 2 cells up is out or not
             if (r - 2 <= 0)
                 continue;
             if (maze[r - 2][c] != 0)
             {
                 maze[r-2][c] = 0;
                 maze[r-1][c] = 0;
                 recursion(r - 2, c);
             }
             break;
         case 2: // Right
             // Whether 2 cells to the right is out or not
             if (c + 2 >= col - 1)
                 continue;
             if (maze[r][c + 2] != 0)
             {
                 maze[r][c + 2] = 0;
                 maze[r][c + 1] = 0;
                 recursion(r, c + 2);
             }
             break;
         case 3: // Down
             // Whether 2 cells down is out or not
             if (r + 2 >= row - 1)
                 continue;
             if (maze[r + 2][c] != 0)
             {
                 maze[r+2][c] = 0;
                 maze[r+1][c] = 0;
                 recursion(r + 2, c);
             }
             break;
         case 4: // Left
             // Whether 2 cells to the left is out or not
             if (c - 2 <= 0)
                 continue;
             if (maze[r][c - 2] != 0)
             {
                 maze[r][c - 2] = 0;
                 maze[r][c - 1] = 0;
                 recursion(r, c - 2);
             }
             break;
		}
	}
}

/*shuffles direction array*/
void shuffle(int directions[4])
{
    size_t i;
	for (i = 0; i < 4; i++)
	{
	  size_t j = i + rand() / (RAND_MAX / (4 - i) + 1);
	  int t = directions[j];
	  directions[j] = directions[i];
	  directions[i] = t;
	}
}

void Keys(unsigned char key,int x, int y)
{
    if(scene==0 || scene==1)
    {
        switch(key)
        {
            case 'S':
            case 's':scene=2;
                myReshape(width,height);
                break;
            case 'I':
            case 'i':scene=1;
                break;
        }
    }
    else
    {
        switch(key)
        {
            //camera movement cases
            case 'n':if(day)
                        day=0;
                    else
                        day=1;
                    break;
            case 'i':if(camdist>=9)
                camdist-=1.5;
                    break;
            case 'k':camdist+=1.5;
                    break;
            case 'j':if(alpha<=0)
                        alpha=360;
                    else
                        alpha-=5;
                    break;
            case 'l':if(alpha>=360)
                        alpha=0;
                    else
                        alpha+=5;
                    break;
            case 'u':if(theta<=1)
                        break;
                    else
                        theta-=1;
                    break;
            case 'o':if(theta>=88)
                        break;
                    else
                        theta+=1;
                    break;
            case 'r':camdist=302,theta=79,alpha=360;    //reset camera
                    break;

            //quit
            case 'q':exit(0);
                    break;
	}

        if((alpha>315 && alpha<=360) || (alpha<=45 && alpha>=0))
        {
            switch(key)
            {
                //player movement and collision detection cases
                case 'a':if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //left
                        if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX+=2;
                        break;
                case 's':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)  //down
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)
                                posZ-=2;
                        break;
                case 'd':if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //right
                        if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX-=2;
                        break;
                case 'w':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)  //up
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)
                                posZ+=2;
                        break;
            }
        }

        if(alpha>45 && alpha<=135)
        {
            switch(key)
            {
                //player movement and collision detection cases
                case 'w':if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //left
                        if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX+=2;
                        break;
                case 'a':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)  //down
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)
                                posZ-=2;
                        break;
                case 's':if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //right
                        if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX-=2;
                        break;
                case 'd':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)  //up
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)
                                posZ+=2;
                        break;
            }
        }

        if(alpha>135 && alpha<=225)
        {
            switch(key)
            {
                //player movement and collision detection cases
                case 'd':if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //left
                        if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX+=2;
                        break;
                case 'w':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)  //down
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)
                                posZ-=2;
                        break;
                case 'a':if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //right
                        if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX-=2;
                        break;
                case 's':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)  //up
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)
                                posZ+=2;
                        break;
            }
        }

        if(alpha>225 && alpha<=315)
        {
            switch(key)
            {
                //player movement and collision detection cases
                case 's':if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //left
                        if((maze[(int)((posX+bound+1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX+=2;
                        break;
                case 'd':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)  //down
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ-bound-1)/cubeSize)])==0)
                                posZ-=2;
                        break;
                case 'w':if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ-bound+1)/cubeSize)])==0)  //right
                        if((maze[(int)((posX-bound-1)/cubeSize)][(int)((posZ+bound-1)/cubeSize)])==0)
                                posX-=2;
                        break;
                case 'a':if((maze[(int)((posX+bound-1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)  //up
                        if((maze[(int)((posX-bound+1)/cubeSize)][(int)((posZ+bound+1)/cubeSize)])==0)
                                posZ+=2;
                        break;
            }
        }
    }
    glutPostRedisplay();
}

void myReshape(int w,int h)
{
    width=w;height=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if(w<=h)
        gluPerspective(45,(GLfloat)h/(GLfloat)w,1,10000);
    else
        gluPerspective(45,(GLfloat)w/(GLfloat)h,1,10000);
    glMatrixMode(GL_MODELVIEW);
}

void drawMaze()
{
    int i,j;

    //draw floor of maze
    glPushMatrix();
    glMaterialfv(GL_FRONT,GL_DIFFUSE,darkbrown);
    glMaterialfv(GL_FRONT,GL_SPECULAR,black);
    glPushMatrix();
    glTranslatef(halfCube*row,-0.1*halfCube,halfCube*col);
    glScalef(row,0.1,col);
    glutSolidCube(cubeSize);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(cubeSize+(halfCube/6),0,halfCube/6);
        glCallList(lamp);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(((cubeSize*row)-(cubeSize+halfCube/6)),0,(cubeSize*col));
        glCallList(lamp);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(halfCube,0,halfCube);
    for(i=0;i<row;i++)
        for(j=0;j<col;j++)
        {
            glPushMatrix();
            glTranslatef(i*cubeSize,0,j*cubeSize);
            if(maze[i][j]==1)
            {
                glCallList(grass);
                glCallList(tree);
            }
            glPopMatrix();
        }
    glPopMatrix();

}

void move()
{
    glPushMatrix();
    glTranslatef(posX,bound,posZ);
        glMaterialfv(GL_FRONT,GL_SPECULAR,white);
        glMaterialf(GL_FRONT,GL_SHININESS,50);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,red);
        glutSolidSphere(bound,20,20);
    glPopMatrix();
}

void Text(const char *input, float x, float y, int flag, float c1, float c2, float c3)
{
	void *font;
	glColor3f(c1,c2,c3);
	if(flag==0)
		font=GLUT_BITMAP_TIMES_ROMAN_24;
	else if(flag==1)
		font=GLUT_BITMAP_HELVETICA_12;
	else
		font=GLUT_BITMAP_HELVETICA_18;
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	int i,len;
	glRasterPos2f(x,y);
	len=(int)strlen(input);
	for(i=0;i<len;i++)
		glutBitmapCharacter(font,input[i]);
	glDisable(GL_BLEND);
}

void lighting()
{
    GLfloat spec1[] = {1.0, 1.0, 1.0, 1.0 };
    GLfloat diff1[]={0.9, 0.9, 0.9, 1.0};
    GLfloat amb1[]={0.1, 0.1, 0.1, 1.0};
    GLfloat diff0n[]={0.3, 0.3, 0.3, 1.0};
    GLfloat amb0n[] = {0, 0, 0, 1.0};
    GLfloat diff0d[]={1.0, 1.0, 1.0, 1.0};
    GLfloat amb0d[] = {0.6, 0.6, 0.6, 1.0};
    GLfloat position0[]={cubeSize*row/2,cubeSize*20,cubeSize*col,0};
    GLfloat position1[]={cubeSize+(halfCube/6),cubeSize,halfCube/6,1.0};
    GLfloat position2[]={(cubeSize*row)-(cubeSize+halfCube/6),cubeSize,cubeSize*col,1.0};
    glPushMatrix();
    if(day==1)
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,amb0d);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diff0d);
    }
    else
    {
        glLightfv(GL_LIGHT0,GL_AMBIENT,amb0n);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,diff0n);
    }
    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glPopMatrix();
    glPushMatrix();
    glLightfv(GL_LIGHT1,GL_SPECULAR,spec1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,diff1);
    glLightfv(GL_LIGHT1,GL_AMBIENT,amb1);
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1);
    glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.005);
    glLightfv(GL_LIGHT1, GL_POSITION,position1);
    glPopMatrix();
    glPushMatrix();
    glLightfv(GL_LIGHT2,GL_SPECULAR,spec1);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,diff1);
    glLightfv(GL_LIGHT2,GL_AMBIENT,amb1);
    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,1);
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.005);
    glLightfv(GL_LIGHT2, GL_POSITION, position2);
    glPopMatrix();
}

void drawkey(const char* str, GLfloat x, GLfloat y)
{
    GLfloat keySize1=30,keySize2=25;
    glColor3f(0.4,0.4,0.4);
    glBegin(GL_QUADS);
        glVertex3f(x-keySize1,y-keySize1,0);
        glVertex3f(x-keySize1,y+keySize1,0);
        glVertex3f(x+keySize1,y+keySize1,0);
        glVertex3f(x+keySize1,y-keySize1,0);

    glColor3f(0.8,0.8,0.8);
        glVertex3f(x-keySize2,y-keySize2,0.1);
        glVertex3f(x-keySize2,y+keySize2,0.1);
        glVertex3f(x+keySize2,y+keySize2,0.1);
        glVertex3f(x+keySize2,y-keySize2,0.1);
    glEnd();
    glPushMatrix();
    glTranslatef(0,0,0.2);
    Text(str,x-10,y-10,0,0,0,0);
    glPopMatrix();
}

void display()
{
    if(scene==0)    //Start name page
    {
        glDisable(GL_LIGHTING);
        lights=0;
        glClearColor(0.2,0.7,0.7,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,1000,0,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        Text("DEPARTMENT OF COMPUTER SCIENCE AND ENGINEERING",250,850,0,0,0,0);
        Text("ST. JOSEPH ENGINEEERING COLLEGE, MANGALURU-28",265,800,0,0,0,0);
        Text("COMPUTER GRAPHICS PROJECT",350,600,0,0,0,0);
        Text("\"3D MAZE GAME\"",415,550,0,0.4,1,0.2);
        Text("SUBMITTED BY:",415,350,0,0,0,0);
        Text("SHRAVAN KANAGOKAR",140,275,0,0.4,1,0.2);
        Text("4SO13CS106",185,225,0,0.4,1,0.2);
        Text("VI SEMESTER CS&E",155,175,0,0.4,1,0.2);
        Text("TREVOR NEIL LOREN",620,275,0,0.4,1,0.2);
        Text("4SO13CS120",660,225,0,0.4,1,0.2);
        Text("VI SEMESTER CS&E",630,175,0,0.4,1,0.2);
        Text("Press 'S' to start game or 'I' for instructions",397,100,1,0,0,0);
    }

    else if(scene==1)    //instructions
    {
        glDisable(GL_LIGHTING);
        lights=0;
        glClearColor(0.2,0.7,0.7,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,1366,0,768);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glPushMatrix();
        glTranslatef(200,-150,0);

        drawkey("W",240,600);
        drawkey("S",260,537);
        drawkey("A",195,537);
        drawkey("D",325,537);
        drawkey("R",370,600);
        drawkey("N",555,474);
        drawkey("U",565,600);
        drawkey("I",630,600);
        drawkey("O",695,600);
        drawkey("K",650,537);
        drawkey("J",585,537);
        drawkey("L",715,537);

        Text("left",130,530,3,0,0,0);
        Text("reverse",230,490,3,0,0,0);
        Text("forward",210,635,3,0,0,0);
        Text("right",360,530,3,0,0,0);
        Text("reset camera",320,635,3,0,0,0);
        Text("toggle day/night",495,425,3,0,0,0);
        Text("zoom in",595,635,3,0,0,0);
        Text("pan right",475,530,3,0,0,0);
        Text("pan left",750,530,3,0,0,0);
        Text("tilt down",500,635,3,0,0,0);
        Text("tilt up",700,635,3,0,0,0);
        Text("zoom out",615,490,3,0,0,0);
        Text("Press 'S' to start game",400,300,1,0,0,0);
        glPopMatrix();
    }

    else if(scene==2)    //The maze
    {
        if (lights==0)
        {
            init();
            myReshape(width,height);
            lights=1;
            glutPostRedisplay();
        }
        alphar=alpha*(pi/180);
        thetar=theta*(pi/180);
        lighting();
        if(day)
            glClearColor(0.7,0.7,1,1);
        else
            glClearColor(0.05,0,0.1,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        gluLookAt(posX-camdist*sin(alphar),camdist*tan(thetar),posZ-camdist*cos(alphar),posX,bound,posZ,0,1,0);
        glLineWidth(3);
        glPushMatrix();
        drawMaze();
        move();
        glPopMatrix();

        //finish maze condition
        if(posZ>(col*cubeSize)-cubeSize/4)
        {
            scene=3;
            end_time=clock();       //stop the timer and calculate elapsed time
            elapsed=(end_time - start_time) / CLOCKS_PER_SEC;
        }
    }

    else if(scene==3)   //game over, score, etc
    {
        glDisable(GL_LIGHTING);
        glClearColor(0.2,0.7,0.7,1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0,1000,0,1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        Text("GAME OVER",450,550,0,0.4,1,0.2);
        char d[50];
        sprintf(d,"You finished the game in: %ld seconds",elapsed);
        Text(d,375,400,0,0,0,0);
	lights=0;
    }
    glFlush();
    glutSwapBuffers();
}

void init(void)		//lighting
{
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    start_time=clock();
}

void menu(int id)
{
    switch(id)
    {
        case 1:scene=0;
                break;
        case 2:scene=1;
                break;
        case 3:scene=2;
                generateMaze(maze);
                posX=cubeSize+halfCube;
                posZ=halfCube;
                start_time=clock();
		myReshape(width,height);
                break;
        case 4:if(day)
                    day=0;
                else
                    day=1;
                break;
        case 5:exit(0);
                break;
    }
    glutPostRedisplay();
}

void createList()
{
    //Draws the Tree Trunk
    glNewList(cylinder,GL_COMPILE);
        gluCylinder(gluNewQuadric(),cubeSize/6,cubeSize/6,halfCube,20,20);
    glEndList();
    //Draws the Cones for the Tree
    glNewList(cone,GL_COMPILE);
        glTranslatef(0,0,halfCube);
        glutSolidCone(halfCube,cubeSize,8,8);
        glTranslatef(0,0,halfCube);
        glutSolidCone(halfCube-halfCube/6,cubeSize,8,8);
        glTranslatef(0,0,halfCube);
        glutSolidCone(halfCube-halfCube/3,cubeSize,8,8);
    glEndList();
    //Draws the Base of the Tree
    glNewList(grass,GL_COMPILE);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,lightgreen);
        glMaterialfv(GL_FRONT,GL_SPECULAR,darkestgreen);
        glPushMatrix();
        glScalef(1,0.1,1);
        glutSolidCube(cubeSize);
        glPopMatrix();
    glEndList();
    //Draws the Tree
    glNewList(tree,GL_COMPILE);
        glRotatef(270,1,0,0);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,brown);
        glMaterialfv(GL_FRONT,GL_SPECULAR,darkbrown);
        glCallList(cylinder);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,green);
        glMaterialfv(GL_FRONT,GL_SPECULAR,darkestgreen);
        glCallList(cone);
    glEndList();
    //Draws the Lamp Post
    glNewList(lamp,GL_COMPILE);
        glPushMatrix();
        glTranslatef(0,cubeSize,0);
        glMaterialfv(GL_FRONT,GL_EMISSION,lightyellow);
        glutSolidSphere(halfCube/6,10,10);
        glPopMatrix();
        glPushMatrix();
        glMaterialfv(GL_FRONT,GL_EMISSION,black);
        glMaterialfv(GL_FRONT,GL_DIFFUSE,grey);
        glMaterialfv(GL_FRONT,GL_SPECULAR,white);
        glRotatef(-90,1,0,0);
        gluCylinder(gluNewQuadric(),halfCube/10,halfCube/10,cubeSize,10,10);
        glPopMatrix();
    glEndList();
}

void createmenu()
{
	glutCreateMenu(menu);
	glutAddMenuEntry("Introduction",1);
	glutAddMenuEntry("Instructions",2);
	glutAddMenuEntry("New Game",3);
	glutAddMenuEntry("Toggle Day/Night",4);
	glutAddMenuEntry("Quit",5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
	generateMaze(maze);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH|GLUT_RGBA);
	glutInitWindowSize (1366, 768);
	glutCreateWindow("3D MAZE GAME");
	createList();
	createmenu();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(Keys);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}


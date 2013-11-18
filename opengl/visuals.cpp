#include <math.h>
#include <iostream>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "visuals.h"
#include <time.h>
#include "shader.hpp"
#include <vector>
#include "ReadFile.h"

#define PI 3.14159265359
#define mouseSpeed 0.00005f
#define cursor_speed 0.0018f
#define RADIUS 0.25;
// units per second
int C = 0; //color:
int S = 1; //scale
int X = 2; //coord x
int Y = 3; //coord Y

float DATA[] = {	0.0, 1.0, 1.0, 0.0,
					1.0, 1.0, 2.0, 0.0,
					2.0, 1.0, 3.0, 0.0, 
					3.0, 1.0, 4.0, 0.0	};
char * filename = "C:\\Users\\Student\\Desktop\\LAB4\\VisualStudio\\project\\MainProject\\Debug\\test.txt";
float *data;

int length = 4*100;


// This will be used with shader
//GLuint VertexArrayID;
GLuint	onebuffer, twobuffer, threebuffer, fourbuffer, fivebuffer, sixbuffer, sevenbuffer, 
		eightbuffer, ninebuffer, zerobuffer, shape1buffer, shape2buffer, shape3buffer,  shape4buffer, shape5buffer, shape6buffer, 
		colorbuffer, redbuffer, bluebuffer, yellowbuffer, greenbuffer, pinkbuffer, orangebuffer;
GLuint objvertexbuffer; // for obj
GLuint programID_1, programID_2;

int last_time, current_time;

GLuint MatrixID; // Handler Matrix for moving the cam
glm::mat4 MVP, M, Mn, Mm; // FInal Homogeneous Matrix

glm::mat4 MVPUnit, MVPNumber;
glm::mat4 Projection,View,Model;

// Variables for moving camera with mouse
int mouse_x = 800/2;
int mouse_y = 600/2;
int mouse_button =  GLUT_LEFT_BUTTON;
int mouse_state = GLUT_UP;
int sp_key =0;

float counter=0;
// Initial position : on +Z
glm::vec3 position = glm::vec3(5.63757, 1.7351, -2.19067 );
// Initial horizontal angle : toward -Z
float horizontalAngle = -1.07947;
// Initial vertical angle : none
float verticalAngle = -0.369055;
// Initial Field of View
float initialFoV = 45.0f;
glm::vec3 direction =  glm::vec3(0,0,0) - position;
glm::vec3 right  = glm::vec3(0,0,1) ;
glm::vec3 up = glm::vec3(0,1,0);
int reds, greens, blues, yellows, pinks, oranges;

void MouseGL(int button, int state, int x, int y)
{
    if ( state == GLUT_DOWN) // if key is pressed
    {
        mouse_state=state; // save in global memomry
        // move point to the midle of screen
        glutWarpPointer(800/2, 600/2);
    }
    else
    {
        mouse_state=GLUT_UP;
    }
}

void Mouse_active(int x, int y)
{
    //save values in global memory
    mouse_x=x;
    mouse_y=y;
}

int dt;
void Idle()
{
    current_time = glutGet(GLUT_ELAPSED_TIME);

    dt = current_time - last_time;

    if(mouse_state ==  GLUT_DOWN)
    {
        horizontalAngle += mouseSpeed * float( 800/2 - mouse_x); // 800 = window width
        verticalAngle   += mouseSpeed * float(600/2 - mouse_y );// 600 = window height

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        direction= glm::vec3(
                    cos(verticalAngle) * sin(horizontalAngle),
                    sin(verticalAngle),
                    cos(verticalAngle) * cos(horizontalAngle)
                    );

        // Right vector
        right = glm::vec3(
                    sin(horizontalAngle - PI/2.0f),
                    0,
                    cos(horizontalAngle - PI/2.0f)
                    );

        // Up vector
        up = glm::cross( right, direction );
    }

    //            // Move forward
    if (sp_key == GLUT_KEY_UP)
    {
        position += direction * (dt * cursor_speed);
    }
    // Move backward
    if (sp_key == GLUT_KEY_DOWN)
    {

        position -= direction * (dt * cursor_speed);
    }
    // Strafe right
    if (sp_key == GLUT_KEY_RIGHT)
    {

        position += right * (dt * cursor_speed);
    }
    // Strafe left
    if (sp_key== GLUT_KEY_LEFT)
    {

        position -= right * (dt * cursor_speed);
    }
    sp_key =0;
	counter =counter+0.002*dt;

    // Camera matrix
	View       = glm::lookAt(
				position, // Camera is at position, in World Space
				position+direction, // and looks here : at the same position, plus "direction"
				up                  // Head is up (set to 0,-1,0 to look upside-down)
				);

    glutPostRedisplay();
    std::cout << position.x << " " << position.y <<  " " << position.z << std::endl;


    last_time =current_time;// update when the last timer;
}


void ReshapeGL( int w, int h )
{
    std::cout << "ReshapGL( " << w << ", " << h << " );" << std::endl;

    if ( h == 0)                                        // Prevent a divide-by-zero error
    {
        h = 1;                                      // Making Height Equal One
    }

    g_iWindowWidth = w;
    g_iWindowHeight = h;

    glViewport( 0, 0, 800, 600 );

}

void Specialkey(int c, int x, int y )
{
    sp_key =c;
}

void KeyboardGL( unsigned char c, int x, int y )
{
    // Store the current scene so we can test if it has changed later.
    unsigned char currentScene = g_eCurrentScene;

    switch ( c )
    {
    case '1':
    {
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );                         // Black background
        g_eCurrentScene = 1;
    }
        break;
    case '2':
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );                         // Black background
        g_eCurrentScene = 2;
    }
        break;
    case '3':
    {
        glClearColor( 0.27f, 0.27f, 0.27f, 1.0f );                      // Dark-Gray background
        g_eCurrentScene = 3;
    }
        break;
    case '4':
    {
        glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );                      // Light-Gray background
        g_eCurrentScene = 4;
    }
        break;

    case 's':
    case 'S':
    {
        std::cout << "Shade Model: GL_SMOOTH" << std::endl;
        // Switch to smooth shading model
        glShadeModel( GL_SMOOTH );
    }
        break;
    case 'f':
    case 'F':
    {
        std::cout << "Shade Model: GL_FLAT" << std::endl;
        // Switch to flat shading model
        glShadeModel( GL_FLAT );
    }
        break;
    case '\033': // escape quits
    case '\015': // Enter quits
    case 'Q':    // Q quits
    case 'q':    // q (or escape) quits
    {
        // Cleanup up and quit
        Cleanup(0);
    }
        break;

    case '\72': //arrow up
    {

    }
        break;
    }


    if ( currentScene != g_eCurrentScene )
    {
        std::cout << "Changed Render Scene: " << int( g_eCurrentScene ) << std::endl;
    }

    glutPostRedisplay();
}


void Cleanup( int errorCode, bool bExit )
{
    if ( g_iGLUTWindowHandle != 0 )
    {
        glutDestroyWindow( g_iGLUTWindowHandle );
        g_iGLUTWindowHandle = 0;
    }

    if ( bExit )
    {
        exit( errorCode );
    }
}


void DisplayGL()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
    // and the depth buffer

    switch ( g_eCurrentScene )
    {
    case 1:
    {
		float *d1 = &DATA[0];
		float *d2 = data;
		float *d3 = &data[0];
		
		RenderScene(data, length);
    }
        break;
	}
	
    glutSwapBuffers();
    // All drawing commands applied to the
    // hidden buffer, so now, bring forward
    // the hidden buffer and hide the visible one
}

GLuint IntToBuffer(int i)
{
	switch(i)
	{
	case 0:
	{
		return zerobuffer;
	} break;
	case 1:
	{
		return onebuffer;
	} break;
	case 2:
	{
		return twobuffer;
	} break;
	case 3:
	{
		return threebuffer;
	} break;
	case 4:
	{
		return fourbuffer;
	} break;
	case 5:
	{
		return fivebuffer;
	} break;
	case 6:
	{
		return sixbuffer;
	} break;
	case 7:
	{
		return sevenbuffer;
	} break;
	case 8:
	{
		return eightbuffer;
	} break;
	case 9:
	{
		return ninebuffer;
	} break;
	}
	return onebuffer;

}

int count = 0;
void RenderScene(float *data, int length)
{
    glClearColor( 0.4f, 0.4f, 0.4f, 1.0f );
    // Enable bac-face culling
    glEnable(GL_CULL_FACE);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 1000.0f);
    // Model matrix : an identity matrix (model will be at the origin)
    Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    // Use our shader
    glUseProgram(programID_1);
    // 1rst attribute buffer : vertices
    //glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, shape1buffer);
    //glVertexAttribPointer(
    //            0, // attribute. No particular reason for 0, but must match the layout in the shader.
    //            3, // size
    //            GL_FLOAT, // type
    //            GL_FALSE, // normalized?
    //            0, // stride
    //            (void*)0 // array buffer offset
    //            );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);

/*        for(int i = 0; i < length; i++)
                printf("D:%f", d[i]);*/
	count++;
        for(int i = 0; i < length / 4; i++)
        {
			
                float* d = read(filename, i);
                M = glm::mat4(1.0f);

                M = glm::translate(M,glm::vec3(
                        d[X] / 100
                        , 0.0,
                        d[Y] / 100));

                M = glm::scale(M,glm::vec3(
                        d[S] * 3/ 100,
                        d[S] * 3/ 100,
                        d[S] * 3/ 100));

                int color = d[C];
                
                MVPUnit = MVP * M;

				Mn = glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 1.0, 0.3));
				Mn = glm::rotate(Mn, float(count), glm::vec3(0, 1, 0));
                MVPNumber = MVPUnit * Mn;
               
				Mm = glm::rotate(Mm, float(count), glm::vec3(0, 1, 0));
				glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);

				glVertexAttribPointer(
					1, // attribute. No particular reason for 1, but must match the layout in the shader.
					3, // size
					GL_FLOAT, // type
					GL_FALSE, // normalized?
					0, // stride
					(void*)0 // array buffer offset
				);

                switch(color)
                {
                case 4:
				{
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape4buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, yellowbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = yellows / 10;
					int num2 = yellows - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, yellowbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

                }
                        break;
                case 1:
                {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape1buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = reds / 10;
					int num2 = reds - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
                }
                        break;
                case 3:
                {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape3buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = blues / 10;
					int num2 = blues - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, bluebuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
                }
                        break;
                case 2:
                {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape2buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = greens / 10;
					int num2 = greens - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
                }
                        break;
                
                case 5:
                {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape5buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, pinkbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = pinks / 10;
					int num2 = pinks - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, pinkbuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
                }        break;
                
                case 6:
                {
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
					// 1rst attribute buffer : vertices
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, shape3buffer);
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
                    glBindBuffer(GL_ARRAY_BUFFER, orangebuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					int num1 = oranges / 10;
					int num2 = oranges - num1 * 10; 

					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num2));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);

					// 1rst attribute buffer : vertices
					//glEnableVertexAttribArray(0);
					MVPNumber = MVPNumber * glm::translate(glm::mat4(1.0f),glm::vec3(0.0, 0.0, -0.3));;
						
					glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPNumber[0][0]);
					glBindBuffer(GL_ARRAY_BUFFER, IntToBuffer(num1));
					glVertexAttribPointer(
								0, // attribute. No particular reason for 0, but must match the layout in the shader.
								3, // size
								GL_FLOAT, // type
								GL_FALSE, // normalized?
								0, // stride
								(void*)0 // array buffer offset
								);

                    glBindBuffer(GL_ARRAY_BUFFER, bluebuffer);
					glDrawArrays(GL_TRIANGLES, 0, 12*3);
                }        break;
                }



			
			//glDrawArrays(GL_TRIANGLES, 0, 12*3); 
        }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void SetupGL() //
{
	for(int i = 0; i < length; i++)
	{
		int color = read(filename, i)[C];
		switch(color)
		{
		case 1:
		{
			reds++;
		}break;
		case 2:
		{
			greens++;
		} break;
		case 3:
		{
			blues++;
		} break;
		case 4:
			{
			yellows++;
		} break;
		case 5:
		{
			pinks++;
		} break;
		case 6:
		{
			oranges++;
		}
		}
	}

	MVPNumber = glm::mat4(1.0f);
	//data = read(filename);
	counter =counter+0.002*dt;
    //Parameter handling
    glShadeModel (GL_SMOOTH);

    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // polygon rendering mode
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

    //Set up light source
    GLfloat light_position[] = { 0.0, 30.0,-50.0,0.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Black background
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );   // Black background

    // Register GLUT callbacks
    glutDisplayFunc(DisplayGL);
    glutKeyboardFunc(KeyboardGL);
    glutSpecialFunc(Specialkey);
    glutReshapeFunc(ReshapeGL);
    glutMouseFunc(MouseGL);
    glutMotionFunc(Mouse_active);

    //Call to the drawing function
    glutIdleFunc(Idle);
    last_time = glutGet(GLUT_ELAPSED_TIME);

    // Setup initial GL State
    glClearDepth( 1.0f );

    //
    // Init GLEW
    if ( glewInit() != GLEW_OK )
    {
        std::cerr << "Failed to initialize GLEW." << std::endl;
        exit(-1);
    }

    // Setup initial GL State
    glClearDepth( 1.0f );

    std::cout << "Initialise OpenGL: Success!" << std::endl;

    //VAO
    //        glGenVertexArrays(1, &VertexArrayID);
    //        glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID_1 = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );


	const float MULT = 0.4f;
    //VBO -- VERTEX
	double r = RADIUS;

	static const GLfloat g_one_buffer_data[] = {
	    -r / 2,		-r,		0, // triangle 1 : begin
        -r / 2,		-r,		 r / 2,
        -r / 2,		 r,	 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,		0,
        -r / 2,		 r,	 r / 2,
        -r / 2,		 r,	0,
	};

	static const GLfloat g_two_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			-r / 2,//-r / 2,
		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		 r / 2,			 0,//r / 2,

		-r / 2,		 r / 2,			r / 2,//-r / 2,
		-r / 2,		 r / 2,			0,//-r / 2,
		-r / 2,		-r / 2,			0,//r / 2,


		//-r / 2,		 r / 2,			 0,//-r / 2,
		//-r / 2,		 r / 2,			-r / 2,//-r / 2,
		//-r / 2,		-r / 2,			-r / 2,//r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,
	};

	static const GLfloat g_three_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 2,//r / 2,

		-r / 2,		 r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 0,//r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,				 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,				-r / 2,
		-r / 2,		-r / 4,				 r / 2,
	};

	static const GLfloat g_four_buffer_data[] = {


		-r / 2,		-r,				 r / 4,//-r / 2,
		-r / 2,		-r,				 r / 2,//-r / 2,
		-r / 2,		 r,				 r / 2,//r / 2,

		-r / 2,		 r,				 r / 2,//-r / 2,
		-r / 2,		 r,				 r / 4,//-r / 2,
		-r / 2,		-r,				 r / 4,//r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,			 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,			-r / 2,
		-r / 2,		-r / 4,			 r / 2,

		-r / 2,		 0,				-r / 2,//-r / 2,
		-r / 2,		 0,				-r / 4,//-r / 2,
		-r / 2,		 r,				-r / 4,//r / 2,


		-r / 2,		 r,				-r / 4,//-r / 2,
		-r / 2,		 r,				-r / 2,//-r / 2,
		-r / 2,		 0,				-r / 2,//r / 2,
	};

	static const GLfloat g_five_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 r / 2,//-r / 2,
		-r / 2,		 0,		 		 r / 2,//r / 2,

		-r / 2,		 0,				 r / 2,//-r / 2,
		-r / 2,		 0,				 0,//-r / 2,
		-r / 2,		-r / 2,			 0,//r / 2,

		-r / 2,		 r / 2,			 0,//-r / 2,
		-r / 2,		 r / 2,			-r / 2,//-r / 2,
		-r / 2,		 0,		 		-r / 2,//r / 2,

		-r / 2,		 0,				-r / 2,//-r / 2,
		-r / 2,		 0,				 0,//-r / 2,
		-r / 2,		 r / 2,			 0,//r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,				 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,				-r / 2,
		-r / 2,		-r / 4,				 r / 2,
	};

	static const GLfloat g_six_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 r / 2,//-r / 2,
		-r / 2,		 0,		 		 r / 2,//r / 2,

		-r / 2,		 0,				 r / 2,//-r / 2,
		-r / 2,		 0,				 0,//-r / 2,
		-r / 2,		-r / 2,			 0,//r / 2,

		-r / 2,		-r,				 0,//-r / 2,
		-r / 2,		 r,		 		-r / 2,//r / 2,
		-r / 2,		-r,				-r / 2,//-r / 2,

		-r / 2,		 r,				-r / 2,//-r / 2,
		-r / 2,		-r,				 0,//r / 2,
		-r / 2,		 r,				 0,//-r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,				 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,				-r / 2,
		-r / 2,		-r / 4,				 r / 2,
	};

	static const GLfloat g_seven_buffer_data[] = {

		-r / 2,		-r / 2,			-r / 2,//-r / 2,
		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		 r / 2,			 0,//r / 2,

		-r / 2,		 r / 2,			r / 2,//-r / 2,
		-r / 2,		 r / 2,			0,//-r / 2,
		-r / 2,		-r / 2,			0,//r / 2,


		//-r / 2,		 r / 2,			 0,//-r / 2,
		//-r / 2,		 r / 2,			-r / 2,//-r / 2,
		//-r / 2,		-r / 2,			-r / 2,//r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,
	};

	static const GLfloat g_eight_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			 r / 4,//-r / 2,
		-r / 2,		-r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 2,//r / 2,

		-r / 2,		 r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 4,//-r / 2,
		-r / 2,		-r / 2,			 r / 4,//r / 2,

		-r / 2,		-r / 2,			-r / 4,//-r / 2,
		-r / 2,		 r / 2,			-r / 2,//r / 2,
		-r / 2,		-r / 2,			-r / 2,//-r / 2,

		-r / 2,		 r / 2,			-r / 2,//-r / 2,
		-r / 2,		-r / 2,			-r / 4,//r / 2,
		-r / 2,		 r / 2,			-r / 4,//-r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,			 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,			-r / 2,
		-r / 2,		-r / 4,			 r / 2,
	};

	static const GLfloat g_nine_buffer_data[] = {

		-r / 2,		 0,			 r / 4,//-r / 2,
		-r / 2,		 0,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 2,//r / 2,

		-r / 2,		 r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 4,//-r / 2,
		-r / 2,		 0,			 r / 4,//r / 2,

		-r / 2,		-r / 2,			-r / 4,//-r / 2,
		-r / 2,		 r / 2,			-r / 2,//r / 2,
		-r / 2,		-r / 2,			-r / 2,//-r / 2,

		-r / 2,		 r / 2,			-r / 2,//-r / 2,
		-r / 2,		-r / 2,			-r / 4,//r / 2,
		-r / 2,		 r / 2,			-r / 4,//-r / 2,

		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

		-r / 2,		 r / 4,			-r / 2, // triangle 1 : begin
        -r / 2,		-r / 4,			 r / 2,  // triangle 1 : end
		-r / 2,		 r / 4,			 r / 2,

		-r / 2,		 r / 4,			-r / 2,
		-r / 2,		-r / 4,			-r / 2,
		-r / 2,		-r / 4,			 r / 2,
	};

	static const GLfloat g_zero_buffer_data[] = {
	    -r / 2,		-r,				-r / 2, // triangle 1 : begin
        -r / 2,		-r,				 r / 2,
        -r / 2,		-r / 2,			 r / 2,  // triangle 1 : end
        
		-r / 2,		-r,				-r / 2,
		-r / 2,		-r / 2,			 r / 2,
        -r / 2,		-r / 2,			-r / 2,

		-r / 2,		-r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 r / 2,//r / 2,

		-r / 2,		 r / 2,			 r / 2,//-r / 2,
		-r / 2,		 r / 2,			 0,//-r / 2,
		-r / 2,		-r / 2,			 0,//r / 2,

		-r / 2,		-r / 2,			-r / 2,//-r / 2,
		-r / 2,		-r / 2,			-0,//-r / 2,
		-r / 2,		 r / 2,			-0,//r / 2,


		-r / 2,		 r / 2,			 0,//-r / 2,
		-r / 2,		 r / 2,			 -r / 2,//-r / 2,
		-r / 2,		-r / 2,			 -r / 2,//r / 2,


		-r / 2,		 r / 2,			-r / 2, // triangle 1 : begin
        -r / 2,		 r / 2,			 r / 2,
        -r / 2,		 r,				 r / 2,  // triangle 1 : end
        
		-r / 2,		 r / 2,			-r / 2,
		-r / 2,		 r,				 r / 2,
        -r / 2,		 r,				-r / 2,

	};
    static const GLfloat g_shape1_buffer_data[] = {
        -r,	-r, -r, // triangle 1 : begin
        -r,	-r, r,
        -r,	 r, r, // triangle 1 : end
        
		 r,	 r,-r, // triangle 2 : begin
        -r,	-r,-r,
        -r,	 r,-r, // triangle 2 : end
         
		 r,	-r, r,
        -r,	-r,-r,
         r,	-r,-r,
         
		 r,	 r,-r,
         r,	-r,-r,
        -r,	-r,-r,
        
		-r,	-r,-r,
        -r,	 r, r,
        -r,	 r,-r,
        
		 r,	-r, r,
        -r,	-r, r,
        -r,	-r,-r,
        
		-r,	 r, r,
        -r,	-r, r,
         r,	-r, r,
        
		 r,	 r, r,
         r,	-r,-r,
         r,	 r,-r,
         
		 r,	-r,-r,
         r,	 r, r,
         r,	-r, r,
         
		 r,	 r, r,
         r,	 r,-r,
        -r,	 r,-r,
         
		 r,	 r, r,
        -r,	 r,-r,
        -r,	 r, r,
         
		 r,	 r, r,
        -r,	 r, r,
         r,	-r, r, //12*3

    };
	
	float o = 0.1f;

	static const GLfloat g_shape2_buffer_data[] = {
        -r ,	 r,		-r, // floor : begin
         r,		 r,		-r,
		-r ,	 r,		 r, 
        
        -r,		 r,		 r,
		 r,		 r,		-r, 
		 r,		 r,		 r, //floor: end

		 r,		 r,		-r, //side 1
		-r,		 r,		-r,
		 0,		-r,		 0,

		 r,		 r,		 r,
		 r,		 r,		-r,
		 0,		-r,		 0,

		-r,		 r,		 r,
		 r,		 r,		 r,
		 0,		-r,		 0,

		-r,		 r,		-r,
		-r,		 r,		 r,
		 0,		-r,		 0,

		-r,		-r + r*2,		-r,
		 r,		-r + r*2,		-r, //side 1
		 0,		 r + r*2,		 0,

		 r,		-r + r*2,		-r,
		 r,		-r + r*2,		 r,
		 0,		 r + r*2,		 0,

		 r,		-r + r*2,		 r,
		-r,		-r + r*2,		 r,
		 0,		 r + r*2,		 0,

		-r,		-r + r*2,		 r,
		-r,		-r + r*2,		-r,
		 0,		 r + r*2,		 0

    };

	static const GLfloat g_shape3_buffer_data[] = {
        -r - o,	-r, -r - o, // triangle 1 : begin
        -r - o,	-r, r - o,
        -r + o,	 r, r + o, // triangle 1 : end
        
		 r + o,	 r,-r + o, // triangle 2 : begin
        -r - o,	-r,-r - o,
        -r + o,	 r,-r + o, // triangle 2 : end
         
		 r - o,	-r, r - o,
        -r - o,	-r,-r - o,
         r - o,	-r,-r - o,
         
		 r + o,	 r,-r + o,
         r - o,	-r,-r - o,
        -r - o,	-r,-r - o,
        
		-r - o,	-r,-r - o,
        -r + o,	 r, r + o,
        -r + o,	 r,-r + o,
        
		 r - o,	-r, r - o,
        -r - o,	-r, r - o,
        -r - o,	-r,-r - o,
        
		-r + o,	 r, r + o,
        -r - o,	-r, r - o,
         r - o,	-r, r - o,
        
		 r + o,	 r, r + o,
         r - o,	-r,-r - o,
         r + o,	 r,-r + o,
         
		 r - o,	-r,-r - o,
         r + o,	 r, r + o,
         r - o,	-r, r - o,
         
		 r + o,	 r, r + o,
         r + o,	 r,-r + o,
        -r + o,	 r,-r + o,
         
		 r + o,	 r, r + o,
        -r + o,	 r,-r + o,
        -r + o,	 r, r + o,
         
		 r + o,	 r, r + o,
        -r + o,	 r, r + o,
         r - o,	-r, r - o, //12*3

    };

		static const GLfloat g_shape4_buffer_data[] = {
        -r ,	-r,		-r, // floor : begin
         r,		-r,		-r,
		-r ,	-r,		 r, 
        
        -r,		-r,		 r,
		 r,		-r,		-r, 
		 r,		-r,		 r, //floor: end

		 r,		-r,		-r, //side 1
		-r,		-r,		-r,
		 0,		 r,		 0,

		 r,		-r,		 r,
		 r,		-r,		-r,
		 0,		 r,		 0,

		-r,		-r,		 r,
		 r,		-r,		 r,
		 0,		 r,		 0,

		-r,		-r,		-r,
		-r,		-r,		 r,
		 0,		 r,		 0
    };

		static const GLfloat g_shape5_buffer_data[] = {
        -r ,	 r,		-r, // floor : begin
         r,		 r,		-r,
		-r ,	 r,		 r, 
        
        -r,		 r,		 r,
		 r,		 r,		-r, 
		 r,		 r,		 r, //floor: end

		 r,		 r,		-r, //side 1
		-r,		 r,		-r,
		 0,		-r,		 0,

		 r,		 r,		 r,
		 r,		 r,		-r,
		 0,		-r,		 0,

		-r,		 r,		 r,
		 r,		 r,		 r,
		 0,		-r,		 0,

		-r,		 r,		-r,
		-r,		 r,		 r,
		 0,		-r,		 0

    };

		static const GLfloat g_shape6_buffer_data[] = {
        -r ,	 r,		-r, // floor : begin
         r,		 r,		-r,
		-r ,	 r,		 r, 
        
        -r,		 r,		 r,
		 r,		 r,		-r, 
		 r,		 r,		 r, //floor: end

		 r,		 r,		-r, //side 1
		-r,		 r,		-r,
		 0,		-r,		 0,

		 r,		 r,		 r,
		 r,		 r,		-r,
		 0,		-r,		 0,

		-r,		 r,		 r,
		 r,		 r,		 r,
		 0,		-r,		 0,

		-r,		 r,		-r,
		-r,		 r,		 r,
		 0,		-r,		 0,

        -r ,	-r,		-r, // floor : begin
         r,		-r,		-r,
		-r ,	-r,		 r, 
        
        -r,		-r,		 r,
		 r,		-r,		-r, 
		 r,		-r,		 r, //floor: end

		 r,		-r,		-r, //side 1
		-r,		-r,		-r,
		 0,		 r,		 0,

		 r,		-r,		 r,
		 r,		-r,		-r,
		 0,		 r,		 0,

		-r,		-r,		 r,
		 r,		-r,		 r,
		 0,		 r,		 0,

		-r,		-r,		-r,
		-r,		-r,		 r,
		 0,		 r,		 0

    };


	static const GLfloat red_buffer_data[] = {
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		0.8f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f
	};

	static const GLfloat blue_buffer_data[] = {

		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  8.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
		0.0f,	 0.0f,	  1.0f,
	};

	static const GLfloat pink_buffer_data[] = {
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	0.6f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		1.0f,	0.0f,	1.0f,
		
	};

	static const GLfloat orange_buffer_data[] = {
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.4f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f,
		1.0f,	0.3f,	0.0f
	};

	static const GLfloat green_buffer_data[] = {

		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 0.8f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
		0.0f,	 1.0f,	  0.0f,
	};

	static const GLfloat yellow_buffer_data[] = {
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 6.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
		1.0f,	 1.0f,	  0.0f,
	};

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &zerobuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, zerobuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_zero_buffer_data), g_zero_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &onebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, onebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_one_buffer_data), g_one_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
		
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &twobuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, twobuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_two_buffer_data), g_two_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &threebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, threebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_three_buffer_data), g_three_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &fourbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, fourbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_four_buffer_data), g_four_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &fivebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, fivebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_five_buffer_data), g_five_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

     // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &sixbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, sixbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_six_buffer_data), g_six_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

     // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &sevenbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, sevenbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_seven_buffer_data), g_seven_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &eightbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, eightbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_eight_buffer_data), g_eight_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &ninebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, ninebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_nine_buffer_data), g_nine_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape1buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape1buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape1_buffer_data), g_shape1_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape2buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape2buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape2_buffer_data), g_shape2_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape3buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape3buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape3_buffer_data), g_shape3_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape4buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape4buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape4_buffer_data), g_shape4_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape5buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape5buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape5_buffer_data), g_shape5_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &shape6buffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, shape6buffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_shape6_buffer_data), g_shape6_buffer_data, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);


    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &colorbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    // Give our vertices to OpenGL.
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &redbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(red_buffer_data), red_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &bluebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, bluebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(blue_buffer_data), blue_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &yellowbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, yellowbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(yellow_buffer_data), yellow_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &greenbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(green_buffer_data), green_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &orangebuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, orangebuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(orange_buffer_data), orange_buffer_data, GL_STATIC_DRAW);

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &pinkbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, pinkbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(pink_buffer_data), pink_buffer_data, GL_STATIC_DRAW);

}

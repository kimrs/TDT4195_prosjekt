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
int length = 16;


// This will be used with shader
//GLuint VertexArrayID;
GLuint vertexbuffer, colorbuffer, redbuffer, bluebuffer, yellowbuffer, greenbuffer;
GLuint objvertexbuffer; // for obj
GLuint programID_1, programID_2;

int last_time, current_time;

GLuint MatrixID; // Handler Matrix for moving the cam
glm::mat4 MVP, M; // FInal Homogeneous Matrix

glm::mat4 MVPUnit;
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
        RenderScene(&DATA[0], length);
    }
        break;
	}
	
    glutSwapBuffers();
    // All drawing commands applied to the
    // hidden buffer, so now, bring forward
    // the hidden buffer and hide the visible one
}


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
    Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
    // Use our shader
    glUseProgram(programID_1);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);

	for(int i = 0; i < length / 4; i++)
	{
		M = glm::scale(glm::mat4(1.0f),glm::vec3(
			*(data + (4 * i + S)),
			1.0,
			*(data + (4 * i + S))));

		M = glm::translate(M,glm::vec3(
			*(data + (4 * i + X))
			, 0.0, 
			*(data + (4 * i + Y))));
		
		MVPUnit = MVP * M;
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVPUnit[0][0]);
		
		int color = *(data + (4 * i + C));
		switch(color)
		{
		case 0:
		{
			glBindBuffer(GL_ARRAY_BUFFER, yellowbuffer);
		}
			break;
		case 1:
		{
			glBindBuffer(GL_ARRAY_BUFFER, redbuffer);
		}
			break;
		case 2:
		{
			glBindBuffer(GL_ARRAY_BUFFER, bluebuffer);
		}
			break;
		case 3:
		{
			glBindBuffer(GL_ARRAY_BUFFER, greenbuffer);
		}
			break;
		}


		glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
            );

		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles
	}

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


void SetupGL() //
{
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
	double radius = RADIUS;
    static const GLfloat g_vertex_buffer_data[] = {
        -radius,	-radius, -radius, // triangle 1 : begin
        -radius,	-radius, radius,
        -radius,	 radius, radius, // triangle 1 : end
        
		 radius,	 radius,-radius, // triangle 2 : begin
        -radius,	-radius,-radius,
        -radius,	 radius,-radius, // triangle 2 : end
         
		 radius,	-radius, radius,
        -radius,	-radius,-radius,
         radius,	-radius,-radius,
         
		 radius,	 radius,-radius,
         radius,	-radius,-radius,
        -radius,	-radius,-radius,
        
		-radius,	-radius,-radius,
        -radius,	 radius, radius,
        -radius,	 radius,-radius,
        
		 radius,	-radius, radius,
        -radius,	-radius, radius,
        -radius,	-radius,-radius,
        
		-radius,	 radius, radius,
        -radius,	-radius, radius,
         radius,	-radius, radius,
        
		 radius,	 radius, radius,
         radius,	-radius,-radius,
         radius,	 radius,-radius,
         
		 radius,	-radius,-radius,
         radius,	 radius, radius,
         radius,	-radius, radius,
         
		 radius,	 radius, radius,
         radius,	 radius,-radius,
        -radius,	 radius,-radius,
         
		 radius,	 radius, radius,
        -radius,	 radius,-radius,
        -radius,	 radius, radius,
         
		 radius,	 radius, radius,
        -radius,	 radius, radius,
         radius,	-radius, radius, //12*3

    };

	static const GLfloat red_buffer_data[] = {

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
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f,
		1.0f,	 0.0f,	  0.0f
	};

	static const GLfloat blue_buffer_data[] = {

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

	static const GLfloat green_buffer_data[] = {

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
		0.0f,	 1.0f,	  0.0f,
	};

	static const GLfloat yellow_buffer_data[] = {
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
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
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

}

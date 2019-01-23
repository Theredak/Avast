#pragma once
#include <windows.h>

//
#include "ShaderProgram.h"
#include "Mesh.h"
#include "MeshGroup.h"
#include "Texture.h"
#include "Camera.h"
#include "Random.h"
#include "Math.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "Timer.h"

#define WINDOW_WIDTH			1200
#define WINDOW_HEIGHT			800
#define FRAMES_PER_SECOND		60

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void reinitializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void keyboardSpecialUp(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	// Linear interpolation
	float lerp(float v0, float v1, float t)
	{
		return (1.0f - t) * v0 + (t)* v1;
	}

	float getRandomSpeed(float min, float max);

	void drawBindMesh(Mesh mesh);
	void checkPlayerCollisionZ();
	void checkPlayerCollisionX();
	void checkPlayerCollisionY();
	void checkPlayerHit();
	void checkPlayerHitPlayer();
	void moveProjectiles();
	void playerMove();
	void playerShoot();
	void checkPlayerDead();
	void player1Pop();
	void player2Pop();

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

	bool inMenu = true;
	bool inPlayButt = false;
	bool gameOver = false;
	float deltaTime;
	float speed[10];
	int player1Health = 12;
	int player2Health = 12;

	int mX;
	int mY;

	vec3 player1Pos;
	vec3 player2Pos;

	vec3 p1Aim;
	vec3 p2Aim;

	Camera camera;
	MeshGroup player1;
	Mesh player2;
	Mesh cannon[10];
	Mesh cBallPile;
	Mesh barrel;
	Mesh pistol;
	Mesh p1Bullet;
	Mesh p2Bullet;
	Mesh Ship;
	Mesh water;
	Mesh light1;
	Mesh lantern1;
	Mesh logo;
	Mesh playBut;
	Mesh settingBut;
	Mesh p1Win;
	Mesh p2Win;	
	Texture p1Sign;
	Texture p2Sign;
	Texture Logo;
	Texture PlayBut;
	Texture SettingBut;
	Texture wood;
	Texture Water;
	Texture Player;
	Texture Player2;
	Texture cBall;
	Texture Barrel;
	ShaderProgram shaderAni;
	ShaderProgram shader;
	ShaderProgram shader2;
	ShaderProgram passThrough;
	ShaderProgram ocean;

	vec3 trans = vec3(0.0f, 100.0f, 5.0f);
	mat4 cameraTransform;
	mat4 cameraProjection;
	
	KeyInput input;

private:

};

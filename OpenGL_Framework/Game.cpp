#include "Game.h"
#include "Transform.h"
#include <iostream>
#include <fstream>
#include <random>

vec4 fogColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);

Game::Game()
{
}

Game::~Game()
{
	delete updateTimer;

	passThrough.unLoad();
	p1Win.unload();
	p2Win.unload();
	logo.unload();
	playBut.unload();
	settingBut.unload();
	player1.unload();	
	player2.unload();
	Ship.unload();
	wood.unload();
	Water.unload();
	Player.unload();
	Player2.unload();
	for (int i = 0; i < 10; i++)
	{
		cannon[i].unload();
	}
}

void Game::initializeGame()
{
	updateTimer = new Timer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//loading player models
	std::vector<std::string> player1Files;
	for (int c = 0; c < 4; ++c)
	{
		player1Files.push_back("Assets/Models/P1Ani/P1model" + std::to_string(c) + ".obj");
	}
	player1.loadFiles(player1Files);
	player2.loadFromFile("Assets/Models/P1Ani/P1model0.obj");
		
	//loading shaders
	passThrough.load("Assets/Shaders/passThrought.vert", "Assets/Shaders/passThrought.frag");
	shader.load("Assets/Shaders/shader.vert", "Assets/Shaders/shader.frag");
	shader2.load("Assets/Shaders/shader2.vert", "Assets/Shaders/shader2.frag");
	ocean.load("Assets/Shaders/Water.vert", "Assets/Shaders/Water.frag");
	shaderAni.load("Assets/Shaders/shaderAni.vert", "Assets/Shaders/passThrought.frag");

	//Loading of the model objs for scene
	logo.loadFromFile("Assets/Models/logo.obj");
	playBut.loadFromFile("Assets/Models/playSign.obj");
	settingBut.loadFromFile("Assets/Models/button.obj");
	cBallPile.loadFromFile("Assets/Models/CannonballPyramid.obj");
	barrel.loadFromFile("Assets/Models/Barrel.obj");
	p1Win.loadFromFile("Assets/Models/endSign.obj");
	p2Win.loadFromFile("Assets/Models/endSign.obj");			
	Ship.loadFromFile("Assets/Models/Pirate_Ship_2.1.obj");
	water.loadFromFile("Assets/Models/Ground.obj");
	light1.loadFromFile("Assets/Models/Cannonball.obj");
	lantern1.loadFromFile("Assets/Models/Pirate_Lantern.obj");
	p1Bullet.loadFromFile("Assets/Models/Cannonball.obj");
	p2Bullet.loadFromFile("Assets/Models/Cannonball.obj");
	for (int i = 0; i < 10; i++)
	{
		cannon[i].loadFromFile("Assets/Models/Cannonball.obj");
	}
	
	//loading textures
	Logo.load("logo.png");
	PlayBut.load("play.png");
	SettingBut.load("settings.png");
	wood.load("House.png");
	Water.load("water2K2.png");
	Player.load("player2.png");
	Player2.load("player2.png");
	cBall.load("iron.png");
	Barrel.load("barrel.png");
	p1Sign.load("p1Win.png");
	p2Sign.load("p2Win.png");

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
			
	cameraTransform.Translate(trans);	
	cameraProjection = mat4::PerspectiveProjection(60.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 10000.0f);
		
	for (int i = 0; i < 10; i++)
	{
		speed[i] = getRandomSpeed(15, 25);
	}

	//setting object starting positions and rotations			
	logo.m_pLocalPosition = vec3(0, 100, -6);
	playBut.m_pLocalPosition = vec3(0, 99, -5);
	settingBut.m_pLocalPosition = vec3(0, 96, -5);
	p1Win.m_pLocalPosition = vec3(0, -50, 0);
	p2Win.m_pLocalPosition = vec3(0, -50, 0);
	light1.setScale(0.1f);	
	lantern1.m_pLocalPosition = vec3(-5, 0, -5);
	light1.m_pLocalPosition = vec3(-5, -1, -5);
	player1.m_pLocalPosition = vec3(-12, -15, -6);
	player2.m_pLocalPosition = vec3(12, -15, -6);
	player1Pos = vec3(-12, -8.0f, -6);
	player2Pos = vec3(12, -8.0f, -6);
	p1Aim = vec3(player1Pos.x, -8.0f, player1Pos.z + 10.0f);
	p2Aim = vec3(player2Pos.x, -8.0f, player2Pos.z + 10.0f);
	p1Bullet.m_pLocalPosition = vec3(player1Pos.x, p1Aim.y, player1Pos.z);
	p2Bullet.m_pLocalPosition = vec3(player2Pos.x, p2Aim.y, player2Pos.z);
	p1Bullet.setScale(0.1f);
	p2Bullet.setScale(0.1f);
	pistol.m_pLocalPosition = vec3(2, 2, 0);
	barrel.m_pLocalPosition = vec3(-13, -9, -10);
	cBallPile.m_pLocalPosition = vec3(14, -9, -10);
	pistol.setScale(0.5);
	cannon[0].m_pLocalPosition = vec3(-11, -9, 10);
	cannon[1].m_pLocalPosition = vec3(-8, -9, -30);
	cannon[2].m_pLocalPosition = vec3(-5, -9, 10);
	cannon[3].m_pLocalPosition = vec3(-3, -9, -30);
	cannon[4].m_pLocalPosition = vec3(0, -9, 10);
	cannon[5].m_pLocalPosition = vec3(3, -9, -30);
	cannon[6].m_pLocalPosition = vec3(5, -9, 10);
	cannon[7].m_pLocalPosition = vec3(8, -9, -30);
	cannon[8].m_pLocalPosition = vec3(11, -9, 10);
	cannon[9].m_pLocalPosition = vec3(14, -9, -30);
	Ship.m_pLocalPosition = vec3(0, -10, -10);
	water.m_pLocalPosition = vec3(0, -12, -18);
	player1.setScale(0.3f);
	player2.setScale(0.3f);
	logo.setScale(3.5f);;
	barrel.setScale(0.5f);
	logo.setRotationAngleX(90.0f);
	playBut.setRotationAngleX(90.0f);
	settingBut.setRotationAngleX(90.0f);
	p1Win.setRotationAngleX(40.0f);
	p2Win.setRotationAngleX(40.0f);
	water.setRotationAngleX(5.0f);
	water.setScale(3.0f);
}

//used to restart game for easy replayability
void Game::reinitializeGame()//called when a player has won
{
	cameraTransform.Translate(vec3(0, 103, -3));
	player1.m_pLocalPosition = vec3(-12, -15, -6);
	player2.m_pLocalPosition = vec3(12, -15, -6);
	player1Pos = vec3(-12, -15, -6);
	player2Pos = vec3(12, -15, -6);
	barrel.m_pLocalPosition = vec3(-13, -9, -10);
	cBallPile.m_pLocalPosition = vec3(14, -9, -10);
	p1Win.m_pLocalPosition = vec3(0, -50, 0);
	p2Win.m_pLocalPosition = vec3(0, -50, 0);
	player1Health = 12;
	player2Health = 12;
}

void Game::update()
{	
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;	

	if (inMenu == false)//checks if in the menu
	{// if not the gamplay starts
		checkPlayerCollisionZ();
		checkPlayerCollisionX();
		checkPlayerCollisionY();
		checkPlayerHit();
		checkPlayerHitPlayer();
		
		playerMove();
		playerShoot();
		//moveProjectiles();
		checkPlayerDead();
		player1Pop();
		player2Pop();

		if(input.P1Shoot == false)
			p1Bullet.m_pLocalPosition = player1Pos;
		if (input.P2Shoot == false)
			p2Bullet.m_pLocalPosition = player2Pos;
		//lantern1.m_pLocalPosition = p2Aim;

		p1Win.update(deltaTime);
		p2Win.update(deltaTime);		

		player1.update(deltaTime);
		player2.update(deltaTime);
		barrel.update(deltaTime);
		cBallPile.update(deltaTime);
		pistol.update(deltaTime);
		Ship.update(deltaTime);
		water.update(deltaTime);
		light1.update(deltaTime);
		lantern1.update(deltaTime);
		p1Bullet.update(deltaTime);
		p2Bullet.update(deltaTime);
		for (int i = 0; i < 10; i++)
		{
			cannon[i].update(deltaTime);
		}
	}
	logo.update(deltaTime);
	playBut.update(deltaTime);
	settingBut.update(deltaTime);
}

void Game::draw()
{
	glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vec4 lightPos = cameraTransform.GetInverse() * vec4(light1.m_pLocalPosition, 1.0f);
	//vec4 lightPos2 = cameraTransform.GetInverse() * vec4(lantern1.m_pLocalPosition, 1.0f);
			
	static float aniTimer = 0.f;
	static int index = 0;
	aniTimer += updateTimer->getElapsedTimeSeconds();
	if (aniTimer > 1.0f)
	{
		aniTimer = 0.0f;
		index = (index + 1) % 4;
	}	
	shaderAni.bind();
	shaderAni.sendUniformMat4("uModel", player1.getLocalToWorldMatrix().data, false);
	shaderAni.sendUniformMat4("uView", cameraTransform.GetInverse().data, false);
	shaderAni.sendUniformMat4("uProj", cameraProjection.data, false);
	shaderAni.sendUniform("interp", aniTimer);
	shaderAni.sendUniform("index", index);
	shaderAni.sendUniform("uLightPosition", vec3(lightPos));

	Player.bind();
	player1.bind();
	player1.draw();
	player1.unbind();
	//drawBindMesh(player1);
	Player.unbind();
	shaderAni.unBind();

	shader.bind();
	shader.sendUniformMat4("uModel", light1.getLocalToWorldMatrix().data, false);
	shader.sendUniformMat4("uView", cameraTransform.GetInverse().data, false);
	shader.sendUniformMat4("uProj", cameraProjection.data, false);
	shader.sendUniform("uLightPosition", vec3(lightPos));
		
	drawBindMesh(light1);
	shader.unBind();
	
	passThrough.bind();
	passThrough.sendUniformMat4("uView", cameraTransform.GetInverse().data, false);
	passThrough.sendUniformMat4("uProj", cameraProjection.data, false);
	passThrough.sendUniform("uLightPosition", vec3(lightPos));	
			
	p1Sign.bind();
	drawBindMesh(p1Win);
	p1Sign.unbind();

	p2Sign.bind();
	drawBindMesh(p2Win);
	p2Sign.unbind();
	
	Logo.bind();
	drawBindMesh(logo);
	Logo.unbind();

	PlayBut.bind();
	drawBindMesh(playBut);
	PlayBut.unbind();

	SettingBut.bind();
	drawBindMesh(settingBut);
	SettingBut.unbind();	

	Barrel.bind();
	drawBindMesh(barrel);
	Barrel.unbind();

	drawBindMesh(p1Bullet);
	drawBindMesh(p2Bullet);
		
	Player2.bind();
	drawBindMesh(player2);
	Player2.unbind();

	cBall.bind();
	drawBindMesh(lantern1); //Lantern: TEMPORARY TEXTURE
	drawBindMesh(cBallPile);
	for (int i = 0; i < 10; i++)
	{
		drawBindMesh(cannon[i]);
	}	
	cBall.unbind();

	wood.bind(0);
	drawBindMesh(Ship);
	wood.unbind(0);
	passThrough.unBind();	

	ocean.bind();
	ocean.sendUniformMat4("uModel", water.getLocalToWorldMatrix().data, false);
	ocean.sendUniformMat4("uView", cameraTransform.GetInverse().data, false);
	ocean.sendUniformMat4("uProj", cameraProjection.data, false);
	ocean.sendUniform("uLightPosition", vec3(lightPos));
	ocean.sendUniform("uTime", TotalGameTime);	

	Water.bind();
	drawBindMesh(water);
	Water.unbind();
	ocean.unBind();
	
	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	
	switch(key)
	{
	case 27: // the escape key
		exit(1);
		break;
	case 'q': // the 'q' key	
		//player1.m_pLocalPosition.y += 10 * deltaTime;
		break;
	case 'e': // the 'e' key	
		input.P1Shoot = true;
		break;
	case 'u': // the 'u' key	
		input.P2Shoot = true;
		break;
	case 'd':		
		input.P1Right = true;
		break;
	case 'a':		
		input.P1Left = true;
		break;
	case 'w':
		input.P1Forward = true;
		break;
	case 's':
		input.P1Backward = true;
		break;	
	case 'l':
		input.moveRight = true;
		break;
	case 'j':
		input.moveLeft = true;
		break;
	case 'i':
		input.moveForward = true;
		break;
	case 'k':
		input.moveBackward = true;
		break;
	case '8'://key up
		light1.m_pLocalPosition.z -= 6.0f * deltaTime;
		break;
	case '2'://key down
		light1.m_pLocalPosition.z += 6.0f * deltaTime;
		break;
	case '6'://key right
		light1.m_pLocalPosition.x += 6.0f * deltaTime;
		break;
	case '4'://key left
		light1.m_pLocalPosition.x -= 6.0f * deltaTime;
		break;
	case '7'://key forward
		light1.m_pLocalPosition.y -= 6.0f * deltaTime;
		break;
	case '9'://key backward
		light1.m_pLocalPosition.y += 6.0f * deltaTime;
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32:
		break;
	case 'p':
		if (inMenu)
		{
			cameraTransform.Translate(vec3(0, -103, 3));
			cameraTransform.RotateX(-40.0f);
			inMenu = false;
		}
		break;
	case 'r':
		if (!inMenu && gameOver)
		{
			cameraTransform.RotateX(40.0f);
			reinitializeGame();
			inMenu = true;
			gameOver = false;
		}		
		break;
	case 'e':
		//input.P1Shoot == false;
		break;
	case 'd':
		input.P1Right = false;
		break;
	case 'a':
		input.P1Left = false;
		break;
	case 'w':
		input.P1Forward = false;
		break;
	case 's':
		input.P1Backward = false;
		break;
	case 'l':
		input.moveRight = false;
		break;
	case 'j':
		input.moveLeft = false;
		break;
	case 'i':
		input.moveForward = false;
		break;
	case 'k':
		input.moveBackward = false;
		break;
	case 27: // the escape key
	case 'q': // the 'q' key
		//exit(1);
		break;
	}
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_F5:
		//passThrough.reload();
		break;
	case GLUT_KEY_UP:
		//cameraTransform.Translate(vec3(0, 1, 0));
		break;
	case GLUT_KEY_DOWN:
		//cameraTransform.Translate(vec3(0, -1, 0));
		break;
	case GLUT_KEY_RIGHT:
		//cameraTransform.Translate(vec3(1, 0, 0));
		break;
	case GLUT_KEY_LEFT:
		//cameraTransform.Translate(vec3(-1, 1, 0));
		break;
	}
}

void Game::keyboardSpecialUp(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		//input.moveForward = false;
		break;
	case GLUT_KEY_DOWN:
		//input.moveBackward = false;
		break;
	case GLUT_KEY_RIGHT:
		//input.moveRight = false;
		break;
	case GLUT_KEY_LEFT:
		//input.moveLeft = false;
		break;
	case GLUT_KEY_PAGE_UP:
		//input.moveUp = false;
		break;
	case GLUT_KEY_PAGE_DOWN:
		//input.moveDown = false;
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{	
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:
			
			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}

float Game::getRandomSpeed(float min, float max)
{
	return RandomRangef(min, max);	
}

void Game::drawBindMesh(Mesh mesh)
{	
	passThrough.sendUniformMat4("uModel", mesh.getLocalToWorldMatrix().data, false);
	mesh.bind();
	mesh.draw();
	mesh.unbind();
}

void Game::checkPlayerCollisionZ()
{
	if (player1.m_pLocalPosition.z < -12)
	{
		input.P1Forward = false;
	}
	if (player1.m_pLocalPosition.z > -5.5f)
	{
		input.P1Backward = false;
	}
	if (player2.m_pLocalPosition.z < -12)
	{
		input.moveForward = false;
	}
	if (player2.m_pLocalPosition.z > -5.5f)
	{
		input.moveBackward = false;
	}
}

void Game::checkPlayerCollisionX()
{
	if (player1.m_pLocalPosition.x < -12)
	{
		input.P1Left = false;
	}
	if (player1.m_pLocalPosition.x > 13)
	{
		input.P1Right = false;
	}
	if (player2.m_pLocalPosition.x < -12)
	{
		input.moveLeft = false;
	}
	if (player2.m_pLocalPosition.x > 13)
	{
		input.moveRight = false;
	}
}

void Game::checkPlayerCollisionY()
{
	if (player1.m_pLocalPosition.y < -9)
	{
		player1.m_pLocalPosition.y = -9;
	}
	if (player2.m_pLocalPosition.y < -9)
	{
		player2.m_pLocalPosition.y = -9;
	}
}

void Game::checkPlayerHit()
{
	for (int i = 0; i < 10; i++)
	{
		if (player1.m_pLocalPosition.x + 0.3f > (cannon[i].m_pLocalPosition.x - 0.4f) &&
			player1.m_pLocalPosition.x - 0.3f < (cannon[i].m_pLocalPosition.x + 0.4f) &&
			player1.m_pLocalPosition.z - 0.3f < (cannon[i].m_pLocalPosition.z + 0.4f) &&
			player1.m_pLocalPosition.z + 0.3f >(cannon[i].m_pLocalPosition.z - 0.4f))
		{
			std::cout << "player1 hit" << std::endl;
			player1Health--;
			player1.m_pLocalPosition.y += 30.0f * deltaTime;
		}
		if (player2.m_pLocalPosition.x + 0.3f > (cannon[i].m_pLocalPosition.x - 0.4f) &&
			player2.m_pLocalPosition.x - 0.3f < (cannon[i].m_pLocalPosition.x + 0.4f) &&
			player2.m_pLocalPosition.z - 0.3f < (cannon[i].m_pLocalPosition.z + 0.4f) &&
			player2.m_pLocalPosition.z + 0.3f >(cannon[i].m_pLocalPosition.z - 0.4f))
		{
			std::cout << "player2 hit" << std::endl;
			player2Health--;
			player2.m_pLocalPosition.y += 30.0f * deltaTime;
		}
	}
}

void Game::checkPlayerHitPlayer()
{
	if (player1.m_pLocalPosition.x + 0.1f > player2.m_pLocalPosition.x - 0.1f &&
		player1.m_pLocalPosition.x - 0.1f < player2.m_pLocalPosition.x + 0.1f &&
		player1.m_pLocalPosition.z + 0.2f < player2.m_pLocalPosition.z - 0.2f &&
		player1.m_pLocalPosition.z - 0.2f < player2.m_pLocalPosition.z + 0.2f)
	{
		std::cout << "Both players hit!" << std::endl;
		player1Health--;
		player2Health--;
	}
}

void Game::moveProjectiles()
{
		for (int i = 0; i < 10; i += 2)
		{
			if (cannon[i].m_pLocalPosition.z < 0)
			{
				cannon[i].m_pLocalPosition.z += speed[i] * deltaTime;// make float value random
			}
			else
			{
				cannon[i].m_pLocalPosition.z = -30.0f;
			}
		}
		for (int j = 1; j < 10; j += 2)
		{
			if (cannon[j].m_pLocalPosition.z > -30)
			{
				cannon[j].m_pLocalPosition.z -= speed[j] * deltaTime;// make float value random
			}
			else
			{
				cannon[j].m_pLocalPosition.z = 0.0f;
			}
		}		
}

void Game::playerMove()
{
	if (input.moveForward)
	{
		player2Pos.z -= 3.0f * deltaTime;
		player2.setRotationAngleY(180.0f);
		p2Aim = vec3(player2Pos.x, -8.0f, player2Pos.z - 10.0f);		
	}
	if (input.moveBackward)
	{
		player2Pos.z += 3.0f * deltaTime;
		player2.setRotationAngleY(0.0f);
		p2Aim = vec3(player2Pos.x, -8.0f, player2Pos.z + 10.0f);
	}
	if (input.moveRight)
	{
		player2Pos.x += 3.0f * deltaTime;
		player2.setRotationAngleY(90.0f);
		p2Aim = vec3(player2Pos.x + 10.0f, -8.0f, player2Pos.z);
	}
	if (input.moveLeft)
	{
		player2Pos.x -= 3.0f * deltaTime;
		player2.setRotationAngleY(-90.0f);
		p2Aim = vec3(player2Pos.x - 10.0f, -8.0f, player2Pos.z);
	}
	if (input.P1Forward)
	{
		player1Pos.z -= 3.0f * deltaTime;
		player1.setRotationAngleY(180.0f);
		p1Aim = vec3(player1Pos.x, -8.0f, player1Pos.z - 10.0f);
	}
	if (input.P1Backward)
	{
		player1Pos.z += 3.0f * deltaTime;
		player1.setRotationAngleY(0.0f);
		p1Aim = vec3(player1Pos.x, -8.0f, player1Pos.z + 10.0f);
	}
	if (input.P1Right)
	{
		player1Pos.x += 3.0f * deltaTime;
		player1.setRotationAngleY(90.0f);
		p1Aim = vec3(player1Pos.x + 10.0f, -8.0f, player1Pos.z);
	}
	if (input.P1Left)
	{
		player1Pos.x -= 3.0f * deltaTime;
		player1.setRotationAngleY(-90.0f);
		p1Aim = vec3(player1Pos.x - 10.0f, -8.0f, player1Pos.z);
	}
	for (float i = 0; i < 1.1f; i += 0.1f)
	{
		player1.m_pLocalPosition.x = lerp(player1.m_pLocalPosition.x, player1Pos.x, i * deltaTime);
		player1.m_pLocalPosition.z = lerp(player1.m_pLocalPosition.z, player1Pos.z, i * deltaTime);
		player2.m_pLocalPosition.x = lerp(player2.m_pLocalPosition.x, player2Pos.x, i * deltaTime);
		player2.m_pLocalPosition.z = lerp(player2.m_pLocalPosition.z, player2Pos.z, i * deltaTime);
	}
}

void Game::playerShoot()
{
	if (input.P1Shoot == true)
	{			
		for (float i = 0; i < 1.1f; i += 0.1f) {
			p1Bullet.m_pLocalPosition.x = lerp(p1Bullet.m_pLocalPosition.x, p1Aim.x, i * deltaTime);
			p1Bullet.m_pLocalPosition.z = lerp(p1Bullet.m_pLocalPosition.z, p1Aim.z, i * deltaTime);
		}		
	}
	if (input.P2Shoot == true)
	{
		for (float i = 0; i < 1.1f; i += 0.1f) {
			p2Bullet.m_pLocalPosition.x = lerp(p2Bullet.m_pLocalPosition.x, p2Aim.x, i * deltaTime);
			p2Bullet.m_pLocalPosition.z = lerp(p2Bullet.m_pLocalPosition.z, p2Aim.z, i * deltaTime);
		}
	}
}

void Game::checkPlayerDead()
{
	if (player1Health == 0)
	{
		//std::cout << "Player1 is dead" << std::endl;
		player1.m_pLocalPosition = vec3(0, -50, 0);
		p2Win.m_pLocalPosition = vec3(0, 0, 0);
		gameOver = true;
	}
	if (player2Health == 0)
	{
		//std::cout << "Player2 is dead" << std::endl;
		player2.m_pLocalPosition = vec3(0, -50, 0);
		p1Win.m_pLocalPosition = vec3(0, 0, 0);
		gameOver = true;
	}
}

void Game::player1Pop()
{
	if (player1.m_pLocalPosition.y == -12)
	{
		player1.m_pLocalPosition.y = -12;
	}
	else
	{
		player1.m_pLocalPosition.y -= 6.0f * deltaTime;
	}
}

void Game::player2Pop()
{
	if (player2.m_pLocalPosition.y == -12)
	{
		player2.m_pLocalPosition.y = -12;
	}
	else
	{
		player2.m_pLocalPosition.y -= 6.0f * deltaTime;
	}
}

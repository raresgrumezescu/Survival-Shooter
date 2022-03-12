#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <ctime>
#include <cstdlib>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;



Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;       // logic x
	logicSpace.y = 0;       // logic y
	logicSpace.width = 4;   // logic width
	logicSpace.height = 4;  // logic height

	enemie.x = 0;
	enemie.y = 0;
	enemie.rx = 0;
	enemie.lx = 0;
	enemie.ry = 0;
	enemie.ly = 0;

	healthbar.deltaX = 0;
	healthbar.y = 1.5f;
	healthbar.count = 5;
	healthbar.length = 0.1f;
	healthbar.width = 0.5f;
	healthbar.X.push_back(1.3f);
	healthbar.X.push_back(1.4f);
	healthbar.X.push_back(1.5f);
	healthbar.X.push_back(1.6f);
	healthbar.X.push_back(1.7f);

	
	avatar.plx = 1;
	avatar.ply = 1;
	avatar.rx = 1.13f;
	avatar.ry = 1.2f;
	avatar.lx = 1.02f;
	avatar.ly = 1.2f;
	avatar.length = 0.2f;
	avatar.lengthArm = 0.05f;

	glm::vec3 corner = glm::vec3(0.01, 0.01, 0);
	length = 0.2f;
	length_glonte = 0.05f;
	length_inamic = 0.2f;
	length_brat = 0.05f;
	length_life = 0.05f;

	Mesh* square1 = object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0), true);
	AddMeshToList(square1);

	Mesh* square2 = object2D::CreateSquare("square2", corner, length, glm::vec3(0, 1, 0), true);
	AddMeshToList(square2);

	Mesh* inamic = object2D::CreateSquare("inamic", corner, length_inamic, glm::vec3(0, 1, 1), true);
	AddMeshToList(inamic);

	Mesh* brat = object2D::CreateSquare("brat", corner, length_brat, glm::vec3(1, 1, 0), true);
	AddMeshToList(brat);

	Mesh* glonte = object2D::CreateSquare("glonte", corner, length_glonte, glm::vec3(0, 0, 1), true);
	AddMeshToList(glonte);

	Mesh* health1 = object2D::CreateSquare("health1", corner, healthbar.length, glm::vec3(1, 0.5, 1));
	AddMeshToList(health1);

	Mesh* health2 = object2D::CreateSquare("health2", corner, healthbar.length, glm::vec3(1, 0.5, 1), true);
	AddMeshToList(health2);

	Mesh* life = object2D::CreateSquare("life", corner, length_life, glm::vec3(0.5, 0.5, 0.5), true);
	AddMeshToList(life);
}


// 2D visualization matrix
glm::mat3 Tema1::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}


// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}


void Tema1::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}


void Tema1::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema1::Update(float deltaTimeSeconds)
{
	totalTime += 0.005f;
	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	//viewSpace = ViewportSpace(0, 0, resolution.x / 2, resolution.y / 2);
	SetViewportArea(viewSpace, glm::vec3(0), true);

	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2D(LogicSpace(lx, ly, lweight, lheight), viewSpace);
	//visMatrix *= VisualizationTransf2D(LogicSpace(logicSpace.x, logicSpace.y, logicSpace.width, logicSpace.height), viewSpace);

	DrawScene(visMatrix, avatar.plx, avatar.ply, itsTimeForAnEnemie);

}


void Tema1::FrameEnd()
{
}


void Tema1::DrawScene(glm::mat3 visMatrix, double plx, double ply, int itsTimeForAnEnemie)
{
	//    modelMatrix = visMatrix * transform2D::Translate(1, 0);
	//    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
	itsTimeForAnEnemie++;
	srand(time(NULL));

	if (lifeReady == false) {
		modelMatrix = visMatrix * transform2D::Translate(b1x, b1y);
		RenderMesh2D(meshes["life"], shaders["VertexColor"], modelMatrix);
	}

	if (enemieReady == true) {
		CreateEnemie();
	}

	if (enemieReady == false) {
		for (int i = 0; i < enemies.size(); i++) {
			enemies[i].x = enemies[i].x + enemies[i].directionX;
			enemies[i].y = enemies[i].y + enemies[i].directionY;
			enemies[i].rx = enemies[i].rx + enemies[i].directionX;
			enemies[i].ry = enemies[i].ry + enemies[i].directionY;
			enemies[i].lx = enemies[i].lx + enemies[i].directionX;
			enemies[i].ly = enemies[i].ly + enemies[i].directionY;

			if (enemies[i].x > avatar.plx && enemies[i].y > avatar.ply) {
				enemies[i].directionX = -(double)(enemies[i].x - avatar.plx) / 100;
				enemies[i].directionY = -(double)(enemies[i].y - avatar.ply) / 100;
			}
			if (enemies[i].x > avatar.plx && enemies[i].y < avatar.ply) {
				enemies[i].directionX = -(double)(enemies[i].x - avatar.plx) / 100;
				enemies[i].directionY = (double)(avatar.ply - enemies[i].y) / 100;
			}
			if (enemies[i].x < avatar.plx && enemies[i].y < avatar.ply) {
				enemies[i].directionX = (double)(avatar.plx - enemies[i].x) / 100;
				enemies[i].directionY = (double)(avatar.ply - enemies[i].y) / 100;
			}
			if (enemies[i].x < avatar.plx && enemies[i].y > avatar.ply) {
				enemies[i].directionX = (double)(avatar.plx - enemies[i].x) / 100;
				enemies[i].directionY = -(double)(enemies[i].y - avatar.ply) / 100;
			}

			modelMatrix = visMatrix * transform2D::Translate(enemies[i].x, enemies[i].y);
			RenderMesh2D(meshes["inamic"], shaders["VertexColor"], modelMatrix);

			modelMatrix = visMatrix * transform2D::Translate(enemies[i].rx, enemies[i].ry);
			RenderMesh2D(meshes["brat"], shaders["VertexColor"], modelMatrix);

			modelMatrix = visMatrix * transform2D::Translate(enemies[i].lx, enemies[i].ly);
			RenderMesh2D(meshes["brat"], shaders["VertexColor"], modelMatrix);
			//std::cout << "aici" << endl;

			if (enemies[i].x <= avatar.plx + 0.3f && enemies[i].x >= avatar.plx - 0.3f && enemies[i].y <= avatar.ply + 0.3f && enemies[i].y >= avatar.ply - 0.3f) {

//				enemies.erase(i);
				enemies.erase(enemies.begin() + i);
				healthbar.count--;

			}
		}
	}

	for (int i = 0; i < gloante.size(); i++) {
		modelMatrix = visMatrix * transform2D::Translate(gloante[i].x, gloante[i].y);
		
		gloante[i].x += gloante[i].directionX * gloante[i].speedUpX;
		gloante[i].y += gloante[i].directionY * gloante[i].speedUpX;
		
		RenderMesh2D(meshes["glonte"], shaders["VertexColor"], modelMatrix);
		
			

		for (int j = 0; j < enemies.size(); j++) {
			if (enemies[j].x <= gloante[i].x + 0.1f && enemies[j].x >= gloante[i].x - 0.1f && enemies[j].y <= gloante[i].y + 0.1f && enemies[j].y >= gloante[i].y - 0.1f) {
				//std::cout << i << endl;
				enemies.erase(enemies.begin() + j);

				gloante.erase(gloante.begin() + i);

			}
		}
	}


	for (int i = 0; i < 5; i++) {
		modelMatrix = visMatrix * transform2D::Translate(healthbar.X[i] + healthbar.deltaX, healthbar.y);
		RenderMesh2D(meshes["health1"], shaders["VertexColor"], modelMatrix);
	}

	for (int i = 0; i < healthbar.count; i++) {
		modelMatrix = visMatrix * transform2D::Translate(healthbar.X[i] + healthbar.deltaX, healthbar.y);
		RenderMesh2D(meshes["health2"], shaders["VertexColor"], modelMatrix);
	}

	modelMatrix = visMatrix * transform2D::Translate(0.2, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.4, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.6, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.8, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 0.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 0.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 0.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 0.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);



	modelMatrix = visMatrix * transform2D::Translate(0.2, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.4, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.4, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.2, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(0.4, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);






	modelMatrix = visMatrix * transform2D::Translate(1, 1.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1, 2.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.2, 1.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.2, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.2, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.2, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.2, 2.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.4, 1.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.4, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.4, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.4, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.4, 2.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.6, 1.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.6, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.6, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.6, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.6, 2.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.8, 1.8);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.8, 2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.8, 2.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.8, 2.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(1.8, 2.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);




	//modelMatrix = visMatrix * transform2D::Translate(plx, ply);
	modelMatrix = visMatrix * transform2D::Translate(avatar.plx, avatar.ply);

	RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(avatar.rx, avatar.ry);
	RenderMesh2D(meshes["brat"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(avatar.lx, avatar.ly);
	RenderMesh2D(meshes["brat"], shaders["VertexColor"], modelMatrix);

	if (lifeOK == true && avatar.plx >= b1x - 0.1f && avatar.plx <= b1x + 0.1f && avatar.ply >= b1y - 0.1f && avatar.ply <= b1y + 0.1f) {
		lifeReady = true;
		if (healthbar.count < 5) healthbar.count++;
		lifeOK = false;
	}
	

	modelMatrix = visMatrix * transform2D::Translate(3, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(2.8, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(3, 0.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(2.8, 0.2);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);



	modelMatrix = visMatrix * transform2D::Translate(2.6, 1.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(2.6, 1.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(2.4, 1.6);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * transform2D::Translate(2.4, 1.4);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::CreateEnemie()
{
		Enemie enemie;
		float directionX, directionY;
		float enemieX = rand() % window->GetResolution().x;
		float enemieY = rand() % window->GetResolution().y;
		enemieX = (double)(4 * enemieX / window->GetResolution().x);
		enemieY = (double)(4 * enemieY / window->GetResolution().y);
		enemie.x = enemieX;
		enemie.y = enemieY;
		enemie.rx = enemie.x + 0.13f;
		enemie.ry = enemie.y + 0.2f;
		enemie.lx = enemie.x + 0.02f;
		enemie.ly = enemie.y + 0.2f;
		if (enemieX > plx && enemieY > ply) {
			directionX = -(double)(enemieX - plx) / 100;
			directionY = -(double)(enemieY - ply) / 100;
		}
		if (enemieX > plx && enemieY < ply) {
			directionX = -(double)(enemieX - plx) / 100;
			directionY = (double)(ply - enemieY) / 100;
		}
		if (enemieX < plx && enemieY < ply) {
			directionX = (double)(plx - enemieX) / 100;
			directionY = (double)(ply - enemieY) / 100;
		}
		if (enemieX < plx && enemieY > ply) {
			directionX = (double)(plx - enemieX) / 100;
			directionY = -(double)(enemieY - ply) / 100;
		}
		enemieReady = false;
		enemies.push_back(enemie);
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	//timeForBullet += 0.01f;
	if (totalTime - timeForBullet >= 0.5f) {
		glonteReady = true;
		timeForBullet = totalTime;
	}

	if (totalTime - timeForEnemie >= 2.0000f) {
		enemieReady = true;
		timeForEnemie = totalTime;
	}

	if (totalTime - timeForLife >= 2.000f) {
		lifeReady = false;
		lifeOK = true;
		//b1x = 1;
		//b1y = 1;
		timeForLife = totalTime;
	}

	if (window->KeyHold(GLFW_KEY_W)) {
		if ((avatar.ply + deltaTime >= 2 - 0.2f && avatar.ply + deltaTime <= 2.4 + 0.2f && avatar.plx >= 0 && avatar.plx <= 0.4 + 0.2f) == false) {
			if ((avatar.ply + deltaTime >= 1.8 - 0.2f && avatar.ply + deltaTime <= 2.6 + 0.2f && avatar.plx >= 1 - 0.2f && avatar.plx <= 1.8 + 0.2f) == false) {
				if ((avatar.ply + deltaTime >= 1.4 - 0.2f && avatar.ply + deltaTime <= 1.6 + 0.2f && avatar.plx >= 2.4 - 0.2f && avatar.plx <= 2.6 + 0.2f) == false) {
					if (avatar.ply + deltaTime * 2 < 4 - 0.2f) {
						ly += deltaTime * 2;
						avatar.ply += deltaTime * 2;
						avatar.ry += deltaTime * 2;
						avatar.ly += deltaTime * 2;
						healthbar.y += deltaTime * 2;
					}
				}
			}
		}
	}

	if (window->KeyHold(GLFW_KEY_S)) {
		if ((avatar.ply - deltaTime >= 2 - 0.2f && avatar.ply - deltaTime <= 2.4 + 0.2f && avatar.plx >= 0 && avatar.plx <= 0.4 + 0.2f) == false) {
			if ((avatar.ply - deltaTime >= 1.8 - 0.2f && avatar.ply - deltaTime <= 2.6 + 0.2f && avatar.plx >= 1 - 0.2f && avatar.plx <= 1.8 + 0.2f) == false) {
				if ((avatar.ply - deltaTime >= 1.4 - 0.2f && avatar.ply - deltaTime <= 1.6 + 0.2f && avatar.plx >= 2.4 - 0.2f && avatar.plx <= 2.6 + 0.2f) == false) {
					if ((avatar.ply - deltaTime >= 0 - 0.2f && avatar.ply - deltaTime <= 0.2 + 0.2f && avatar.plx >= 2.8 - 0.2f && avatar.plx <= 3 + 0.2f) == false) {
						if ((avatar.ply - deltaTime <= 0.2 && avatar.plx >= 0.2 && avatar.plx <= 0.8 + 0.2f) == false) {
							if ((avatar.ply - deltaTime <= 0.8 + 0.2f && avatar.plx >= 0.2 - 0.18f && avatar.plx <= 0.2 + 0.2f) == false) {
								if (avatar.ply - deltaTime * 2 > 0) {
									ly -= deltaTime * 2;
									avatar.ply -= deltaTime * 2;
									avatar.ry -= deltaTime * 2;
									avatar.ly -= deltaTime * 2;
									healthbar.y -= deltaTime * 2;
								}
							}
						}
					}
				}
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		if ((avatar.ply >= 2 - 0.2f && avatar.ply <= 2.4 + 0.2f && avatar.plx - deltaTime >= 0 && avatar.plx - deltaTime <= 0.4 + 0.2f) == false) {
			if ((avatar.ply >= 1.8 - 0.2f && avatar.ply <= 2.6 + 0.2f && avatar.plx - deltaTime >= 1 - 0.2f && avatar.plx - deltaTime <= 1.8 + 0.2f) == false) {
				if ((avatar.ply >= 1.4 - 0.2f && avatar.ply <= 1.6 + 0.2f && avatar.plx - deltaTime >= 2.4 - 0.2f && avatar.plx - deltaTime <= 2.6 + 0.2f) == false) {
					if ((avatar.ply >= 0 - 0.2f && avatar.ply <= 0.2 + 0.2f && avatar.plx - deltaTime >= 2.8 - 0.2f && avatar.plx - deltaTime <= 3 + 0.2f) == false) {
						if ((avatar.ply <= 0.2 && avatar.plx - deltaTime >= 0.2 && avatar.plx - deltaTime <= 0.8 + 0.2f) == false) {
							if ((avatar.ply <= 0.8 + 0.2f && avatar.plx - deltaTime >= 0.2 - 0.18f && avatar.plx - deltaTime <= 0.2 + 0.2f) == false) {
								if (avatar.plx - deltaTime * 2 > 0) {
									lx -= deltaTime * 2;
									avatar.plx -= deltaTime * 2;
									avatar.rx -= deltaTime * 2;
									avatar.lx -= deltaTime * 2;
									healthbar.deltaX -= deltaTime * 2;
								}
							}
						}
					}
				}
			}
		}
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		if ((avatar.ply >= 1.8 - 0.2f && avatar.ply <= 2.6 + 0.2f && avatar.plx + deltaTime >= 1 - 0.2f && avatar.plx + deltaTime <= 1.8 + 0.2f) == false) {
			if ((avatar.ply >= 1.4 - 0.2f && avatar.ply <= 1.6 + 0.2f && avatar.plx + deltaTime >= 2.4 - 0.2f && avatar.plx + deltaTime <= 2.6 + 0.2f) == false) {
				if ((avatar.ply >= 0 - 0.2f && avatar.ply <= 0.2 + 0.2f && avatar.plx + deltaTime >= 2.8 - 0.2f && avatar.plx + deltaTime <= 3 + 0.2f) == false) {
					if ((avatar.ply <= 0.2 && avatar.plx + deltaTime >= 0.2 && avatar.plx + deltaTime <= 0.8 + 0.2f) == false) {
						if ((avatar.ply <= 0.8 + 0.2f && avatar.plx + deltaTime >= 0.2 - 0.2f && avatar.plx + deltaTime <= 0.2 + 0.2f) == false) {
							if (avatar.plx + deltaTime * 2 < 4 - 0.2f) {
								lx += deltaTime * 2;
								avatar.plx += deltaTime * 2;
								avatar.rx += deltaTime * 2;
								avatar.lx += deltaTime * 2;
								healthbar.deltaX += deltaTime * 2;
							}
						}
					}
				}
			}
		}
	}

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		//logicSpace.x += 1;
		//logicSpace.y -= deltaTime * 0.5;
	}
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{
	// Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (glonteReady == true) {
		Glonte glonte;
		glonte.x = avatar.plx;
		glonte.y = avatar.ply;
		double MX = mouseX * 1.0000;

		double mX = (double)(2 * MX / window->GetResolution().x);

		double MY = mouseY * 1.0000;
		double mY = (double)(2 * MY / window->GetResolution().y);
		mY = 2 - mY;


		glonte.directionX = (double)(mX - avatar.plx) / 100;
		glonte.directionY = (double)(mY - avatar.ply) / 100;
		glonte.speedUpX = (double) 4 / (mX - avatar.plx);
		glonte.speedUpX = abs(glonte.speedUpX);
		gloante.push_back(glonte);
		glonteReady = false;
	}

}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

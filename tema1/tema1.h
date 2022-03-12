#pragma once

#include "components/simple_scene.h"
#include <vector>

namespace m1
{
	class Tema1 : public gfxc::SimpleScene
	{
	public:
		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

		struct Avatar
		{
			Avatar() : plx(0), ply(0), rx(0), ry(0), lx(0), ly(0), length(0), lengthArm(0) {}
			Avatar(float plx, float ply, float rx, float ry, float lx, float ly, float length, float lengthArm)
				: plx(plx), ply(ply), rx(rx), ry(ry), lx(lx), ly(ly), length(length), lengthArm(lengthArm) {}
			float plx;
			float ply;
			float rx;
			float ry;
			float lx;
			float ly;
			float length;
			float lengthArm;
		};

		struct Enemie
		{
			Enemie() : x(0), y(0), rx(0), ry(0), lx(0), ly(0), retired(false), directionX(0), directionY(0), speedUp(1) {}
			Enemie(float x, float y, float rx, float ry, float lx, float ly, bool retired, float directionX, float directionY, float speedUp)
				: x(x), y(y), rx(rx), ry(ry), lx(lx), ly(ly), retired(retired), directionX(directionX), directionY(directionY), speedUp(speedUp) {}
			float x;
			float y;
			float rx;
			float ry;
			float lx;
			float ly;
			float directionX;
			float directionY;
			bool retired;
			float speedUp;
		};

		struct Glonte
		{
			Glonte() : x(), y(), directionX(), directionY(), ready(), speedUpX() {}
			Glonte(float x, float y, float directionX, float directionY, bool ready, float speedUpX, float speedUpY)
				: x(x), y(y), directionX(directionX), directionY(directionY), ready(ready), speedUpX(speedUpX), speedUpY(speedUpY) {}
			float x;
			float y;
			float directionX;
			float directionY;
			bool ready;
			float speedUpX;
			float speedUpY;
		};

		struct Healthbar
		{
			Healthbar() : deltaX(), y(), length(), width(), count(), X() {}
			Healthbar(float deltaX, float y, float length, float width, int count, std::vector<float> X)
				: deltaX(deltaX), y(y), length(length), width(width), count(count), X(X) {}
			float deltaX;
			float y;
			float length;
			float width;
			int count;
			std::vector<float> X;
		};

	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void DrawScene(glm::mat3 visMatrix, double plx, double ply, int itsTimeForAnEnemie);

		void CreateEnemie();
		void CreateComplexEnemie();
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

		// Sets the logic space and view space
		// logicSpace: { x, y, width, height }
		// viewSpace: { x, y, width, height }
		glm::mat3 VisualizationTransf2D(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
		double lx = 0, ly = 0, lheight = 2, lweight = 2;
		double directionX, directionY;
		double plx = 1, ply = 1, angularStep = 0, timeForBullet = 0, timeForEnemie = 0, timeForLife = 0, timeForShoot = 0, b1x = 1, b1y = 1;
		double proiectX = plx, proiectY = ply, incPX = 0, incPY = 0, length_glonte, length_inamic, length_brat, length_life;
		int itsTimeForAnEnemie = 0, shoot = 0;
		double timeForComplexEnemie = 0;
		bool complexEnemieReady = true;
		void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	protected:
		float length;
		ViewportSpace viewSpace;
		LogicSpace logicSpace;
		glm::mat3 modelMatrix, visMatrix;
		Enemie enemie;
		std::vector<Enemie> enemies;
		std::vector<Enemie> complexEnemies;
		bool glonteReady = true;
		bool lifeReady = false;
		bool lifeOK = false;
		bool shootReady = false;
		std::vector<Glonte> gloante;
		//Glonte glonte;
		float totalTime = 0.000f;
		bool enemieReady = true;
		Healthbar healthbar;
		Avatar avatar;
	};
}   // namespace m1


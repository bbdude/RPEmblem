#pragma once
#include "VectorLib.h"
#include <iostream>
#include <map>
#include <vector>
#include "glut.h"
#include <gl/GL.h>
#include <gl\GLU.h>
#include "SOIL.h"
using namespace std;

template<class type>
class Node
{
public:
	type object;
};
class Land
{
public:
	GLuint	texture;
	float resistance;
	bool walkable;
	vector2 position;
	void init()
	{}
	virtual void draw()
	{}
	virtual bool load()
	{
		return false;
	}
};
class Wall :Land
{
public:
	int health;
	Wall(void){}
	~Wall(){}
	void init()
	{
		resistance = 100000;
		health = 10;
	}
	virtual void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glPushMatrix();

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(position.x - 1, position.y + 0.11 - 1, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(0, 1); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(position.x - 1, position.y - 1, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnd();
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"wall.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
}; 
class Dirt :Land
{
public:
	Dirt(void)
	{

	}
	~Dirt(){}
	void init()
	{
		resistance = 2;
	}
	virtual void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,texture);
		glBegin(GL_QUADS);
		glPushMatrix();

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(position.x - 1, position.y + 0.11 - 1, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(0, 1); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(position.x - 1, position.y - 1, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnd();
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"dirt.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			//throw std::bad_exception("Failure to load image");
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
};
class Selection :Land
{
public:
	Land * belowContent;
	virtual void draw()
	{
		belowContent->draw();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_QUADS);
		glPushMatrix();

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(belowContent->position.x - 1, belowContent->position.y + 0.11 - 1, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(0, 1); glVertex3f(belowContent->position.x - 1.11, belowContent->position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(belowContent->position.x - 1.11, belowContent->position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(belowContent->position.x - 1, belowContent->position.y - 1, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnd();
		//belowContent->draw();
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"select.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
};
class Characters
{
public:
	int health;
	GLint texture;
	vector<vector<Land*>> pathTaken;
	vector2 position;
	vector2 cord;
	std::string name;
	virtual void init(vector2 change, string newName)
	{
		health = 1;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;

		name = newName;
		load();
	}
	virtual void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glPushMatrix();

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(position.x - 1, position.y + 0.11 - 1, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(0, 1); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(1, 0); glVertex3f(position.x - 1, position.y - 1, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnd();
	}
	virtual bool load()
	{
		return false;
	}
};
class Rock : Characters
{
	virtual void init(vector2 change, string newName)
	{
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"Rock.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
};
class Paper : Characters
{
	virtual void init(vector2 change, string newName)
	{
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"Paper.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
};
class Emblem : Characters
{
	virtual void init(vector2 change, string newName)
	{
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();
	}

	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"Emblem.png",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_INVERT_Y
			);

		if (texture == 0)
			return false;


		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		return true;
	}
};
class Grid
{
public:
	Grid();
	~Grid();
public:
	vector2 screen;
	vector2 selectedNode;
	map<unsigned char, bool> keyState;
	map<string, Characters*> playable;
	map<string, Characters*> unplayable;
	vector<vector<Land*>> nodes;
	Selection sel;
	enum STATE { SPLASH, MENU, GAME, EXIT, OPTIONS };
	STATE curr;
	
	void init(int x/*18*/, int y/*18*/)
	{
		playable["Rock"] = (Characters*)new Rock;
		playable["Paper"] = (Characters*)new Paper;
		playable["Emblem"] = (Characters*)new Emblem;
		playable["Rock"]->init(vector2(1,1),"Rock");
		playable["Paper"]->init(vector2(1, 3), "Paper");
		playable["Emblem"]->init(vector2(3, 1), "Emblem");
		selectedNode = vector2(-1, -1);
		//sel.init();
		sel.load();
		vector<vector<Land*>> newNode = vector<vector<Land*>>(18);
		for (int i = 0; i < x; i++)
		{
			vector<Land*> newSubNode = vector<Land*>(18);
			for (int ii = 0; ii < y; ii++)
			{
				if (ii == 0 || ii == y-1 || i == 0 || i == x-1)
				{

					Wall * nWall = new Wall;
					nWall->init();
					//newDirt.object = nDirt;
					newSubNode.at(ii) = (Land*)nWall;
				}
				else
				{
					Dirt * nDirt = new Dirt();
					nDirt->init();
					//newDirt.object = nDirt;
					newSubNode.at(ii) = (Land*)nDirt;
				}
				newSubNode.at(ii)->load();
				newSubNode.at(ii)->position.x = (0.11) * i + (0.11);

				newSubNode.at(ii)->position.y = (0.11) * ii;

				//(((12.5 * breadth)) - ((ii + countx) * breadth)) + ((breadth * 5)*i);
				//0, (((12.5 * breadth)) - (countz * breadth)) + (breadth*i);
			}
			newNode.at(i) = newSubNode;
		}
		nodes = newNode;
	}
	void draw()
	{
		//drawSquare(vector2(0,0));
		//int x = 0,y = 0;
		//for (auto & i : nodes)

		//vector<vector<Land*>>::iterator i;
		//vector<Land*>::iterator ii;

		for (int i = 0; i < 18; i++) {
			for (int ii = 0; ii < 18; ii++) {

				nodes[i][ii]->draw();
				
				/*if (Dirt * p = dynamic_cast<Dirt*>(ii))
				{

				}
				y++;
				if (ii->resistance > 100 || (y == selectedNode.y && x == selectedNode.x))
					drawWall(ii->position);
				else
				{
					drawSquare(ii->position);
				}*/
			}
		}

		playable["Rock"]->draw();
		playable["Paper"]->draw();
		playable["Emblem"]->draw();
	}
	void drawSquare(vector2 pos)
	{
		
		glBegin(GL_QUADS);
		glPushMatrix();

		glColor3f(1, 0, 0);
		glVertex3f(pos.x - 1, pos.y + 0.11 - 1, 0);
		glColor3f(0, 1, 0);
		glVertex3f(pos.x - 1.11, pos.y + 0.11 - 1, 0);
		glColor3f(0, 0, 1);
		glVertex3f(pos.x - 1.11, pos.y - 1, 0);
		glColor3f(0, 0, 0);
		glVertex3f(pos.x - 1, pos.y - 1, 0);
		glPopMatrix();
		glEnd();

	}void drawWall(vector2 pos)
	{
		glBegin(GL_QUADS);
		glPushMatrix();

		glColor3f(0, 0, 0);
		glVertex3f(pos.x - 1, pos.y + 0.11 - 1, 0);
		glVertex3f(pos.x - 1.11, pos.y + 0.11 - 1, 0);
		glVertex3f(pos.x - 1.11, pos.y - 1, 0);
		glVertex3f(pos.x - 1, pos.y - 1, 0);
		glPopMatrix();
		glEnd();

	}
	void update()
	{

	}
};



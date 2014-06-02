#pragma once
#include "VectorLib.h"
#include <iostream>
#include <map>
#include <vector>
#include "glut.h"
#include <gl/GL.h>
#include <gl\GLU.h>
#include "SOIL.h"
#include "Weapon.h"
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
		glTexCoord2f(1,0); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(0, 1); glVertex3f(position.x - 1, position.y - 1, 0);
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
		glTexCoord2f(1,0); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(0, 1); glVertex3f(position.x - 1, position.y - 1, 0);
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
	vector2 position;//The actual value between -2 and 2
	vector2 cord;//The cords on the grid
	std::string name;
	Weapon * weapon;
	int turnOrder;

	virtual void init(vector2 change, string newName)
	{
		health = 1;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;

		name = newName;
		load();
		//weapon->type();
		//Initialize weapon here
	}
	virtual void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		//glRotatef(90, 0, 0, 1);
		glBegin(GL_QUADS);
		glPushMatrix();

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(position.x - 1, position.y + 0.11 - 1, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(1,0); glVertex3f(position.x - 1.11, position.y + 0.11 - 1, 0);
		//glColor3f(0, 0, 1);
		glTexCoord2f(1, 1); glVertex3f(position.x - 1.11, position.y - 1, 0);
		//glColor3f(0, 0, 0);
		glTexCoord2f(0,1); glVertex3f(position.x - 1, position.y - 1, 0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glEnd();
		//glRotatef(-90, 0, 0, 1);
	}
	virtual bool load()
	{
		return false;
	}
	virtual void updatePos(vector2 newCord)
	{
		cord = newCord;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
	}
};
class HeavyMace : Characters
{
	virtual void init(vector2 change, string newName)
	{
		turnOrder = 1;
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();

		weapon = (Weapon*)new MACE(20);
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"rock.png",
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
class Swordsman : Characters
{
	virtual void init(vector2 change, string newName)
	{
		turnOrder = 3;
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();

		weapon = (Weapon*)new SWORD(20);
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
class Axeman : Characters
{
	virtual void init(vector2 change, string newName)
	{
		turnOrder = 2;
		health = 10;
		cord = change;

		position.x = (0.11) * cord.x + (0.11);
		position.y = (0.11) * cord.y;
		name = newName;
		load();

		weapon = (Weapon*)new AXE(20);
	}

	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"emblem.png",
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


class PlayCard
{
public:
	GLint texture;
	Characters * sel;
	Characters * opSel;
	bool showOpSel;
	PlayCard(void)
	{
		showOpSel = false;
	}
	~PlayCard(){}
	virtual void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		glPushMatrix();

		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(0.5f, 1, 0);
		glTexCoord2f(1, 0); glVertex3f(1, 1, 0);
		glTexCoord2f(1, 1); glVertex3f(1, 0.75f, 0);
		glTexCoord2f(0, 1); glVertex3f(0.5f, 0.75f, 0);
		glPopMatrix();
		glEnd();
		
		
		glBindTexture(GL_TEXTURE_2D, sel->texture);
		glBegin(GL_QUADS);
		glPushMatrix();
		//glRotatef(90, 1, 0, 0);

		//glColor3f(1, 0, 0);
		glLoadIdentity();
		glColor3f(1, 1, 1);
		glTexCoord2f(0, 0); glVertex3f(0.51f, 0.99, 0);
		//glColor3f(0, 1, 0);
		glTexCoord2f(1, 0); glVertex3f(0.76, 0.99, 0);
		glTexCoord2f(1, 1); glVertex3f(0.76, 0.76f, 0);
		glTexCoord2f(0, 1); glVertex3f(0.51f, 0.76f, 0);
		glPopMatrix();
		glEnd();
		
		if (showOpSel)
		{
			glBindTexture(GL_TEXTURE_2D, opSel->texture);
			glBegin(GL_QUADS);
			glPushMatrix();
			//glRotatef(90, 1, 0, 0);

			//glColor3f(1, 0, 0);
			glLoadIdentity();
			glColor3f(1, 1, 1);
			glTexCoord2f(0, 0); glVertex3f(0.76f, 0.99, 0);
			//glColor3f(0, 1, 0);
			glTexCoord2f(1, 0); glVertex3f(0.99, 0.99, 0);
			glTexCoord2f(1, 1); glVertex3f(0.99, 0.76f, 0);
			glTexCoord2f(0, 1); glVertex3f(0.76f, 0.76f, 0);
			glPopMatrix();
			glEnd();
		}
		glDisable(GL_TEXTURE_2D);
	}
	virtual bool load()
	{
		texture = SOIL_load_OGL_texture
			(
			"card.png",
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
class Grid
{
public:
	Grid();
	~Grid();
public:
	vector2 screen;//Screen size
	vector2 selectedNode;//The cords for the selected node
	map<unsigned char, bool> keyState;//State of each key
	map<string, Characters*> playable;//List of all the character player
	map<string, Characters*> unplayable;//List of all the non-character players
	vector<vector<Land*>> nodes;//Each tile
	PlayCard card;
	Selection sel;//The selected tile
	Characters * selCharacter;//The selected character
	enum STATE { SPLASH, MENU, GAME, EXIT, OPTIONS };//Game states
	STATE curr;//The current state
	bool move;
	int turn;//Controls who can act and who cannot
	
	void init(int x/*18*/, int y/*18*/)
	{
		turn = 1;
		playable["HeavyMace"] = (Characters*)new HeavyMace;
		playable["Swordsman"] = (Characters*)new Swordsman;
		playable["Axeman"] = (Characters*)new Axeman;
		playable["HeavyMace"]->init(vector2(1,1),"HeavyMace");
		playable["Swordsman"]->init(vector2(1, 3), "Swordsman");
		playable["Axeman"]->init(vector2(3, 1), "Axeman");
		selectedNode = vector2(-1, -1);
		//sel.init();
		sel.load();
		card.load();
		card.sel = (Characters*)playable["HeavyMace"];
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

		playable["HeavyMace"]->draw();
		playable["Swordsman"]->draw();
		playable["Axeman"]->draw();
		card.draw();
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
		//if (selectedNode != vector2(-1,-1))
		for (map<string, Characters*>::iterator it = playable.begin(); it != playable.end();it++)
		{
			if (turn == it->second->turnOrder)
			{
				card.sel = (Characters*)it->second;
				if (move)
				{
					it->second->updatePos(selectedNode);
					selectedNode = vector2(-1, -1);
					turn++;
					move = false;
				}
			}
			if (selectedNode == it->second->cord)
			{
				selCharacter = (Characters*)it->second;
				card.opSel = (Characters*)selCharacter;
				card.showOpSel = true;
			}
			else
			{
				card.showOpSel = false;
				selCharacter = (Characters*)new Characters;
			}
		}
		if (turn > 3)
			turn = 1;
	}
};


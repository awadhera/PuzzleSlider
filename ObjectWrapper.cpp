/******************************************************************************/
/*!
\file ObjectWrapper.cpp
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Implementation of Wrapper of game objects for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/
#include <string>
#include "ObjectWrapper.h"


///Initialize static instance of Wrapper
Wrapper * Wrapper::s_pInstance = 0;
/******************************************************************************/
/*!
Constructor for Wrapper class
*/
/******************************************************************************/
Wrapper::Wrapper()
{
	count = 0;
}

/******************************************************************************/
/*!
Destructor for Wrapper class
*/
/******************************************************************************/
Wrapper::~Wrapper()
{
}


/******************************************************************************/
/*!
Create instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::create()
{
	s_pInstance = new Wrapper;
}

/******************************************************************************/
/*!
Destroy(deallocate) instance of Wrapper class
\return
void
*/
/******************************************************************************/
void Wrapper::destroy()
{
	delete s_pInstance;
}

/******************************************************************************/
/*!
Provide static instance of Wrapper class
\return
Reference to Wrapper instance
*/
/******************************************************************************/
Wrapper & Wrapper::Instance()
{
	return *s_pInstance;
}

/******************************************************************************/
/*!
Create the Vertices and Indices for all the geometric objects
Also create the transform objects
\return
void
*/
/******************************************************************************/
void Wrapper::createGeometries()
{
	grid_2x2[1].addTexture("1.tga");
	grid_2x2[2].addTexture("2.tga");
	grid_2x2[3].addTexture("3.tga");
	grid_3x3[1].addTexture("1.tga");
	grid_3x3[2].addTexture("2.tga");
	grid_3x3[3].addTexture("3.tga");
	grid_3x3[4].addTexture("4.tga");
	grid_3x3[5].addTexture("5.tga");
	grid_3x3[6].addTexture("6.tga");
	grid_3x3[7].addTexture("7.tga");
	if(!grid_3x3[8].addTexture("8.tga"))
	{
		(Logger::Instance()).checkAndReportGLError("Failed to add Texture to Grid",'e',4,__LINE__,__FILE__);
	}
	for(unsigned int i = 0;i < 8;++i)
	{
		grid_misc[i].addTexture("Misc.tga");
	}
	grid_2x2[0].createGeometry(0,0,0);
	grid_3x3[0].createGeometry(0,0,0);
	for(unsigned int i = 1;i < 4; ++i)
	{
		grid_2x2[i].createGeometry(255,255,0);
	}
	for(unsigned int i = 1;i < 9; ++i)
	{
		grid_3x3[i].createGeometry(255,255,0);
	}

	for(unsigned int i = 0;i < 8; ++i)
	{
		grid_misc[i].createGeometry(255,255,0);
	}
	t_3x3[0].upload(-1.0f,1.0f,0.0f);
	t_3x3[1].upload(0.0f,1.0f,0.0f);
	t_3x3[2].upload(1.0f,1.0f,0.0f);
	t_3x3[3].upload(-1.0f,0.0f,0.0f);
	t_3x3[4].upload(0.0,0.0f,0.0f);
	t_3x3[5].upload(1.0f,0.0f,0.0f);
	t_3x3[6].upload(-1.0f,-1.0f,0.0f);
	t_3x3[7].upload(0.0f,-1.0f,0.0f);
	t_3x3[8].upload(1.0f,-1.0f,0.0f);

	t_2x2[0].upload(-0.5f,0.5f,0.0f);
	t_2x2[1].upload(0.5f,0.5f,0.0f);
	t_2x2[2].upload(-0.5f,-0.5f,0.0f);
	t_2x2[3].upload(0.5f,-0.5f,0.0f);

	t_misc[0].upload(5.3f,3.2f,0.0f);
	t_misc[1].upload(5.3f,2.0f,0.0f);
	t_misc[2].upload(5.3f,0.8f,0.0f);
	t_misc[3].upload(5.3f,-0.4f,0.0f);
	t_misc[4].upload(5.3f,-1.6f,0.0f);
	t_misc[5].upload(5.3f,-2.8f,0.0f);
	t_misc[6].upload(-1.0f,-4.0f,0.0f);
	t_misc[7].upload(1.0f,-4.0f,0.0f);

	s[0].upload(4.0f,1.0f,1.0f);
	s[1].upload(1.5f,1.0f,1.0f);

}

/******************************************************************************/
/*!
Upload the geometries to the GPU
\return
void
*/
/******************************************************************************/
void Wrapper::uploadGeometries()
{
	for(unsigned int i = 0;i < 4; ++i)
	{
		grid_2x2[i].uploadGeometry();
	}
	for(unsigned int i = 0;i < 9; ++i)
	{
		grid_3x3[i].uploadGeometry();
	}
	for(unsigned int i = 0;i < 8; ++i)
	{
		grid_misc[i].uploadGeometry();
	}
}

/******************************************************************************/
/*!
Destroy the geometries - Unbind the buffers, delete the vertices and indices info
\return
void
*/
/******************************************************************************/
void Wrapper::destroyGeometries()
{
	for(unsigned int i = 0;i < 4; ++i)
	{
		grid_2x2[i].destroyGeometry();
	}
	for(unsigned int i = 0;i < 9; ++i)
	{
		grid_3x3[i].destroyGeometry();
	}
	for(unsigned int i = 0;i < 8; ++i)
	{
		grid_misc[i].destroyGeometry();
	}
}

/******************************************************************************/
/*!
createTiles:Create game objects for Tiles
\return
void
*/
/******************************************************************************/
void Wrapper::createTiles()
{
	for(unsigned int i = 0;i < 4; ++i)
	{
		tiles_2x2[i] = new GameObject(&grid_2x2[i],square);
	}
	for(unsigned int i = 0;i < 9; ++i)
	{
		tiles_3x3[i] = new GameObject(&grid_3x3[i],square);
	}
	for(unsigned int i = 0;i < 8; ++i)
	{
		tiles_misc[i] = new GameObject(&grid_misc[i],square);
	}
}

/******************************************************************************/
/*!
destroyTiles:Delete the game objects for tiles
\return
void
*/
/******************************************************************************/
void Wrapper::destroyTiles()
{
	for(unsigned int i = 0;i < 4; ++i)
		delete tiles_2x2[i];
	for(unsigned int i = 0;i < 9; ++i)
		delete tiles_3x3[i];
	for(unsigned int i = 0;i < 8; ++i)
		delete tiles_misc[i];
}

/******************************************************************************/
/*!
InitializeTiles:Initialize the game objects for tiles
\return
void
*/
/******************************************************************************/
void Wrapper::InitializeTiles()
{
	for(int i = 0;i < 4; ++i)
	{
		tiles_2x2[i]->translate.upload(t_2x2[i].x,t_2x2[i].y,t_2x2[i].z);
		tiles_2x2[i]->geoType->local.add(&tiles_2x2[i]->translate);
		tiles_2x2[i]->p.x = t_2x2[i].x;
		tiles_2x2[i]->p.y = t_2x2[i].y;
		tiles_2x2[i]->p.z = t_2x2[i].z;
	}
	for(int i = 0;i < 9; ++i)
	{
		tiles_3x3[i]->translate.upload(t_3x3[i].x,t_3x3[i].y,t_3x3[i].z);
		tiles_3x3[i]->geoType->local.add(&tiles_3x3[i]->translate);
		tiles_3x3[i]->p.x = t_3x3[i].x;
		tiles_3x3[i]->p.y = t_3x3[i].y;
		tiles_3x3[i]->p.z = t_3x3[i].z;
	}
	for(int i = 0;i < 8; ++i)
	{
		tiles_misc[i]->translate.upload(t_misc[i].x,t_misc[i].y,t_misc[i].z);
		tiles_misc[i]->geoType->local.add(&tiles_misc[i]->translate);
		tiles_misc[i]->p.x = t_misc[i].x;
		tiles_misc[i]->p.y = t_misc[i].y;
		tiles_misc[i]->p.z = t_misc[i].z;
	}
	for(int i = 0;i < 4; ++i)
	{
		state_2x2[i] = i;
	}
	for(int i = 0;i < 9; ++i)
	{
		state_3x3[i] = i;
	}
	for(int i = 0;i < 6; ++i)
	{
		tiles_misc[i]->scale.upload(s[0].x,s[0].y,s[0].z);
		tiles_misc[i]->geoType->local.add(&tiles_misc[i]->scale);
	}
	for(int i = 6;i < 8; ++i)
	{
		tiles_misc[i]->scale.upload(s[1].x,s[1].y,s[1].z);
		tiles_misc[i]->geoType->local.add(&tiles_misc[i]->scale);
	}
	gameInSlidingState = 0;
	speed = 3.0f;
	animationSpeed = "Animation Speed:Slow";
	algorithmUsed = "Algorithm:DFS";
	np_2x2 = new NPuzzle(2);
	np_3x3 = new NPuzzle(3);
	sm = DFS;
	gameinAIMode = 0;
}

/******************************************************************************/
/*!
RandomizeTiles:Randomize the position of the tiles
\return
void
*/
/******************************************************************************/
void Wrapper::RandomizeTiles()
{
	if(gameinAIMode == 0)
	{
		if(state == 2)
		{
			generateNrandom(state_2x2,4);
			for(int i = 0;i < 4; ++i)
			{
				tiles_2x2[state_2x2[i]]->geoType->local.remove();
				tiles_2x2[state_2x2[i]]->translate.upload(t_2x2[i].x,t_2x2[i].y,t_2x2[i].z);
				tiles_2x2[state_2x2[i]]->geoType->local.add(&tiles_2x2[state_2x2[i]]->translate);
				tiles_2x2[state_2x2[i]]->p.x = t_2x2[i].x;
				tiles_2x2[state_2x2[i]]->p.y = t_2x2[i].y;
				tiles_2x2[state_2x2[i]]->p.z = t_2x2[i].z;
			}
		}
		else if(state == 3)
		{
			generateNrandom(state_3x3,9);
			for(int i = 0;i < 9; ++i)
			{
				tiles_3x3[state_3x3[i]]->geoType->local.remove();
				tiles_3x3[state_3x3[i]]->translate.upload(t_3x3[i].x,t_3x3[i].y,t_3x3[i].z);
				tiles_3x3[state_3x3[i]]->geoType->local.add(&tiles_3x3[state_3x3[i]]->translate);
				tiles_3x3[state_3x3[i]]->p.x = t_3x3[i].x;
				tiles_3x3[state_3x3[i]]->p.y = t_3x3[i].y;
				tiles_3x3[state_3x3[i]]->p.z = t_3x3[i].z;
			}
		}
	}
}

/******************************************************************************/
/*!
processInput:Process the mouse click
\param [in] posX
Position of the x mouse coord
\param [in] posY
Position of the y mouse coord
\return
void
*/
/******************************************************************************/
void Wrapper::processInput(const float &posX,const float &posY)
{
	if((posX>=3.3f && posX<=7.3f) && (posY>=-2.1f && posY<=-1.1f))
	{
		speed = (speed == 3.0f)?15.0f:3.0f;
		animationSpeed = (speed == 3.0f)?"Animation Speed:Slow":"Animation Speed:Fast";
	}
	else if(gameinAIMode == 0)
	{
		(Wrapper::Instance()).nodesGenerated = "";
		(Wrapper::Instance()).timeTaken = "";
		(Wrapper::Instance()).solutionSteps = "";
		(Wrapper::Instance()).stressTestStr = "";
		int clickedTile = checkClickOnTile(posX,posY);
		if(clickedTile!=-1)
		{
			if(checkPossibleMove(clickedTile))
			{
				moveTileToEmptyCell();
			}
		}
		else if((posX>=3.3f && posX<=7.3f) && (posY>=1.5f && posY<=2.5f))
		{
			RandomizeTiles();
		}
		else if((posX>=-1.75f && posX<=-0.25f) && (posY>=-4.5f && posY<=-3.5f))
		{
			state = 2;
		}
		else if((posX>=0.25f && posX<=1.75f) && (posY>=-4.5f && posY<=-3.5f))
		{
			state = 3;
		}
		else if((posX>=3.3f && posX<=7.3f) && (posY>=0.3f && posY<=1.3f))
		{
			if(state == 2)
			{
				for(int i = 0;i < 4; ++i)
				{
					state_2x2[i] = i;
				}
				for(int i = 0;i < 4; ++i)
				{
					tiles_2x2[state_2x2[i]]->geoType->local.remove();
					tiles_2x2[state_2x2[i]]->translate.upload(t_2x2[i].x,t_2x2[i].y,t_2x2[i].z);
					tiles_2x2[state_2x2[i]]->geoType->local.add(&tiles_2x2[state_2x2[i]]->translate);
					tiles_2x2[state_2x2[i]]->p.x = t_2x2[i].x;
					tiles_2x2[state_2x2[i]]->p.y = t_2x2[i].y;
					tiles_2x2[state_2x2[i]]->p.z = t_2x2[i].z;
				}
			}
			else if(state == 3)
			{
				for(int i = 0;i < 9; ++i)
				{
					state_3x3[i] = i;
				}
				for(int i = 0;i < 9; ++i)
				{
					tiles_3x3[state_3x3[i]]->geoType->local.remove();
					tiles_3x3[state_3x3[i]]->translate.upload(t_3x3[i].x,t_3x3[i].y,t_3x3[i].z);
					tiles_3x3[state_3x3[i]]->geoType->local.add(&tiles_3x3[state_3x3[i]]->translate);
					tiles_3x3[state_3x3[i]]->p.x = t_3x3[i].x;
					tiles_3x3[state_3x3[i]]->p.y = t_3x3[i].y;
					tiles_3x3[state_3x3[i]]->p.z = t_3x3[i].z;
				}
			}
		}
		else if((posX>=3.3f && posX<=7.3f) && (posY>=2.7f && posY<=3.7f))
		{
			sm = (sm==DFS)?ASTAR:DFS;
			algorithmUsed = (algorithmUsed.compare("Algorithm:DFS")==0)?"Algorithm:AStar":"Algorithm:DFS";
		}
		else if((posX>=3.3f && posX<=7.3f) && (posY>=-0.9f && posY<=0.1f))
		{
			bool solRet = false;
			if(state == 2)
			{
				np_2x2->SetState(state_2x2,(np_2x2->GetTilesNumber())+1);
				LARGE_INTEGER freq, oldCount, newCount;
				QueryPerformanceFrequency(&freq);
				QueryPerformanceCounter(&oldCount);
				std::cout<<std::endl;
				std::cout<<"SEARCHING FOR SOLUTION...PLEASE WAIT";
				std::cout<<std::endl;
				solRet = np_2x2->SearchForSolution(sm); 
				if(solRet)
				{
					gameinAIMode = 1;
					if(!sol.empty())
					{
						sol.clear();
					}
					sol = np_2x2->GetSolution();
					QueryPerformanceCounter(&newCount);
					std::stringstream ss1;
					std::string str1;
					double milliSeconds = ((newCount.QuadPart - oldCount.QuadPart)*1000.0/freq.QuadPart);
					ss1 << milliSeconds;
					ss1 >> str1;
					(Wrapper::Instance()).timeTaken = "Time taken to find solution:" + str1 + " milliseconds";
					MyStatistics *stats = np_2x2->GetStatistics();
					std::stringstream ss2; 
					std::string str2;
					ss2 << stats->m_nodesGenerated;
					ss2 >> str2;
					(Wrapper::Instance()).nodesGenerated = "Nodes Generated:" + str2;
					std::stringstream ss3;
					std::string str3;
					ss3 << sol.size()-1;
					ss3 >> str3;
					(Wrapper::Instance()).solutionSteps = "Solution Steps:" + str3;
					it1 = sol.begin();
					it2 = it1+1;
				}
				else
				{
					(Wrapper::Instance()).nodesGenerated = "No Solution found";
					(Wrapper::Instance()).timeTaken = "";
					(Wrapper::Instance()).solutionSteps = "";
				}
			}
			else if(state == 3)
			{
				np_3x3->SetState(state_3x3,(np_3x3->GetTilesNumber())+1);
				LARGE_INTEGER freq, oldCount, newCount;
				QueryPerformanceFrequency(&freq);
				QueryPerformanceCounter(&oldCount);
				std::cout<<std::endl;
				std::cout<<"SEARCHING FOR SOLUTION...PLEASE WAIT";
				std::cout<<std::endl;
				solRet = np_3x3->SearchForSolution(sm);
				if(solRet)
				{
					gameinAIMode = 1;
					if(!sol.empty())
					{
						sol.clear();
					}
					sol = np_3x3->GetSolution();
					QueryPerformanceCounter(&newCount);
					std::stringstream ss1;
					std::string str1;
					double milliSeconds = ((newCount.QuadPart - oldCount.QuadPart)*1000.0/freq.QuadPart);
					ss1 << milliSeconds;
					ss1 >> str1;
					(Wrapper::Instance()).timeTaken = "Time taken to find solution:" + str1 + " milliseconds";
					MyStatistics *stats = np_3x3->GetStatistics();
					std::stringstream ss2;
					std::string str2;
					ss2 << stats->m_nodesGenerated;
					ss2 >> str2;
					(Wrapper::Instance()).nodesGenerated = "Nodes Generated:" + str2;
					std::stringstream ss3;
					std::string str3;
					ss3 << sol.size()-1;
					ss3 >> str3;
					(Wrapper::Instance()).solutionSteps = "Solution Steps:" + str3;
					it1 = sol.begin();
					it2 = it1+1;
				}
				else
				{
					(Wrapper::Instance()).nodesGenerated = "No Solution found";
					(Wrapper::Instance()).timeTaken = "";
					(Wrapper::Instance()).solutionSteps = "";
				}
			}
		}
		else if((posX>=3.3f && posX<=7.3f) && (posY>=-3.3f && posY<=-2.3f))
		{
			StressTest();
		}
	}
}

/******************************************************************************/
/*!
checkClickOnTile:Check if mouse click is inside the boxes in game
\param [in] posX
x mouse coord
\param [in] posY
y mouse coord
\return
int - the position of clicked tile
*/
/******************************************************************************/
int Wrapper::checkClickOnTile(const float &posX,const float &posY)
{
	if(state == 2)
	{
		if(posY>=0.0f && posY<=1.0f)
		{
			if(posX>=-1.0f && posX<=0.0f)
			{
				return 0;
			}
			else if(posX>=0.0f && posX<=1.0f)
			{
				return 1;
			}
			else
				return -1;
		}
		else if(posY>=-1.0f && posY<=0.0f)
		{
			if(posX>=-1.0f && posX<=0.0f)
			{
				return 2;
			}
			else if(posX>=0.0f && posX<=1.0f)
			{
				return 3;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	else if(state == 3)
	{
		if(posY>=0.5f && posY<=1.5f)
		{
			if(posX>=-1.5f && posX<=-0.5f)
			{
				return 0;
			}
			else if(posX>=-0.5f && posX<=0.5f)
			{
				return 1;
			}
			else if(posX>=0.5f && posX<=1.5f)
			{
				return 2;
			}
			else
			{
				return -1;
			}
		}
		else if(posY>=-0.5f && posY<=0.5f)
		{
			if(posX>=-1.5f && posX<=-0.5f)
			{
				return 3;
			}
			else if(posX>=-0.5f && posX<=0.5f)
			{
				return 4;
			}
			else if(posX>=0.5f && posX<=1.5f)
			{
				return 5;
			}
			else
			{
				return -1;
			}
		}
		else if(posY>=-1.5f && posY<=-0.5f)
		{
			if(posX>=-1.5f && posX<=-0.5f)
			{
				return 6;
			}
			else if(posX>=-0.5f && posX<=0.5f)
			{
				return 7;
			}
			else if(posX>=0.5f && posX<=1.5f)
			{
				return 8;
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

}

/******************************************************************************/
/*!
Check if the clicked tile can be moved to empty position
\param [in] index
Index of the clicked tile
\return
bool - True if tile can be moved
*/
/******************************************************************************/
bool Wrapper::checkPossibleMove(int index)
{
	float diffX,diffY;
	diffX = -1.0f;
	diffY = -1.0f;
	if(state == 2)
	{
		for(int i = 0;i < 4;++i)
		{
			if(state_2x2[i] == 0)
			{
				zeroCellPos = i;
				break;
			}
		}
		diffX = std::fabs(tiles_2x2[state_2x2[zeroCellPos]]->p.x - tiles_2x2[state_2x2[index]]->p.x);
		diffY = std::fabs(tiles_2x2[state_2x2[zeroCellPos]]->p.y - tiles_2x2[state_2x2[index]]->p.y);
	}
	else if(state == 3)
	{
		for(int i = 0;i < 9;++i)
		{
			if(state_3x3[i] == 0)
			{
				zeroCellPos = i;
				break;
			}
		}
		diffX = std::fabs(tiles_3x3[state_3x3[zeroCellPos]]->p.x - tiles_3x3[state_3x3[index]]->p.x);
		diffY = std::fabs(tiles_3x3[state_3x3[zeroCellPos]]->p.y - tiles_3x3[state_3x3[index]]->p.y);
	}
	if((diffX == 1.0f && diffY == 0.0f) || (diffX == 0.0f && diffY == 1.0f))
	{
		slidingTileIndex = index;
		return true;
	}
	else
		return false;
}

/******************************************************************************/
/*!
Move the clicked tile to empty cell
\return
void
*/
/******************************************************************************/
void Wrapper::moveTileToEmptyCell()
{
	float diffX,diffY;

	int index = slidingTileIndex;

	if(state == 2)
	{	
		diffX = tiles_2x2[state_2x2[zeroCellPos]]->p.x - tiles_2x2[state_2x2[index]]->p.x;
		diffY = tiles_2x2[state_2x2[zeroCellPos]]->p.y - tiles_2x2[state_2x2[index]]->p.y;

		tiles_2x2[state_2x2[index]]->v.z = 0.0f;
		if(diffX == 1.0f)
		{
			tiles_2x2[state_2x2[index]]->v.x = 1.0f;
			tiles_2x2[state_2x2[index]]->v.y = 0.0f;
		}
		else if(diffX == -1.0f)
		{
			tiles_2x2[state_2x2[index]]->v.x = -1.0f;
			tiles_2x2[state_2x2[index]]->v.y = 0.0f;
		}
		else if(diffY == 1.0f)
		{
			tiles_2x2[state_2x2[index]]->v.x = 0.0f;
			tiles_2x2[state_2x2[index]]->v.y = 1.0f;
		}
		else if(diffY == -1.0f)
		{
			tiles_2x2[state_2x2[index]]->v.x = 0.0f;
			tiles_2x2[state_2x2[index]]->v.y = -1.0f;
		}
		tiles_2x2[state_2x2[index]]->speed = speed;
		tiles_2x2[state_2x2[index]]->d.x = tiles_2x2[state_2x2[zeroCellPos]]->p.x;
		tiles_2x2[state_2x2[index]]->d.y = tiles_2x2[state_2x2[zeroCellPos]]->p.y;
		tiles_2x2[state_2x2[index]]->d.z = tiles_2x2[state_2x2[zeroCellPos]]->p.z;
		++gameInSlidingState;

		tiles_2x2[state_2x2[zeroCellPos]]->p.x = tiles_2x2[state_2x2[index]]->p.x;
		tiles_2x2[state_2x2[zeroCellPos]]->p.y = tiles_2x2[state_2x2[index]]->p.y;
		tiles_2x2[state_2x2[zeroCellPos]]->translate.x = tiles_2x2[state_2x2[index]]->translate.x;
		tiles_2x2[state_2x2[zeroCellPos]]->translate.y = tiles_2x2[state_2x2[index]]->translate.y;
	}
	else if(state == 3)
	{	
		diffX = tiles_3x3[state_3x3[zeroCellPos]]->p.x - tiles_3x3[state_3x3[index]]->p.x;
		diffY = tiles_3x3[state_3x3[zeroCellPos]]->p.y - tiles_3x3[state_3x3[index]]->p.y;

		tiles_3x3[state_3x3[index]]->v.z = 0.0f;
		if(diffX == 1.0f)
		{
			tiles_3x3[state_3x3[index]]->v.x = 1.0f;
			tiles_3x3[state_3x3[index]]->v.y = 0.0f;
		}
		else if(diffX == -1.0f)
		{
			tiles_3x3[state_3x3[index]]->v.x = -1.0f;
			tiles_3x3[state_3x3[index]]->v.y = 0.0f;
		}
		else if(diffY == 1.0f)
		{
			tiles_3x3[state_3x3[index]]->v.x = 0.0f;
			tiles_3x3[state_3x3[index]]->v.y = 1.0f;
		}
		else if(diffY == -1.0f)
		{
			tiles_3x3[state_3x3[index]]->v.x = 0.0f;
			tiles_3x3[state_3x3[index]]->v.y = -1.0f;
		}
		tiles_3x3[state_3x3[index]]->speed = speed;
		tiles_3x3[state_3x3[index]]->d.x = tiles_3x3[state_3x3[zeroCellPos]]->p.x;
		tiles_3x3[state_3x3[index]]->d.y = tiles_3x3[state_3x3[zeroCellPos]]->p.y;
		tiles_3x3[state_3x3[index]]->d.z = tiles_3x3[state_3x3[zeroCellPos]]->p.z;
		++gameInSlidingState;

		tiles_3x3[state_3x3[zeroCellPos]]->p.x = tiles_3x3[state_3x3[index]]->p.x;
		tiles_3x3[state_3x3[zeroCellPos]]->p.y = tiles_3x3[state_3x3[index]]->p.y;
		tiles_3x3[state_3x3[zeroCellPos]]->translate.x = tiles_3x3[state_3x3[index]]->translate.x;
		tiles_3x3[state_3x3[zeroCellPos]]->translate.y = tiles_3x3[state_3x3[index]]->translate.y;
	}
	slideTile();
}

/******************************************************************************/
/*!
Slide the Tile
\return
void
*/
/******************************************************************************/
void Wrapper::slideTile()
{
	float speedTime = speed*dt;
	int index = slidingTileIndex;
	if(state == 2)
	{
		Vector velocity = tiles_2x2[state_2x2[index]]->v*speedTime;
		/// Update the position
		tiles_2x2[state_2x2[index]]->p = tiles_2x2[state_2x2[index]]->p + velocity;
		int flag = 0;
		if(tiles_2x2[state_2x2[index]]->v.x == 1.0f && tiles_2x2[state_2x2[index]]->p.x >= tiles_2x2[state_2x2[index]]->d.x)
		{
			flag = 1;
		}
		else if(tiles_2x2[state_2x2[index]]->v.x == -1.0f && tiles_2x2[state_2x2[index]]->p.x <= tiles_2x2[state_2x2[index]]->d.x)
		{
			flag = 1;
		}
		else if(tiles_2x2[state_2x2[index]]->v.y == 1.0f && tiles_2x2[state_2x2[index]]->p.y >= tiles_2x2[state_2x2[index]]->d.y)
		{
			flag = 1;
		}
		else if(tiles_2x2[state_2x2[index]]->v.y == -1.0f && tiles_2x2[state_2x2[index]]->p.y <= tiles_2x2[state_2x2[index]]->d.y)
		{
			flag = 1;
		}
		if(flag == 1)
		{
			tiles_2x2[state_2x2[index]]->p.x = tiles_2x2[state_2x2[index]]->d.x;
			tiles_2x2[state_2x2[index]]->p.y = tiles_2x2[state_2x2[index]]->d.y;
			tiles_2x2[state_2x2[index]]->p.z = tiles_2x2[state_2x2[index]]->d.z;
			--gameInSlidingState;
		}

		/// Translate the geometric object for this physics object through translate trasform
		tiles_2x2[state_2x2[index]]->translate.x = tiles_2x2[state_2x2[index]]->p.x;
		tiles_2x2[state_2x2[index]]->translate.y = tiles_2x2[state_2x2[index]]->p.y;
		tiles_2x2[state_2x2[index]]->translate.z = tiles_2x2[state_2x2[index]]->p.z;
		if(flag == 1)
		{
			int temp = state_2x2[index];
			state_2x2[index] = state_2x2[zeroCellPos];
			state_2x2[zeroCellPos] = temp;
		}
	}
	else if(state == 3)
	{
		Vector velocity = tiles_3x3[state_3x3[index]]->v*speedTime;
		/// Update the position
		tiles_3x3[state_3x3[index]]->p = tiles_3x3[state_3x3[index]]->p + velocity;
		int flag = 0;
		if(tiles_3x3[state_3x3[index]]->v.x == 1.0f && tiles_3x3[state_3x3[index]]->p.x >= tiles_3x3[state_3x3[index]]->d.x)
		{
			flag = 1;
		}
		else if(tiles_3x3[state_3x3[index]]->v.x == -1.0f && tiles_3x3[state_3x3[index]]->p.x <= tiles_3x3[state_3x3[index]]->d.x)
		{
			flag = 1;
		}
		else if(tiles_3x3[state_3x3[index]]->v.y == 1.0f && tiles_3x3[state_3x3[index]]->p.y >= tiles_3x3[state_3x3[index]]->d.y)
		{
			flag = 1;
		}
		else if(tiles_3x3[state_3x3[index]]->v.y == -1.0f && tiles_3x3[state_3x3[index]]->p.y <= tiles_3x3[state_3x3[index]]->d.y)
		{
			flag = 1;
		}
		if(flag == 1)
		{
			tiles_3x3[state_3x3[index]]->p.x = tiles_3x3[state_3x3[index]]->d.x;
			tiles_3x3[state_3x3[index]]->p.y = tiles_3x3[state_3x3[index]]->d.y;
			tiles_3x3[state_3x3[index]]->p.z = tiles_3x3[state_3x3[index]]->d.z;
			--gameInSlidingState;
		}

		/// Translate the geometric object for this physics object through translate trasform
		tiles_3x3[state_3x3[index]]->translate.x = tiles_3x3[state_3x3[index]]->p.x;
		tiles_3x3[state_3x3[index]]->translate.y = tiles_3x3[state_3x3[index]]->p.y;
		tiles_3x3[state_3x3[index]]->translate.z = tiles_3x3[state_3x3[index]]->p.z;
		if(flag == 1)
		{
			int temp = state_3x3[index];
			state_3x3[index] = state_3x3[zeroCellPos];
			state_3x3[zeroCellPos] = temp;
		}
	}
}

/******************************************************************************/
/*!
Do a stress Test for 1000 different solutions
\return
void
*/
/******************************************************************************/
void Wrapper::StressTest()
{
	(Wrapper::Instance()).stressTestStr = "";
	unsigned int tn,*array;
	NPuzzle *np = NULL;
	if(state == 2)
	{
		np = np_2x2;
		np->SetState(state_2x2,(np_2x2->GetTilesNumber())+1);
	}
	else if(state == 3)
	{
		np = np_3x3;
		np->SetState(state_3x3,(np_3x3->GetTilesNumber())+1);
	}
	tn = np->GetTilesNumber()+1;
	array = new unsigned int [tn];
	unsigned int numberOfDifferentSolutions = 1000;
	bool res;
	if(!paths.empty())
	{
		paths.clear();
	}
	const unsigned int *saveState = np->GetState();
	LARGE_INTEGER freq, oldCount, newCount;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&oldCount);
	bool flag;
	for(unsigned int loop = 0; loop < numberOfDifferentSolutions; ++loop)
	{
		flag = false;
		while(!flag)
		{
			generateNrandom(array,tn);
			if(np->CheckLegalTilesPositions(array))
			{
				flag = true;
			}
		}
		np->SetState(array,np->GetTilesNumber()+1);
		res = np->SearchForSolution(sm);
		if(res)
		{
			PathInfo pi;
			pi.size = np->GetSolution().size() - 1;
			pi.numberOfNodes = np->GetStatistics()->m_nodesGenerated;
			pi.numberOfElements = 1;

			unsigned int sz = paths.size();
			bool newone = true;
			for(unsigned int p = 0; p < sz; ++p)
			{
				if(paths[p].size == pi.size)
				{
					paths[p].numberOfElements++;
					paths[p].numberOfNodes += pi.numberOfNodes;
					newone = false;
					break;
				}
			}
			if(newone)
			{
				paths.push_back(pi);
			}
		}
	}
	QueryPerformanceCounter(&newCount);
	double diffInMilliSeconds = ((newCount.QuadPart - oldCount.QuadPart)*1000.0/freq.QuadPart);
	/// sort the paths by sizes
	std::sort(paths.begin(), paths.end(), sortSolutionPathsBySize());
	/// display same solution sizes (nodes generated) average
	unsigned int sz = paths.size();
	(Wrapper::Instance()).stressTestStr = " \n";
	for(unsigned int ss = 0; ss < sz; ++ss)
	{
		std::stringstream ss1,ss2,ss3; 
		std::string str1,str2,str3;
		ss1 << paths[ss].size;
		ss1 >> str1;
		ss2 << paths[ss].numberOfElements;
		ss2 >> str2;
		ss3 << (paths[ss].numberOfNodes / paths[ss].numberOfElements);
		ss3 >> str3;
		(Wrapper::Instance()).stressTestStr+= ("Size: " + str1 + " - Problems: " + str2 + " - Avg nodes: " + str3  + " \n");
	}
	std::stringstream ss1,ss2; 
	std::string str1,str2;
	ss1 << numberOfDifferentSolutions;
	ss1 >> str1;
	ss2 << diffInMilliSeconds;
	ss2 >> str2;
	(Wrapper::Instance()).stressTestStr+=("Time taken to solve " +  str1 + " different solutions = " + str2 + " milliSeconds");
	std::cout<<(Wrapper::Instance()).stressTestStr;
	delete [] array;
	np->SetState(saveState,np->GetTilesNumber()+1);
}

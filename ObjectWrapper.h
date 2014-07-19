/******************************************************************************/
/*!
\file ObjectWrapper.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
Interface for Wrapper of geometric objects and their transforms for this project
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/

#ifndef OBJECTWRAPPER_H
#define OBJECTWRAPPER_H

#include "ObjectGrid.h"
#include "Camera.h"
#include "GameObject.h"
#include "AIUtilities.h"
#include "Npuzzle.h"
#include <sstream>

struct PathInfo
{
	PathInfo() {size = 0; numberOfNodes = 0; numberOfElements = 0;}
	unsigned int size;
	unsigned int numberOfNodes;//total of all elements
	unsigned int numberOfElements;//of the same size
};

struct sortSolutionPathsBySize 
{
	bool operator() (PathInfo &p1,PathInfo &p2) 
	{ 
		return (p1.size<p2.size);
	}
};

//!  Wrapper class
/*!
Class for implmenting wrapper of geomtric objects and transforms
*/
class Wrapper
{
	static Wrapper *s_pInstance;	/*! Pointer to a Wrapper object which is static*/
	int count;	/*! Variable to keep track of total number of geometric objects*/
	Wrapper();
	~Wrapper();
	Wrapper(Wrapper &);
	Wrapper & operator = (Wrapper &);
public:
	Grid grid_2x2[4];	/*!< Geometric Grid for 2x2 */
	Grid grid_3x3[9];	/*!< Geometric Grid for 3x3*/
	Grid grid_misc[8];	/*!< Geometric Grid for misc boxes */
	Translate t_2x2[4];	/*!< Translate Object for 2x2 */
	Translate t_3x3[9];	/*!< Translate Object for 3x3 */
	Translate t_misc[8];	/*!< Translate Object for misc boxes */
	Scale s[2];		/*!< Scale Object for misc boxes */
	GameObject *tiles_2x2[4];	/*!< Gameobjects for 2x2 */
	GameObject *tiles_3x3[9];	/*!< Gameobjects for 3x3 */
	GameObject *tiles_misc[8];	/*!< Gameobjects for misc boxes */
	unsigned int state_2x2[4];	/*!< State of a 2x2 puzzle*/
	unsigned int state_3x3[9];	/*!< State of a 3x3 puzzle */
	int state;	/*!< Game state - in 2x2 or 3x3 */
	int gameInSlidingState;	/*!< Represents if game is in sliding state */
	float speed;	/*!< Speed of the moving tiles */
	float dt;	/*!< Frame time */
	int slidingTileIndex;	/*!< Index of the sliding tile*/
	int zeroCellPos;	/*!< Position of the zeroeth Cell */
	ModelSpace mSpace;	/*!< ModelSpace used */
	std::string animationSpeed,algorithmUsed,nodesGenerated,timeTaken,solutionSteps,stressTestStr; /*!< Strings used for display */
	NPuzzle *np_2x2,*np_3x3;	/*!< Npuzzle pointer */
	int gameinAIMode;	/*!< Represents if game is in AI or user mode*/
	SEARCH_METHOD sm;	/*!< Search Method Used */
	std::vector<unsigned int *> sol;	/*!< vector of solutions */
	std::vector<unsigned int *>::iterator it1,it2;	/*!< Iterators to the solution vector*/
	std::vector<PathInfo> paths;	/*!< Path objects used for stats*/
	Camera camera;	/*!< Camera object of the project */
	static void create();
	static void destroy();
	static Wrapper &Instance();
	void createGeometries();
	void uploadGeometries();
	void destroyGeometries();
	void createTiles();
	void destroyTiles();
	void InitializeTiles();
	void RandomizeTiles();
	void processInput(const float &posX,const float &posY);
	int checkClickOnTile(const float &posX,const float &posY);
	bool checkPossibleMove(int index);
	void moveTileToEmptyCell();
	void slideTile();
	void StressTest();
};



#endif
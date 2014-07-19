/******************************************************************************/
/*!
\file NPuzzle.h
\author Anshuman Wadhera
\par email: a.anshumanwadhera\@digipen.edu
\par a.anshumanwadhera 900995
\par Course: CS1150
\par Project #01
\date 21/02/2012
\brief
NPuzzle header file
Copyright (C) 2012 DigiPen Institute of Technology Singapore
*/
/******************************************************************************/


#ifndef NPUZZLE_H
#define NPUZZLE_H

#include <vector>
#include <stack>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <queue>
#include <set>

//!  Puzzle kind enum
/*!
enum for type of state space used
*/
enum PUZZLE_KIND{ NORMAL, CIRCULAR };

//!  Search method enum
/*!
enum for type of search method used
*/
enum SEARCH_METHOD{ DFS=0, ASTAR=1 };

//!  MyNode struct
/*!
Node generated in search tree
*/
struct MyNode
{
	MyNode() {m_data = 0; m_parent = 0; m_currentActualCost = 0; m_totalCost = 0;}
	unsigned int * m_data; /*!< The array of integers represents the current order of the tiles in the puzzle. The last int (not for m_initialState) is to save the index in m_data of the hole in the puzzle. */
	MyNode * m_parent;	/*!< The parent is needed to always have track of the current optimal solution and to be able to extract the result */
	unsigned int m_currentActualCost; /*!< Actual cost to reach this node state from the start (needed to save this variable separately because children nodes need this information) */
	unsigned int m_totalCost; /*!< = actual cost to reach this node state from the start + estimated heuristic cost from current node state to the goal state */
};

//! MyStatistics struct
/*!
Statistics structure
*/
struct MyStatistics
{
	MyStatistics() {m_nodesGenerated = 0;}
	unsigned int m_nodesGenerated; /*!<How many times we created successful new nodes (that are added to the frontier) while doing one specific search */
};

//!  CompareNodesByCost struct
/*!
Used as a function object
*/
struct CompareNodesByCost
{
	bool operator()( const MyNode* node1, const MyNode* node2 ) const 
	{
		return node1->m_totalCost > node2->m_totalCost;
	}
};

//!  checkAlreadyPresent struct
/*!
Used as a function object
*/
struct checkAlreadyPresent
{
	unsigned int bufferSize;	/*!< size of buffer */
	checkAlreadyPresent(unsigned int size) : bufferSize(size)
	{
	}
	bool operator() (MyNode *m1,MyNode *m2)
	{
		for(unsigned int i = 0;i < bufferSize; ++i)
		{
			if(m1->m_data[i]!=m2->m_data[i])
				return false;
		}
		return true;
	}
};

//!  checkEqualCost struct
/*!
Used as a function object
*/
struct checkEqualCost
{
	bool operator() (MyNode *m1,MyNode *m2)
	{
		return(m1->m_totalCost == m2->m_totalCost);
	}
};

//!  NPuzzle class
/*!
Npuzzle class
*/
class NPuzzle
{
	unsigned int m_totalTilesNumber;	/*!< This variable represents the number of tiles including the empty space hole tile */
	unsigned int m_stateSize; /*!< m_totalTilesNumber + 1 (1 is for the last integer that holds the hole position). It represents the current state in a search algorithm. */

	MyNode * m_initialState; /*!< m_totalTilesNumber. This integers buffer saves always what was the latest user input (using SetState function).*/
	MyNode * m_currentState; /*!< m_stateSize = m_totalTilesNumber + 1 (1 is for the last integer that holds the hole position). This state will always change while a search is happening. It is the state that is being extracted from the frontier to be tested and expanded if possible. */
	MyNode * m_targetState; /*!< m_stateSize = m_totalTilesNumber + 1 (1 is for the last integer that holds the hole position). This integers buffer holds the target state that will always be compared will a search is happening.  */

	std::vector<std::vector<unsigned int>> constraint;	/*!< vector to help specify the constraints of the puzzle */
	std::vector<MyNode *> Frontier;	/*!< Frontier of the search during DFS*/
	std::vector<MyNode *> Explored; /*!< Explored data of the search */
	std::vector<unsigned int *> Solution;	/*!< vector of solutions */
	int **distanceHeuristic;	/*!< Multi dimensional array to help with heuristic */
	std::multiset<MyNode *,CompareNodesByCost> a_Frontier;	/*!< Frontier used for AStar */
	MyStatistics *stats;	/*!< Statistics */
public:
	NPuzzle(unsigned sideSize, PUZZLE_KIND puzzleKind = NORMAL);
	~NPuzzle();
	bool SetState(const unsigned int * state, unsigned int bufferSize);//This function will set state as the initial current internal state
	const unsigned int * GetState();
	bool SearchForSolution(unsigned int searchMethod);
	bool CheckLegalTilesPositions(const unsigned int * array, PUZZLE_KIND puzzleKind = NORMAL);
	unsigned int GetTilesNumber();
	unsigned int GetSideSize();
	std::vector<unsigned int *> & GetSolution();
	MyStatistics * GetStatistics();
	bool depthFirstSearch();
	bool aStarSearch();
	unsigned int NPuzzle::calculateHeuristicCost(unsigned int *state,unsigned int bufferSize);
};



#endif
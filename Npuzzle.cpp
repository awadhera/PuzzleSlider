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

#include "Npuzzle.h"
#include <cmath>

/******************************************************************************/
/*!
NPuzzle constructor
\param [in] sideSize
Size of the puzzle
\param [in] PUZZLE_KIND
Type of state space to be used
*/
/******************************************************************************/
NPuzzle::NPuzzle(unsigned sideSize, PUZZLE_KIND)
{
	m_totalTilesNumber = sideSize*sideSize;
	m_stateSize = m_totalTilesNumber + 1;
	m_initialState = new MyNode();
	m_currentState = new MyNode();
	m_targetState = new MyNode();
	unsigned int i;
	m_targetState->m_data = new unsigned int[m_stateSize];
	int *lengthLimit = new int[sideSize];
	int initial = (sideSize-1)*(-1);
	distanceHeuristic = new int* [m_totalTilesNumber];
	for(unsigned int i = 0;i < m_totalTilesNumber;++i)
	{
		distanceHeuristic[i] = new int[m_totalTilesNumber];
	}
	for(unsigned int i = 0,j = 2;i < sideSize;++i)
	{
		lengthLimit[i] = initial + j*i;
	}
	std::vector< std::pair<int,int> > imagCoord;
	std::vector< std::pair<int,int> >::iterator it;
	for(unsigned int i = 0;i < sideSize; ++i)
	{
		for(unsigned int j = 0;j < sideSize; ++j)
		{
			imagCoord.push_back(std::make_pair(lengthLimit[j],lengthLimit[sideSize-(i+1)]));
		}
	}

	std::pair<int,int> coord1,coord2;
	for(unsigned int i = 0; i < sideSize*sideSize; ++i)
	{
		for(unsigned int j = 0; j < sideSize*sideSize ; ++j)
		{
			coord1 = imagCoord[i];
			coord2 = imagCoord[j];
			distanceHeuristic[i][j] = abs(coord2.second - coord1.second) + abs(coord2.first - coord1.first);
			distanceHeuristic[i][j]/=2;
		}
	}	
	for(unsigned int i = 0; i < sideSize*sideSize;++i)
	{
		std::vector<unsigned int> row;
		for(unsigned int j = 0; j < sideSize*sideSize ; ++j)
		{
			coord1 = imagCoord[i];
			coord2 = imagCoord[j];
			if(coord1 == coord2)
				continue;
			else if(abs(coord1.first - coord2.first) == 2 && abs(coord2.second - coord1.second) == 0)
				row.push_back(j);
			else if(abs(coord1.second - coord2.second) == 2 && abs(coord2.first - coord1.first) == 0)
				row.push_back(j);
		}
		constraint.push_back(row);
	}
	for(i = 0; i < m_totalTilesNumber; ++i)
	{
		m_targetState->m_data[i] = i;
	}
	m_targetState->m_data[i] = 0;
	stats = new MyStatistics;
}

/******************************************************************************/
/*!
NPuzzle Destructor
*/
/******************************************************************************/
NPuzzle::~NPuzzle()
{	
	Frontier.clear();
	Explored.clear();
	Solution.clear();
	a_Frontier.clear();
	for(unsigned int i = 0;i < m_totalTilesNumber; ++i)
	{
		delete [] distanceHeuristic[i];
	}
	delete [] distanceHeuristic;
	delete [] m_initialState;
	delete [] m_currentState;
	delete [] m_targetState;
	delete stats;
}

/******************************************************************************/
/*!
NPuzzle::SetState - Set the state to the one specified in argument
\param [in]	state
State of the puzzle given as input
\param [in] bufferSize
Size of the state array
\return
bool
*/
/******************************************************************************/
bool NPuzzle::SetState(const unsigned int *state,unsigned int bufferSize)
{
	m_initialState->m_data = new unsigned int[m_totalTilesNumber];
	if(!m_initialState->m_data)
	{
		return false;
	}	
	unsigned int index = 0;
	m_currentState->m_data = new unsigned int[m_stateSize];
	for(unsigned int i = 0;i < bufferSize; ++i)
	{
		m_initialState->m_data[i] = state[i];
		if(state[i] == 0)
			index = i;
		m_currentState->m_data[i] = m_initialState->m_data[i];

	}
	m_currentState->m_data[bufferSize] = index; 
	m_initialState->m_currentActualCost = 0;
	m_initialState->m_totalCost = m_initialState->m_currentActualCost + calculateHeuristicCost(m_initialState->m_data,bufferSize);
	m_currentState->m_currentActualCost = m_initialState->m_currentActualCost;
	m_currentState->m_totalCost = m_initialState->m_totalCost;
	Frontier.clear();
	Explored.clear();
	Solution.clear();
	a_Frontier.clear();
	return true;
}

/******************************************************************************/
/*!
NPuzzle GetState - Get the current state of the puzzle
\return
const unsigned int *
*/
/******************************************************************************/
const unsigned int * NPuzzle::GetState()
{
	return m_currentState->m_data;
}

/******************************************************************************/
/*!
NPuzzle::GetTilesNumber - Get the tiles number from puzzle
\return
unsigned int
*/
/******************************************************************************/
unsigned int NPuzzle::GetTilesNumber()
{
	return m_totalTilesNumber-1;
}

/******************************************************************************/
/*!
NPuzzle::GetSideSize - Get the side size of the puzzle
\return
unsigned int
*/
/******************************************************************************/
unsigned int NPuzzle::GetSideSize()
{
	return (unsigned int(std::sqrt(float(m_totalTilesNumber))));
}

/******************************************************************************/
/*!
NPuzzle::CheckLegalTilesPositions
\param [in]	array
input array to be check for legality
\param [in] PUZZLE_KIND
Kind of puzzle used
\return
bool
*/
/******************************************************************************/
bool NPuzzle::CheckLegalTilesPositions(const unsigned int *array, PUZZLE_KIND)
{
	unsigned int i,j,sum,sumForEachTile;
	sum = 0;
	for(i = 0;i < m_totalTilesNumber;++i)
	{
		sumForEachTile = 0;
		for(j = i+1; j < m_totalTilesNumber;++j)
		{
			if((array[j]<array[i]) && (array[j]!=0))
				++sumForEachTile;
		}
		sum+=sumForEachTile;
	}
	if(sum%2 == 0)
		return true;
	else
		return false;
}

/******************************************************************************/
/*!
NPuzzle::GetSolution - Get the solution vector containing the states to success
\return
vector<unsigned int *>
*/
/******************************************************************************/
std::vector<unsigned int *> & NPuzzle::GetSolution()
{
	return Solution;
}

/******************************************************************************/
/*!
NPuzzle::SearchForSolution - Search for solution given an input algorithm
\param [in]	searchMethod
0 for DFS, 1 for AStar
\return
bool
*/
/******************************************************************************/
bool NPuzzle::SearchForSolution(unsigned int searchMethod)
{
	bool solutionFound = false;
	if(searchMethod == 0)
	{
		solutionFound = depthFirstSearch();
	}
	else
	{
		solutionFound = aStarSearch();
	}
	return solutionFound;
}

/******************************************************************************/
/*!
NPuzzle::depthFirstSearch - Do a depth first search for solution
\return
bool
*/
/******************************************************************************/
bool NPuzzle::depthFirstSearch()
{
	int flag = 0;
	stats->m_nodesGenerated = 1;
	for(unsigned int i = 0;i < m_totalTilesNumber; ++i)
	{
		if(m_initialState->m_data[i] !=  m_targetState->m_data[i])
		{
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		Solution.push_back(m_initialState->m_data);
		return true;
	}
	else
	{
		Frontier.push_back(m_currentState);
		bool solutionFound = false;
		MyNode *element;
		unsigned int zeroTileIndex;
		std::vector<unsigned int>::iterator it;
		while(!Frontier.empty())
		{
			element = Frontier.back();
			Frontier.pop_back();
			Explored.push_back(element);
			std::vector<MyNode *> Leaves;
			MyNode *leaf;
			unsigned int swap;
			zeroTileIndex = element->m_data[m_totalTilesNumber];
			for(it=constraint[zeroTileIndex].begin() ;it < constraint[zeroTileIndex].end(); ++it )
			{
				leaf = new MyNode;
				leaf->m_data = new unsigned int[m_stateSize];
				for(unsigned int i = 0;i < m_stateSize; ++i)
				{
					leaf->m_data[i] = element->m_data[i];
				}
				swap = leaf->m_data[zeroTileIndex];
				leaf->m_data[zeroTileIndex] = leaf->m_data[*it];
				leaf->m_data[*it] = swap;
				leaf->m_data[m_totalTilesNumber] = *it;
				leaf->m_parent = element;
				++stats->m_nodesGenerated;
				if(m_totalTilesNumber == 4)
				{
					Leaves.push_back(leaf);	
				}
				else if(CheckLegalTilesPositions(leaf->m_data))
				{
					Leaves.push_back(leaf);	
				}
			}
			std::vector<MyNode *>::iterator LeafIter,FrontierIter,ExploredIter;
			unsigned int FrontierHelpCount,ExploredHelpCount;
			for(LeafIter = Leaves.begin();LeafIter != Leaves.end(); ++LeafIter)
			{		
				FrontierHelpCount = 0;
				ExploredHelpCount = 0;
				for(FrontierIter = Frontier.begin();FrontierIter != Frontier.end(); ++FrontierIter)
				{
					for(unsigned int i = 0;i < m_stateSize; ++i)
					{
						if((*LeafIter)->m_data[i] != (*FrontierIter)->m_data[i])
						{
							FrontierHelpCount++;
							break;
						}
					}
				}
				if(FrontierHelpCount == Frontier.size())
				{
					for(ExploredIter = Explored.begin();ExploredIter != Explored.end(); ++ExploredIter)
					{	
						for(unsigned int i = 0;i < m_stateSize; ++i)
						{	
							if((*LeafIter)->m_data[i] != (*ExploredIter)->m_data[i])
							{
								ExploredHelpCount++;
								break;
							}
						}
					}
					if(ExploredHelpCount == Explored.size())
					{
						int goalInsert = 1;
						for(unsigned int i = 0;i < m_stateSize; ++i)
						{
							if((*LeafIter)->m_data[i] != m_targetState->m_data[i])
							{
								goalInsert = 0;
								break;
							}
						}
						if(goalInsert!=1)
						{
							Frontier.push_back(*LeafIter);
						}
						else
						{
							MyNode *node = *LeafIter;
							do
							{
								Solution.push_back(node->m_data);
								node = node->m_parent;
							}while(node!=0);
							std::reverse(Solution.begin(),Solution.end());
							solutionFound = true;
							return solutionFound;
						}
					}
				}
			}
		}
		return solutionFound;
	}
}

/******************************************************************************/
/*!
NPuzzle::aStarSearch - Do a AStar search for solution
\return
bool
*/
/******************************************************************************/
bool NPuzzle::aStarSearch()
{
	stats->m_nodesGenerated = 1;
	int flag = 0;
	for(unsigned int i = 0;i < m_totalTilesNumber; ++i)
	{
		if(m_initialState->m_data[i] !=  m_targetState->m_data[i])
		{
			flag = 1;
			break;
		}
	}
	if(flag == 0)
	{
		Solution.push_back(m_initialState->m_data);
		return true;
	}
	else
	{
		a_Frontier.insert(m_currentState);
		bool solutionFound = false;
		MyNode *element;
		unsigned int zeroTileIndex;
		std::vector<unsigned int>::iterator it;
		while(!a_Frontier.empty())
		{
			std::multiset<MyNode *,CompareNodesByCost>::iterator end;
			end = a_Frontier.end();
			end--;
			element = *end;
			a_Frontier.erase(end);
			int targetFound = 1;
			for(unsigned int i = 0;i < m_stateSize; ++i)
			{
				if(element->m_data[i] != m_targetState->m_data[i])
				{
					targetFound = 0;
					break;
				}
			}
			if(targetFound == 1)
			{
				do
				{
					Solution.push_back(element->m_data);
					element = element->m_parent;
				}while(element!=0);
				std::reverse(Solution.begin(),Solution.end());
				solutionFound = true;
				return solutionFound;
			}
			Explored.push_back(element);
			std::vector<MyNode *> Leaves;
			MyNode *leaf;
			unsigned int swap;
			zeroTileIndex = element->m_data[m_totalTilesNumber];
			for(it = constraint[zeroTileIndex].begin();it!= constraint[zeroTileIndex].end(); ++it)
			{

				leaf = new MyNode;
				leaf->m_data = new unsigned int[m_stateSize];
				for(unsigned int i = 0;i < m_stateSize; ++i)
				{
					leaf->m_data[i] = element->m_data[i];
				}
				swap = leaf->m_data[zeroTileIndex];
				leaf->m_data[zeroTileIndex] = leaf->m_data[*it];
				leaf->m_data[*it] = swap;
				leaf->m_data[m_totalTilesNumber] = *it;
				leaf->m_parent = element;
				leaf->m_currentActualCost = 1 + element->m_currentActualCost;
				leaf->m_totalCost = leaf->m_currentActualCost + calculateHeuristicCost(leaf->m_data,m_totalTilesNumber);
				++stats->m_nodesGenerated;
				if(m_totalTilesNumber == 4)
				{
					Leaves.push_back(leaf);
				}
				else if(CheckLegalTilesPositions(leaf->m_data))
				{
					Leaves.push_back(leaf);
				}
			}
			std::vector<MyNode *>::iterator LeafIter,ExploredIter;
			std::multiset<MyNode *,CompareNodesByCost>::iterator FrontierIterBegin;
			unsigned int ExploredHelpCount;
			for(LeafIter = Leaves.begin();LeafIter != Leaves.end(); ++LeafIter)
			{	
				ExploredHelpCount = 0;
				FrontierIterBegin = find_first_of(a_Frontier.begin(),a_Frontier.end(),&(*LeafIter),&(*LeafIter)+1,checkAlreadyPresent(m_totalTilesNumber));
				for(ExploredIter = Explored.begin();ExploredIter != Explored.end(); ++ExploredIter)
				{	
					for(unsigned int i = 0;i < m_stateSize; ++i)
					{		
						if((*LeafIter)->m_data[i] != (*ExploredIter)->m_data[i])
						{
							ExploredHelpCount++;
							break;
						}
					}
				}
				if(ExploredHelpCount == Explored.size() && FrontierIterBegin==a_Frontier.end())
				{
					a_Frontier.insert(*LeafIter);
				}
				else if((FrontierIterBegin!=a_Frontier.end()) && ((*FrontierIterBegin)->m_totalCost > ((*LeafIter)->m_totalCost)))
				{
					a_Frontier.erase(FrontierIterBegin);
					a_Frontier.insert(*LeafIter);
				}
			}
		}
		return solutionFound;
	}
}

/******************************************************************************/
/*!
NPuzzle::calculateHeuristicCost - Find heuristic cost of the puzzle(current)
\param [in]	state
State of the puzzle given as input
\param [in] bufferSize
Size of the state array
\return
unsigned int
*/
/******************************************************************************/
unsigned int NPuzzle::calculateHeuristicCost(unsigned int *state,unsigned int bufferSize)
{
	unsigned int totalCost = 0;
	for(unsigned int i = 0;i < bufferSize;++i)
	{
		if(state[i]!=0)
		{
			totalCost+=distanceHeuristic[i][state[i]];
		}
	}
	return totalCost;
}

/******************************************************************************/
/*!
NPuzzle::MyStatistics - Retrieve the statistics
\return
MyStatistics *
*/
/******************************************************************************/
MyStatistics * NPuzzle::GetStatistics()
{
	return stats;
}
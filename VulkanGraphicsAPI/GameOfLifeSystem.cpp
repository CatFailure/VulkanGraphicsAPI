#include "GameOfLifeSystem.hpp"

namespace SolEngine::System
{
    void GameOfLifeSystem::CheckAllLiveNeighbours(SolGrid& rSolGrid)
    {
        const size_t    maxNodeIndexValue = MAX_CUBES_PER_AXIS_COUNT - 1;
        const glm::vec3 gridDimensions    = rSolGrid.GetDimensions();
        const float     gridStep          = rSolGrid.GetStep();
                                          
        rSolGrid.TraverseGrid([&](const uint32_t xIndex, 
                                  const uint32_t yIndex, 
                                  const uint32_t zIndex)
                              {
                                  Nodes& rGridNodes = rSolGrid.nodes;

                                  const size_t nodeIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        gridDimensions, 
                                                                        gridStep);

                                  unsigned char& rNodeLiveNeighbourCount = rGridNodes.pLiveNeighbourCounts[nodeIndex];
                                  rNodeLiveNeighbourCount = 0U;  // Reset the neighbour count

                                  rGridNodes.pXVertices[xIndex * CUBE_VERTEX_COUNT];

                                  // Is there a node to the left?
                                  if (xIndex > 0)
                                  {
                                      const size_t leftNeighbourIndex = _3DTo1DIndex(xIndex - 1, 
                                                                                     yIndex, 
                                                                                     zIndex, 
                                                                                     gridDimensions, 
                                                                                     gridStep);
                                      // Is this cell alive?
                                      if (rGridNodes.pCellStates[leftNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }

                                  // Is there a node to the right?
                                  if (xIndex < maxNodeIndexValue)
                                  {
                                      const size_t rightNeighbourIndex = _3DTo1DIndex(xIndex + 1, 
                                                                                      yIndex, 
                                                                                      zIndex, 
                                                                                      gridDimensions, 
                                                                                      gridStep);

                                      // Is this cell alive?
                                      if (rGridNodes.pCellStates[rightNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }

                                  // Is there a node above?
                                  if (yIndex > 0)
                                  {
                                      const size_t topNeighbourIndex = _3DTo1DIndex(xIndex, 
                                                                                    yIndex - 1, 
                                                                                    zIndex, 
                                                                                    gridDimensions, 
                                                                                    gridStep);
                                      if (rGridNodes.pCellStates[topNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }

                                  // Is there a node below?
                                  if (yIndex < maxNodeIndexValue)
                                  {
                                      const size_t belowNeighbourIndex = _3DTo1DIndex(xIndex, 
                                                                                      yIndex + 1, 
                                                                                      zIndex, 
                                                                                      gridDimensions, 
                                                                                      gridStep);

                                      if (rGridNodes.pCellStates[belowNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }

                                  // Is there a node to the back?
                                  if (zIndex > 0)
                                  {
                                      const size_t behindNeighbourIndex = _3DTo1DIndex(xIndex, 
                                                                                       yIndex, 
                                                                                       zIndex - 1, 
                                                                                       gridDimensions, 
                                                                                       gridStep);

                                      if (rGridNodes.pCellStates[behindNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }

                                  // Is there a node to the front?
                                  if (zIndex < maxNodeIndexValue)
                                  {
                                      const size_t frontNeighbourIndex = _3DTo1DIndex(xIndex, 
                                                                                      yIndex, 
                                                                                      zIndex + 1, 
                                                                                      gridDimensions, 
                                                                                      gridStep);

                                      if (rGridNodes.pCellStates[frontNeighbourIndex])
                                      {
                                          ++rNodeLiveNeighbourCount;
                                      }
                                  }
                              });
    }

    void GameOfLifeSystem::UpdateAllCellStates(SolGrid& rSolGrid)
    {
        const glm::vec3 gridDimensions = rSolGrid.GetDimensions();
        const float     gridStep       = rSolGrid.GetStep();

        rSolGrid.TraverseGrid([&](const uint32_t xIndex,
                                  const uint32_t yIndex,
                                  const uint32_t zIndex)
                              {
                                  Nodes& rGridNodes = rSolGrid.nodes;

                                  const size_t nodeIndex = _3DTo1DIndex(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        gridDimensions, 
                                                                        gridStep);

                                  // Nothing to do
                                  if (rGridNodes.pLiveNeighbourCounts[nodeIndex] == 0)
                                  {
                                      return;
                                  }

                                  // If alive
                                  if (rGridNodes.pCellStates[nodeIndex])
                                  {
                                      // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
                                      if (rGridNodes.pLiveNeighbourCounts[nodeIndex] < MIN_LIVE_NEIGHBOUR_COUNT)
                                      {
                                          rGridNodes.pCellStates[nodeIndex] = false;
                                      }

                                      // Any live cell with more than three live neighbours dies, as if by overpopulation.
                                      if (rGridNodes.pLiveNeighbourCounts[nodeIndex] > MAX_LIVE_NEIGHBOUR_COUNT)
                                      {
                                          rGridNodes.pCellStates[nodeIndex] = false;
                                      }

                                      // Any live cell with two or three live neighbours lives on to the next generation.
                                      return;
                                  }

                                  // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                                  if (rGridNodes.pLiveNeighbourCounts[nodeIndex] == MAX_LIVE_NEIGHBOUR_COUNT)
                                  {
                                      rGridNodes.pCellStates[nodeIndex] = true;
                                  }
                              });
    }

    void GameOfLifeSystem::Update(const float deltaTime, 
                                  SolGrid& rSolGrid)
    {
        if (_nextGenerationDelayRemaining > 0.f)
        {
            _nextGenerationDelayRemaining -= deltaTime;

            return;
        }

        UpdateAllCellStates(rSolGrid);
        CheckAllLiveNeighbours(rSolGrid);

        _nextGenerationDelayRemaining = NEXT_GENERATION_DELAY;
    }
}
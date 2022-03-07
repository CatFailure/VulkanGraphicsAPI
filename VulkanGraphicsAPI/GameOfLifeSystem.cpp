#include "GameOfLifeSystem.hpp"

namespace SolEngine::System
{
    GameOfLifeSystem::GameOfLifeSystem(SolGrid& rSolGrid)
        : _rSolGrid(rSolGrid)
    {}

    void GameOfLifeSystem::CheckAllLiveNeighbours()
    {
        const size_t    neighbourOffset          = 1U;
        const glm::vec3 gridDimensions           = _rSolGrid.GetDimensions();
        const float     gridStep                 = _rSolGrid.GetStep();
        const Cells&    gridNodes                = _rSolGrid.cells;
        const glm::vec3 scaledDimensions         = _rSolGrid.GetScaledDimensions();
        const glm::vec3 validNeighbourDimensions = scaledDimensions - glm::vec3(1);

        size_t neighbourIndex(0);
                                          
        _rSolGrid.TraverseGrid([&](const uint32_t xIndex, 
                                   const uint32_t yIndex, 
                                   const uint32_t zIndex)
                               {
                                   const size_t nodeIndex = _3DTo1DIndex(xIndex, 
                                                                         yIndex, 
                                                                         zIndex, 
                                                                         scaledDimensions);

                                   NeighbourCount_t& rLiveNeighbourCount = 
                                       gridNodes.pLiveNeighbourCounts[nodeIndex];

                                   rLiveNeighbourCount = 0U;  // Reset the neighbour count
                               
                                   // Is there a node to the left?
                                   if (xIndex > 0)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex - 1, 
                                                                     yIndex, 
                                                                     zIndex, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex, 
                                                           gridNodes.pCellStates,
                                                           rLiveNeighbourCount);
                                   }
                               
                                   // Is there a node to the right?
                                   if (xIndex < validNeighbourDimensions.x)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex + 1, 
                                                                     yIndex, 
                                                                     zIndex, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex,
                                                           gridNodes.pCellStates, 
                                                           rLiveNeighbourCount);
                                   }
                               
                                   // Is there a node above?
                                   if (yIndex > 0)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex, 
                                                                     yIndex - 1, 
                                                                     zIndex, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex,
                                                           gridNodes.pCellStates, 
                                                           rLiveNeighbourCount);
                                   }
                               
                                   // Is there a node below?
                                   if (yIndex < validNeighbourDimensions.y)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex, 
                                                                     yIndex + 1, 
                                                                     zIndex, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex,
                                                           gridNodes.pCellStates, 
                                                           rLiveNeighbourCount);
                                   }
                               
                                   // Is there a node to the back?
                                   if (zIndex > 0)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex, 
                                                                     yIndex, 
                                                                     zIndex - 1, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex,
                                                           gridNodes.pCellStates, 
                                                           rLiveNeighbourCount);
                                   }
                               
                                   // Is there a node to the front?
                                   if (zIndex < validNeighbourDimensions.z)
                                   {
                                       neighbourIndex = _3DTo1DIndex(xIndex, 
                                                                     yIndex, 
                                                                     zIndex + 1, 
                                                                     scaledDimensions);

                                       CheckNeighbourState(neighbourIndex,
                                                           gridNodes.pCellStates, 
                                                           rLiveNeighbourCount);
                                   }
                               });
    }

    void GameOfLifeSystem::UpdateAllCellStates()
    {
        //const glm::vec3 gridDimensions = _rSolGrid.GetDimensions();
        //const float     gridStep       = _rSolGrid.GetStep();
        const glm::vec3 scaledDimensions = _rSolGrid.GetScaledDimensions();

        _rSolGrid.TraverseGrid([&](const uint32_t xIndex,
                                   const uint32_t yIndex,
                                   const uint32_t zIndex)
                               {
                                   Cells& rGridNodes = _rSolGrid.cells;
                               
                                   const size_t cellIndex = _3DTo1DIndex(xIndex, 
                                                                         yIndex, 
                                                                         zIndex, 
                                                                         scaledDimensions);
                               
                                   bool&                  rCellState         = rGridNodes.pCellStates[cellIndex];
                                   const NeighbourCount_t cellNeighbourCount = rGridNodes.pLiveNeighbourCounts[cellIndex];
                               
                                   // Nothing to do
                                   if (cellNeighbourCount == 0)
                                   {
                                       return;
                                   }
                               
                                   // If alive
                                   if (rCellState)
                                   {
                                       // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
                                       if (cellNeighbourCount < MIN_LIVE_NEIGHBOUR_COUNT)
                                       {
                                           rCellState = false;
                                       }
                               
                                       // Any live cell with more than three live neighbours dies, as if by overpopulation.
                                       if (cellNeighbourCount > MAX_LIVE_NEIGHBOUR_COUNT)
                                       {
                                           rCellState = false;
                                       }
                               
                                       // Any live cell with two or three live neighbours lives on to the next generation.
                                       return;
                                   }
                               
                                   // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                                   if (cellNeighbourCount == MAX_LIVE_NEIGHBOUR_COUNT)
                                   {
                                        rCellState = true;
                                   }
                               });
    }

    void GameOfLifeSystem::Update(const float deltaTime)
    {
        if (_nextGenerationDelayRemaining > 0.f)
        {
            _nextGenerationDelayRemaining -= deltaTime;

            return;
        }

        UpdateAllCellStates();
        CheckAllLiveNeighbours();

        onUpdateAllCellStatesEvent.Invoke();

        _nextGenerationDelayRemaining = NEXT_GENERATION_DELAY;
    }

    void GameOfLifeSystem::CheckNeighbourState(const size_t neighbourIndex,
                                               const bool* pCellStates, 
                                               NeighbourCount_t& rLiveNeighbourCount)
    {
        // Is this cell alive?
        if (!pCellStates[neighbourIndex])
        {
            return;
        }

        ++rLiveNeighbourCount;
    }
}
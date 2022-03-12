#include "GameOfLifeSystem.hpp"

namespace SolEngine::System
{
    GameOfLifeSystem::GameOfLifeSystem(SolGrid& rSolGrid, 
                                       GameOfLifeSettings& rGameOfLifeSettings,
                                       SimulationSettings& rSimulationSettings)
        : _rSolGrid(rSolGrid),
          _rGameOfLifeSettings(rGameOfLifeSettings),
          _rSimulationSettings(rSimulationSettings),
          _nextGenerationDelayRemaining(rSimulationSettings.nextGenerationDelay)
    {}

    void GameOfLifeSystem::CheckAllCellNeighbours()
    {
        const uint32_t   neighbourOffset          = 1U;
        const Cells&     gridNodes                = _rSolGrid.cells;
        const glm::uvec3 gridDimensions           = _rSolGrid.GetDimensions();
        const glm::uvec3 validNeighbourDimensions = gridDimensions - glm::uvec3(1U);

        uint32_t neighbourIndex(0);
                                          
        _rSolGrid.TraverseAllGridCells([&](const uint32_t xIndex, 
                                           const uint32_t yIndex, 
                                           const uint32_t zIndex)
                                       {
                                           const uint32_t nodeIndex = _3DTo1DIndex(xIndex, 
                                                                                   yIndex, 
                                                                                   zIndex, 
                                                                                   gridDimensions);

                                           NeighbourCount_t& rLiveNeighbourCount = 
                                               gridNodes.pLiveNeighbourCounts[nodeIndex];

                                           rLiveNeighbourCount = 0U;  // Reset the neighbour count
                                       
                                           // Is there a node to the left?
                                           if (xIndex > 0)
                                           {
                                               CheckNeighbourState(xIndex - neighbourOffset, 
                                                                   yIndex,
                                                                   zIndex,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       
                                           // Is there a node to the right?
                                           if (xIndex < validNeighbourDimensions.x)
                                           {
                                               CheckNeighbourState(xIndex + neighbourOffset, 
                                                                   yIndex,
                                                                   zIndex,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       
                                           // Is there a node above?
                                           if (yIndex > 0)
                                           {
                                               CheckNeighbourState(xIndex, 
                                                                   yIndex - neighbourOffset,
                                                                   zIndex,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       
                                           // Is there a node below?
                                           if (yIndex < validNeighbourDimensions.y)
                                           {
                                               CheckNeighbourState(xIndex, 
                                                                   yIndex + neighbourOffset,
                                                                   zIndex,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       
                                           // Is there a node to the back?
                                           if (zIndex > 0)
                                           {
                                               CheckNeighbourState(xIndex, 
                                                                   yIndex,
                                                                   zIndex - neighbourOffset,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       
                                           // Is there a node to the front?
                                           if (zIndex < validNeighbourDimensions.z)
                                           {
                                               CheckNeighbourState(xIndex, 
                                                                   yIndex,
                                                                   zIndex + neighbourOffset,
                                                                   gridDimensions, 
                                                                   gridNodes.pCellStates, 
                                                                   rLiveNeighbourCount);
                                           }
                                       });
    }

    void GameOfLifeSystem::UpdateAllCellStates()
    {
        const NeighbourCount_t minLiveNeighbourCount          = _rGameOfLifeSettings.minLiveNeighbourCount;
        const NeighbourCount_t maxLiveNeighbourCount          = _rGameOfLifeSettings.maxLiveNeighbourCount;
        const NeighbourCount_t reproductionLiveNeighbourCount = _rGameOfLifeSettings.reproductionLiveNeighbourCount;

        const glm::uvec3 dimensions = _rSolGrid.GetDimensions();

        _rSolGrid.TraverseAllGridCells([&](const uint32_t xIndex,
                                           const uint32_t yIndex,
                                           const uint32_t zIndex)
                                       {
                                           Cells& rGridNodes = _rSolGrid.cells;
                                       
                                           const size_t cellIndex = _3DTo1DIndex(xIndex, 
                                                                                 yIndex, 
                                                                                 zIndex, 
                                                                                 dimensions);
                                       
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
                                               rCellState = !(cellNeighbourCount < minLiveNeighbourCount) && // Any live cell with fewer than minLiveNeighbourCount live neighbours dies, as if by underpopulation.
                                                            !(cellNeighbourCount > maxLiveNeighbourCount);   // Any live cell with more than maxLiveNeighbourCount live neighbours dies, as if by overpopulation.
                                       
                                               // Any live cell with minLiveNeighbourCount or maxLiveNeighbourCount 
                                               // live neighbours lives on to the next generation.
                                               return;
                                           }

                                           // Any dead cell with exactly reproductionLiveNeighbourCount live neighbours becomes a live cell, as if by reproduction.
                                           if (cellNeighbourCount != reproductionLiveNeighbourCount)
                                           {
                                               return;
                                           }

                                           rCellState = true;
                                       });

        onUpdateAllCellStatesEvent.Invoke();
    }

    void GameOfLifeSystem::Update(const float deltaTime)
    {
        if (_rSimulationSettings.simulationState != SimulationState::PLAY)
        {
            return;
        }

        if (_nextGenerationDelayRemaining > 0.f)
        {
            _nextGenerationDelayRemaining -= deltaTime;

            return;
        }

        NextGeneration();
    }

    void GameOfLifeSystem::ForceUpdateCellStates()
    {
        UpdateAllCellStates();
        CheckAllCellNeighbours();

        // Reset the delay
        _nextGenerationDelayRemaining = _rSimulationSettings.nextGenerationDelay;
    }

    void GameOfLifeSystem::CheckNeighbourState(const uint32_t xIndex,
                                               const uint32_t yIndex, 
                                               const uint32_t zIndex, 
                                               const glm::vec3& scaledDimensions, 
                                               const bool* pCellStates, 
                                               NeighbourCount_t& rLiveNeighbourCount)
    {
        const uint32_t neighbourIndex = 
            _3DTo1DIndex(xIndex, 
                         yIndex, 
                         zIndex, 
                         scaledDimensions);

        // Is this cell alive?
        if (!pCellStates[neighbourIndex])
        {
            return;
        }

        ++rLiveNeighbourCount;
    }

    void GameOfLifeSystem::NextGeneration()
    {
        ForceUpdateCellStates();

        ++_rSimulationSettings.generation;
    }
}
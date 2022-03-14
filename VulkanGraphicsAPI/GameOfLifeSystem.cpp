#include "GameOfLifeSystem.hpp"

namespace SolEngine::System
{
    GameOfLifeSystem::GameOfLifeSystem(SolGrid& rSolGrid, 
                                       GameOfLifeSettings& rGameOfLifeSettings,
                                       SimulationSettings& rSimulationSettings)
        : _rSolGrid(rSolGrid),
          _rGameOfLifeSettings(rGameOfLifeSettings),
          _rSimulationSettings(rSimulationSettings),
          _nextGenerationDelayRemaining(rSimulationSettings.speed)
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
                                           NeighbourCount_t liveNeighbourCount{ 0U };

                                           // Is there a node to the left?
                                           if (xIndex > 0)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex - neighbourOffset, 
                                                                                      yIndex,
                                                                                      zIndex,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Is there a node to the right?
                                           if (xIndex < validNeighbourDimensions.x)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex + neighbourOffset, 
                                                                                      yIndex,
                                                                                      zIndex,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Is there a node above?
                                           if (yIndex > 0)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex, 
                                                                                      yIndex - neighbourOffset,
                                                                                      zIndex,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Is there a node below?
                                           if (yIndex < validNeighbourDimensions.y)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex, 
                                                                                      yIndex + neighbourOffset,
                                                                                      zIndex,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Is there a node to the back?
                                           if (zIndex > 0)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex, 
                                                                                      yIndex,
                                                                                      zIndex - neighbourOffset,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Is there a node to the front?
                                           if (zIndex < validNeighbourDimensions.z)
                                           {
                                               liveNeighbourCount += IsNeighbourAlive(xIndex, 
                                                                                      yIndex,
                                                                                      zIndex + neighbourOffset,
                                                                                      gridDimensions, 
                                                                                      gridNodes.pCellStates);
                                           }

                                           // Update live neighbour count of cell
                                           const uint32_t cellIndex = _3DTo1DIndex(xIndex, 
                                                                                   yIndex, 
                                                                                   zIndex, 
                                                                                   gridDimensions);

                                           gridNodes.pLiveNeighbourCounts[cellIndex] = liveNeighbourCount;
                                       });
    }

    void GameOfLifeSystem::UpdateAllCellStates()
    {
        Cells&                 rGridNodes           = _rSolGrid.cells;
        const NeighbourCount_t underpopulationCount = _rGameOfLifeSettings.underpopulationCount;
        const NeighbourCount_t overpopulationCount  = _rGameOfLifeSettings.overpopulationCount;
        const NeighbourCount_t reproductionCount    = _rGameOfLifeSettings.reproductionCount;

        const glm::uvec3 dimensions = _rSolGrid.GetDimensions();

        _rSolGrid.TraverseAllGridCells([&](const uint32_t xIndex,
                                           const uint32_t yIndex,
                                           const uint32_t zIndex)
                                       {
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
                                               rCellState = !(cellNeighbourCount < underpopulationCount) && // Any live cell with fewer than minLiveNeighbourCount live neighbours dies, as if by underpopulation.
                                                            !(cellNeighbourCount > overpopulationCount);   // Any live cell with more than maxLiveNeighbourCount live neighbours dies, as if by overpopulation.
                                       
                                               // Any live cell with minLiveNeighbourCount or maxLiveNeighbourCount 
                                               // live neighbours lives on to the next generation.
                                               return;
                                           }

                                           // Any dead cell with exactly reproductionLiveNeighbourCount live neighbours becomes a live cell, as if by reproduction.
                                           if (cellNeighbourCount != reproductionCount)
                                           {
                                               return;
                                           }

                                           rCellState = true;
                                       });

        onUpdateAllCellStatesEvent.Invoke();
    }

    void GameOfLifeSystem::Update(const float deltaTime)
    {
        if (_rSimulationSettings.state != SimulationState::PLAY)
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
        ResetNextGenerationDelayRemaining();
    }

    void GameOfLifeSystem::ResetNextGenerationDelayRemaining()
    {
        _nextGenerationDelayRemaining = _rSimulationSettings.speed;
    }

    bool GameOfLifeSystem::IsNeighbourAlive(const uint32_t xIndex,
                                            const uint32_t yIndex, 
                                            const uint32_t zIndex, 
                                            const glm::vec3& scaledDimensions, 
                                            const bool* pCellStates)
    {
        const uint32_t neighbourIndex = _3DTo1DIndex(xIndex, 
                                                     yIndex, 
                                                     zIndex, 
                                                     scaledDimensions);

        // Is this cell alive?
        return pCellStates[neighbourIndex];
    }

    void GameOfLifeSystem::NextGeneration()
    {
        ForceUpdateCellStates();

        ++_rSimulationSettings.generation;
    }
}
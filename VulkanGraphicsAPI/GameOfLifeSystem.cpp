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
        const Cells&     gridCells                = _rSolGrid.cells;
        const bool*      pCellStates              = gridCells.pCellStates;
        const glm::uvec3 gridDimensions           = _rSolGrid.GetDimensions();
        const glm::uvec3 validNeighbourDimensions = gridDimensions - glm::uvec3(1U);
        const bool       areDiagonalsChecked      = _rGameOfLifeSettings.neighbourhoodType == NeighbourhoodType::MOORE_NEIGHBOURHOOD;

        uint32_t neighbourIndex(0);
                                          
        _rSolGrid.TraverseAllGridCells([&](const int xIndex, 
                                           const int yIndex, 
                                           const int zIndex)
                                       {
                                           const int cellIndex = _3DTo1DIndex(xIndex, 
                                                                              yIndex, 
                                                                              zIndex, 
                                                                              gridDimensions);

                                           if (cellIndex == -1)
                                           {
                                               // Cell Index is invalid, don't bother checking anything...

                                               return;
                                           }

                                           NeighbourCount_t& rLiveNeighbourCount = 
                                               gridCells.pLiveNeighbourCounts[cellIndex];

                                           rLiveNeighbourCount = 0U;  // Reset the neighbour count

                                           CheckAdjacentNeighbourStates(xIndex,
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        gridDimensions, 
                                                                        pCellStates, 
                                                                        rLiveNeighbourCount);

                                           if (!areDiagonalsChecked)
                                           {
                                               return;
                                           }

                                           CheckDiagonalNeighbourStates(xIndex, 
                                                                        yIndex, 
                                                                        zIndex, 
                                                                        gridDimensions, 
                                                                        pCellStates, 
                                                                        rLiveNeighbourCount);
                                       });
    }

    void GameOfLifeSystem::UpdateAllCellStates()
    {
        const NeighbourCount_t underpopulationCount = _rGameOfLifeSettings.underpopulationCount;
        const NeighbourCount_t overpopulationCount  = _rGameOfLifeSettings.overpopulationCount;
        const NeighbourCount_t reproductionCount    = _rGameOfLifeSettings.reproductionCount;

        const glm::uvec3 dimensions = _rSolGrid.GetDimensions();

        _rSolGrid.TraverseAllGridCells([&](const int xIndex,
                                           const int yIndex,
                                           const int zIndex)
                                       {
                                           Cells& rGridNodes = _rSolGrid.cells;
                                       
                                           const int cellIndex = _3DTo1DIndex(xIndex, 
                                                                              yIndex, 
                                                                              zIndex, 
                                                                              dimensions);

                                           if (cellIndex == -1)
                                           {
                                               return;
                                           }
                                       
                                           bool&                  rIsCellAlive       = rGridNodes.pCellStates[cellIndex];
                                           const NeighbourCount_t cellNeighbourCount = rGridNodes.pLiveNeighbourCounts[cellIndex];
                                       
                                           // Nothing to do - guarenteed dead
                                           if (cellNeighbourCount == 0)
                                           {
                                               rIsCellAlive = false;

                                               return;
                                           }

                                           if (rIsCellAlive)
                                           {
                                               rIsCellAlive = !(cellNeighbourCount < underpopulationCount ||    // Any live cell with fewer than underpopulationCount live neighbours dies, as if by underpopulation.
                                                                cellNeighbourCount > overpopulationCount);      // Any live cell with more than overpopulationCount live neighbours dies, as if by overpopulation.

                                               return;
                                           }

                                           // Any dead cell with exactly reproductionLiveNeighbourCount live neighbours becomes a live cell, as if by reproduction.
                                           if (cellNeighbourCount != reproductionCount)
                                           {
                                               return;
                                           }

                                           rIsCellAlive = true;
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

    inline void GameOfLifeSystem::CheckAdjacentNeighbourStates(const int xIndex,
                                                               const int yIndex, 
                                                               const int zIndex, 
                                                               const glm::uvec3& gridDimensions, 
                                                               const bool* pCellStates, 
                                                               NeighbourCount_t& rLiveNeighbourCount)
    {
        const uint32_t neighbourOffset = 1U;
        const int      leftCellIndex   = xIndex - neighbourOffset;
        const int      rightCellIndex  = xIndex + neighbourOffset;
        const int      aboveCellIndex  = yIndex - neighbourOffset;
        const int      belowCellIndex  = yIndex + neighbourOffset;
        const int      backCellIndex   = zIndex - neighbourOffset;
        const int      frontCellIndex  = zIndex + neighbourOffset;

        // Left Cell Neighbour
        CheckNeighbourState(leftCellIndex, 
                            yIndex,
                            zIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Right Cell Neighbour
        CheckNeighbourState(rightCellIndex, 
                            yIndex,
                            zIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Above Cell Neighbour
        CheckNeighbourState(xIndex, 
                            aboveCellIndex,
                            zIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Below Cell Neighbour
        CheckNeighbourState(xIndex, 
                            belowCellIndex,
                            zIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Back Cell Neighbour
        CheckNeighbourState(xIndex, 
                            yIndex,
                            backCellIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Front Cell Neighbour
        CheckNeighbourState(xIndex, 
                            yIndex,
                            frontCellIndex,
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);
    }

    inline void GameOfLifeSystem::CheckDiagonalNeighbourStates(const int xIndex, 
                                                               const int yIndex, 
                                                               const int zIndex, 
                                                               const glm::uvec3& gridDimensions,
                                                               const bool* pCellStates, 
                                                               NeighbourCount_t& rLiveNeighbourCount)
    {
        const uint32_t neighbourOffset = 1U;
        const int      leftCellIndex   = xIndex - neighbourOffset;
        const int      rightCellIndex  = xIndex + neighbourOffset;
        const int      aboveCellIndex  = yIndex - neighbourOffset;
        const int      belowCellIndex  = yIndex + neighbourOffset;
        const int      backCellIndex   = zIndex - neighbourOffset;
        const int      frontCellIndex  = zIndex + neighbourOffset;

        // Back-Top-Left Cell Neighbour
        CheckNeighbourState(leftCellIndex, 
                            aboveCellIndex, 
                            backCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Back-Top-Right Cell Neighbour
        CheckNeighbourState(rightCellIndex, 
                            aboveCellIndex, 
                            backCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Back-Bottom-Left Cell Neighbour
        CheckNeighbourState(leftCellIndex, 
                            belowCellIndex, 
                            backCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Back-Bottom-Right Cell Neighbour
        CheckNeighbourState(rightCellIndex, 
                            belowCellIndex, 
                            backCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Front-Top-Left Cell Neighbour
        CheckNeighbourState(leftCellIndex, 
                            aboveCellIndex, 
                            frontCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Front-Top-Right Cell Neighbour
        CheckNeighbourState(rightCellIndex, 
                            aboveCellIndex, 
                            frontCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Front-Bottom-Left Cell Neighbour
        CheckNeighbourState(leftCellIndex, 
                            belowCellIndex, 
                            frontCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);

        // Front-Bottom-Right Cell Neighbour
        CheckNeighbourState(rightCellIndex, 
                            belowCellIndex, 
                            frontCellIndex, 
                            gridDimensions, 
                            pCellStates, 
                            rLiveNeighbourCount);
    }

    void GameOfLifeSystem::CheckNeighbourState(const int xIndex,
                                               const int yIndex, 
                                               const int zIndex, 
                                               const glm::uvec3& gridDimensions, 
                                               const bool* pCellStates, 
                                               NeighbourCount_t& rLiveNeighbourCount)
    {
        const int neighbourIndex = 
            _3DTo1DIndex(xIndex, 
                         yIndex, 
                         zIndex, 
                         gridDimensions);

        if (neighbourIndex == -1)
        {
            return;
        }

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
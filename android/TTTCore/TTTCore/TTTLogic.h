#pragma once
#include <cstdlib>
#include <algorithm>
#include <array>
#include <memory>
#include <cstdint>

enum class Mark : int
{
    EMPTY = 0, CROSS = 1, NOUGHT = 2
};

enum Result : std::int8_t
{
    WIN     = 1,
    DRAW    = 0,
    LOSS    = -1
};

template <std::size_t SIDELEN> class Grid
{
public:
    typedef Grid<SIDELEN> My_t;
    enum
    {
        SIZE = SIDELEN*SIDELEN
    };
    Grid() noexcept
    {
        Clear();
    }

    Grid(const My_t&) = default;
    Grid(My_t&&) = default;
    My_t& operator=(const My_t&) = default;
    My_t& operator=(My_t&&) = default;

    Mark At(std::size_t index) const noexcept
    {
        return m_board[index];
    }
    Mark At(std::size_t row, std::size_t col) const noexcept
    {
        return m_board[row * SIDELEN + col];
    }
    void SetCross(std::size_t row, std::size_t col) noexcept
    {
        m_board[row*SIDELEN + col] = Mark::CROSS;
    }
    void SetCross(std::size_t index) noexcept
    {
        m_board[index] = Mark::CROSS;
    }
    void SetNought(std::size_t row, std::size_t col) noexcept
    {
        m_board[row*SIDELEN + col] = Mark::NOUGHT;
    }
    void SetNought(std::size_t index) noexcept
    {
        m_board[index] = Mark::NOUGHT;
    }

    void Clear() noexcept
    {
        m_board.fill(Mark::EMPTY);
    }
    bool IsFilled() const noexcept
    {
        return m_board.cend() == std::find_if(m_board.cbegin(), m_board.cend(), 
                         [](Mark m) {return m == Mark::EMPTY; }
        );
    }
    Mark IsWon() const noexcept
    {
        bool all_cross, all_nought;

        for (std::size_t row = 0; row < SIDELEN; ++row) {
            all_cross = all_nought = true;
            for (std::size_t col = 0; col < SIDELEN; ++col)
                MightWinAt(At(row, col), &all_cross, &all_nought);
            if (all_cross)  return Mark::CROSS;
            if (all_nought) return Mark::NOUGHT;
        }

        for (std::size_t col = 0; col < SIDELEN; ++col) {
            all_cross = all_nought = true;
            for (std::size_t row = 0; row < SIDELEN; ++row)
                MightWinAt(At(row, col), &all_cross, &all_nought);
            if (all_cross)  return Mark::CROSS;
            if (all_nought) return Mark::NOUGHT;
        }

        all_cross = all_nought = true;
        for (std::size_t i = 0; i < SIDELEN*SIDELEN; i += (SIDELEN + 1))
            MightWinAt(At(i), &all_cross, &all_nought);
        if (all_cross)  return Mark::CROSS;
        if (all_nought) return Mark::NOUGHT;

        all_cross = all_nought = true;
        for (std::size_t i = SIDELEN-1; i < SIDELEN*SIDELEN - 1; i += (SIDELEN - 1))
            MightWinAt(At(i), &all_cross, &all_nought);
        if (all_cross)  return Mark::CROSS;
        if (all_nought) return Mark::NOUGHT;

        return Mark::EMPTY;
    }


private:
    void MightWinAt(Mark temp, bool *pallCross, bool *pallNought) const noexcept
    {
        if (temp == Mark::EMPTY)
            *pallCross = *pallNought = false;
        else if (temp == Mark::CROSS)
            *pallNought = false;
        else /*if (temp == Mark::nought) */
            *pallCross = false;
    }
    std::array<Mark, SIZE> m_board;
};

template<class TGrid> class ABPruning;

template<std::size_t SIDELEN>
class ABPruning<Grid<SIDELEN>>
{
public:
    typedef Grid<SIDELEN> Grid_t;
    explicit ABPruning(Mark side) : m_maxSide(side)
    { }
    Result Run(const Grid_t& grid, Mark turn, Result alpha, Result beta) const
    {
        // Terminal node
        Mark winner = grid.IsWon();
        if (winner != Mark::EMPTY)
            return winner == m_maxSide ? WIN : LOSS;
        else if (grid.IsFilled())
            return DRAW;

        // Intermediate node
        if (turn == m_maxSide) {
            Result bestVal = LOSS;
            for (std::size_t index = 0; index < SIDELEN*SIDELEN; ++index) {
                if (grid.At(index) != Mark::EMPTY)
                    continue;

                Grid_t newg(grid);
                if (turn == Mark::CROSS)    newg.SetCross(index);
                else                        newg.SetNought(index);

                Result res = Run(newg, ((turn == Mark::CROSS) ? Mark::NOUGHT : Mark::CROSS), alpha, beta);
                if (res > bestVal) bestVal = res;

                if (bestVal > alpha) alpha = bestVal;
                if (beta <= alpha)
                    break;
            }
            return bestVal;
        }
        else {
            Result bestVal = WIN;
            for (int index = 0; index < SIDELEN*SIDELEN; ++index) {
                if (grid.At(index) != Mark::EMPTY)
                    continue;

                Grid_t newg(grid);
                if (turn == Mark::CROSS)    newg.SetCross(index);
                else                        newg.SetNought(index);

                Result res = Run(newg, ((turn == Mark::CROSS) ? Mark::NOUGHT : Mark::CROSS), alpha, beta);
                if (res < bestVal)	bestVal = res;

                if (bestVal < beta) beta = bestVal;
                if (beta <= alpha)
                    break;
            }
            return bestVal;
        }
    }
private:
    const Mark m_maxSide;
};

template <std::size_t SIDELEN>
class Gameplay
{
public:
    typedef Grid<SIDELEN> Grid_t;

    std::unique_ptr<Grid_t> pGrid;

    Mark AiSide;

    Gameplay() : pGrid(std::make_unique<Grid_t>()), AiSide(Mark::EMPTY)
    { }

    void AiTurn()
    {
        ABPruning<Grid_t> alg(AiSide);

        auto pg = std::make_unique<Grid_t>(*pGrid);

        // win
        for (std::size_t i = 0; i < SIDELEN*SIDELEN; ++i) {
            if (pGrid->At(i) != Mark::EMPTY)
                continue;

            *pg = *pGrid;
            if (AiSide == Mark::CROSS)  pg->SetCross(i);
            else                        pg->SetNought(i);

            if (alg.Run(*pg, (AiSide == Mark::CROSS) ? Mark::NOUGHT : Mark::CROSS, LOSS, WIN) == WIN) {
                std::swap(pGrid, pg);
                return;
            }
        }
        // draw
        for (std::size_t i = 0; i < SIDELEN*SIDELEN; ++i) {
            if (pGrid->At(i) != Mark::EMPTY)
                continue;

            *pg = *pGrid;
            if (AiSide == Mark::CROSS)  pg->SetCross(i);
            else                        pg->SetNought(i);

            if (alg.Run(*pg, (AiSide == Mark::CROSS) ? Mark::NOUGHT : Mark::CROSS, LOSS, WIN) == DRAW) {
                std::swap(pGrid, pg);
                return;
            }
        }
    }
    void Reset()
    {
        pGrid->Clear();
        AiSide = Mark::EMPTY;
    }
};
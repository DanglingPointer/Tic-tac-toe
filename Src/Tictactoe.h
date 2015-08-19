#pragma once
#include<stdexcept>
#include<utility>
#include<set>

#ifdef _CONSOLE
#include<ostream>
#endif

#define ROW first
#define COL second

namespace ttt
{
	typedef unsigned int uint;
	typedef std::pair<uint, uint> Node;

	enum class Mark
	{
		empty,
		cross,
		nought
	};
	enum class Result
	{
		win,
		loss,
		draw
	};
	class Grid
	{
	public:
		Grid() :m_pdata(new Mark[9])
		{
			Clear();
		}
		Grid(const Grid& g) :Grid()
		{
			operator=(g);
		}
		Grid& operator = (const Grid& g)
		{
			for (uint i = 0; i < 9; ++i)
				*(m_pdata + i) = *(g.m_pdata + i);
			return *this;
		}
		~Grid()
		{
			delete[] m_pdata;
		}
		Mark at(uint row, uint col) const
		{
			return *(m_pdata + row * 3 + col);
		}
		bool SetCross(uint row, uint col)
		{
			if (at(row, col) == Mark::empty)
			{
				*(m_pdata + row * 3 + col) = Mark::cross;
				return true;
			}
			return false;
		}
		bool SetNought(uint row, uint col)
		{
			if (at(row, col) == Mark::empty)
			{
				*(m_pdata + row * 3 + col) = Mark::nought;
				return true;
			}
			return false;
		}
		Mark Won() const
		{
			bool all_cross = true, all_nought = true;
			for (uint row = 0; row < 3; ++row)
			{
				for (uint col = 0; (col < 3); ++col)
					won_at(row, col, all_cross, all_nought);
				if (all_cross)
					return Mark::cross;
				else if (all_nought)
					return Mark::nought;
			}

			all_cross = all_nought = true;
			for (uint col = 0; col < 3; ++col)
			{
				for (uint row = 0; (row < 3); ++row)
					won_at(row, col, all_cross, all_nought);
				if (all_cross)
					return Mark::cross;
				else if (all_nought)
					return Mark::nought;
			}

			all_cross = all_nought = true;
			for (uint row = 0, col = 0; row < 3; row = (++col))
				won_at(row, col, all_cross, all_nought);
			if (all_cross)
				return Mark::cross;
			else if (all_nought)
				return Mark::nought;

			all_cross = all_nought = true;
			for (uint row = 2, col = 0; col < 3; row = 2 - (++col))
				won_at(row, col, all_cross, all_nought);
			if (all_cross)
				return Mark::cross;
			else if (all_nought)
				return Mark::nought;

			return Mark::empty;
		}
		bool Filled() const
		{
			for (uint i = 0; i < 9; ++i)
				if (*(m_pdata + i) == Mark::empty)
					return false;
			return true;
		}
		void Clear()
		{
			for (uint i = 0; i < 9; ++i)
				*(m_pdata + i) = Mark::empty;
		}
	private:
		Mark *m_pdata;
		void won_at(uint row, uint col, bool& all_cross, bool& all_nought) const
		{
			Mark temp = at(row, col);
			if (temp == Mark::empty)
				all_cross = all_nought = false;
			else if (temp == Mark::cross)
				all_nought = false;
			else /*if (temp == Mark::nought) */
				all_cross = false;
		}
	};
#ifdef _CONSOLE
	std::ostream& operator << (std::ostream& out, const Grid& g)
	{
		for (uint row = 0; row < 3; ++row)
		{
			for (uint col = 0; col < 3; ++col)
				if (g.at(row, col) == Mark::empty)
					out << "   ";
				else
					g.at(row, col) == Mark::cross ? out << " X " : out << " O ";
			out << '\n';
		}
		return out;
	}
#endif
	class Gameplay
	{
	public:
		Gameplay() :pgrid(new Grid()), side(Mark::empty)
		{
			;
		}
		~Gameplay()
		{
			if (pgrid != nullptr)
				delete pgrid;
		}
		void AITurn()
		{
			for (uint row = 0; row < 3; ++row)
				for (uint col = 0; col < 3; ++col)
				{
					if (pgrid->at(row, col) != Mark::empty)
						continue;
					Grid* pg = new Grid(*pgrid);

					(side == Mark::cross) ? pg->SetCross(row, col) : pg->SetNought(row, col);

					if (Minimax(*pg, (side == Mark::cross) ? Mark::nought : Mark::cross) == Result::win)
					{
						delete pgrid;
						pgrid = pg;
						return;
					}
					delete pg;
				}
			for (uint row = 0; row < 3; ++row)
				for (uint col = 0; col < 3; ++col)
				{
					if (pgrid->at(row, col) != Mark::empty)
						continue;
					Grid* pg = new Grid(*pgrid);

					(side == Mark::cross) ? pg->SetCross(row, col) : pg->SetNought(row, col);

					if (Minimax(*pg, (side == Mark::cross) ? Mark::nought : Mark::cross) == Result::draw)
					{
						delete pgrid;
						pgrid = pg;
						return;
					}
					delete pg;
				}
		}
		void Reset()
		{
			if (pgrid == nullptr) pgrid = new Grid;
			else pgrid->Clear();
			side = Mark::empty;
		}
		Grid* pgrid;
		Mark side; // AI side
	private:
		Result Minimax(const Grid& grid, Mark turn)
		{
			if (turn == Mark::empty || side == Mark::empty)
				throw std::invalid_argument("Error: side or turn unspecified");

			Mark winner = grid.Won();
			if (grid.Filled())
			{
				if (winner == Mark::empty)
					return Result::draw;
				else if (winner != side)
					return Result::loss;
				else
					return Result::draw;
			}

			//std::set<Grid> losing_grids;
			for (uint row = 0; row < 3; ++row)
				for (uint col = 0; col < 3; ++col)
				{
					if (grid.at(row, col) != Mark::empty)
						continue;
					// Changing state
					Grid g = grid;
					(turn == Mark::cross) ? g.SetCross(row, col) : g.SetNought(row, col);
					// Intermediate node
					Result temp = Minimax(g, ((turn == Mark::cross) ? Mark::nought : Mark::cross));
					if (temp == Result::loss)
						return Result::draw;
					if (temp == Result::draw)
						continue;
					if (temp == Result::win)
						return Result::win;
				}
			return Result::draw;
		}
	};
}
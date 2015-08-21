#pragma once
#include<stdexcept>

#ifdef _CONSOLE
#include<ostream>
#endif

#define	WIN 1
#define DRAW 0
#define LOSS -1

namespace ttt
{
	typedef unsigned char uint;
	typedef char Result;
	enum class Mark
	{
		empty,
		cross,
		nought
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
		bool operator == (const Grid& rhs) const
		{
			for (uint i = 0; i < 9; ++i)
				if (*(m_pdata + i) != *(rhs.m_pdata + i))
					return false;
			return true;
		}
		bool operator !=(const Grid& rhs) const
		{
			return !(operator==(rhs));
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
			bool all_cross, all_nought;
			for (uint row = 0; row < 3; ++row)
			{
				all_cross = all_nought = true;
				for (uint col = 0; col < 3; ++col)
					won_at(row, col, all_cross, all_nought);
				if (all_cross)
					return Mark::cross;
				else if (all_nought)
					return Mark::nought;
			}

			for (uint col = 0; col < 3; ++col)
			{
				all_cross = all_nought = true;
				for (uint row = 0; row < 3; ++row)
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
	// --- Algorithms ---
#ifndef ALPHABETA
	class Minimax // Simple Minimax
	{
	public:
		explicit Minimax(Mark side) :m_side(side)
		{ }
		Result operator()(const Grid& grid, Mark turn) const
		{
			if (turn == Mark::empty || m_side == Mark::empty)
				throw std::invalid_argument("Error: side or turn unspecified");

			// Terminal node
			Mark winner = grid.Won();
			if (winner == m_side) return WIN;
			if (winner != m_side && winner != Mark::empty) return LOSS;
			if (winner == Mark::empty && grid.Filled()) return DRAW;

			// Intermediate node
			Result temp, bestval;
			if (turn == m_side)
			{
				bestval = LOSS;
				for (uint row = 0; row < 3; ++row)
					for (uint col = 0; col < 3; ++col)
					{
						if (grid.at(row, col) != Mark::empty)
							continue;
						Grid g = grid;
						(turn == Mark::cross) ? g.SetCross(row, col) : g.SetNought(row, col);

						temp = operator()(g, ((turn == Mark::cross) ? Mark::nought : Mark::cross));
						if (temp > bestval)
							bestval = temp;
					}
				return bestval;
			}
			else
			{
				bestval = WIN;
				for (uint row = 0; row < 3; ++row)
					for (uint col = 0; col < 3; ++col)
					{
						if (grid.at(row, col) != Mark::empty)
							continue;
						Grid g = grid;
						(turn == Mark::cross) ? g.SetCross(row, col) : g.SetNought(row, col);

						temp = operator()(g, ((turn == Mark::cross) ? Mark::nought : Mark::cross));
						if (temp < bestval)
							bestval = temp;
					}
				return bestval;
			}
		}
	private:
		Mark m_side;
	};
#else
	class AlphaBeta // Alpha-beta pruning
	{
	public:
		explicit AlphaBeta(Mark side) : m_side(side)
		{ }
		Result operator()(const Grid& grid, Mark turn, Result alpha = LOSS, Result beta = WIN) const
		{
			if (turn == Mark::empty || m_side == Mark::empty)
				throw std::invalid_argument("Error: side or turn unspecified");

			// Terminal node
			Mark winner = grid.Won();
			if (winner == m_side) return WIN;
			if (winner != m_side && winner != Mark::empty) return LOSS;
			if (winner == Mark::empty && grid.Filled()) return DRAW;

			// Intermediate node
			Result temp, val;
			if (turn == m_side)
			{
				val = LOSS;
				for (uint row = 0; row < 3; ++row)
					for (uint col = 0; col < 3; ++col)
					{
						if (grid.at(row, col) != Mark::empty)
							continue;
						Grid g = grid;
						(turn == Mark::cross) ? g.SetCross(row, col) : g.SetNought(row, col);

						temp = operator()(g, ((turn == Mark::cross) ? Mark::nought : Mark::cross), alpha, beta);
						if (temp > val) val = temp;
						if (val > alpha) alpha = val;
						if (beta <= alpha)
							return val;
					}
				return val;
			}
			else
			{
				val = WIN;
				for (uint row = 0; row < 3; ++row)
					for (uint col = 0; col < 3; ++col)
					{
						if (grid.at(row, col) != Mark::empty)
							continue;
						Grid g = grid;
						(turn == Mark::cross) ? g.SetCross(row, col) : g.SetNought(row, col);

						temp = operator()(g, ((turn == Mark::cross) ? Mark::nought : Mark::cross), alpha, beta);
						if (temp < val)	val = temp;
						if (val < beta) beta = val;
						if (beta <= alpha)
							return val;
					}
				return val;
			}
		}
	private:
		Mark m_side;
	}; 
#endif

	class Gameplay
	{
	public:
		Gameplay() :pgrid(new Grid()), side(Mark::empty)
		{ }
		~Gameplay()
		{
				delete pgrid;
		}

		Grid* pgrid;
		Mark side; // AI side

		template <class Alg_type> void AITurn()
		{
			Alg_type func(side);
			for (uint row = 0; row < 3; ++row)
				for (uint col = 0; col < 3; ++col)
				{
					if (pgrid->at(row, col) != Mark::empty)
						continue;
					Grid* pg = new Grid(*pgrid);

					(side == Mark::cross) ? pg->SetCross(row, col) : pg->SetNought(row, col);

					if ( func(*pg, (side == Mark::cross) ? Mark::nought : Mark::cross) == WIN )
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

					if ( func(*pg, (side == Mark::cross) ? Mark::nought : Mark::cross) == DRAW) 
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
			pgrid->Clear();
			side = Mark::empty;
		}
	}; // class Gameplay

} // namespace ttt
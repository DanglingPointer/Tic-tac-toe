#pragma once
#include<stdexcept>
#include<utility>

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
	class Ttt
	{
	public:
		Ttt() :pgrid(new Grid())
		{
			;
		}
		~Ttt()
		{
			if (pgrid != nullptr)
				delete pgrid;
		}
		void Play()
		{ }
		Grid* pgrid;
	private:
		Result Minimax(Grid*& pg, Node& node, Mark turn, Mark side)
		{
			if (turn == Mark::empty || side == Mark::empty)
				throw std::invalid_argument("Ttt::Minimax()");

			Grid* pnewg = new Grid(*pg);
			for (uint row = 0; row < 3; ++row)
				for (uint col = 0; col < 3; ++col)
				{
					if (pg->at(row, col) == Mark::empty)
					{	// Changing state
						(turn == Mark::cross) ? pnewg->SetCross(row, col) : pnewg->SetNought(row, col);
					}
					Mark winner = pnewg->Won();
					if ((winner == Mark::cross && side == Mark::cross) ||
						(winner == Mark::nought && side == Mark::nought))
					{	// Terminal node
						delete pg;
						pg = pnewg;
						return Result::win;
					}
					if ((winner == Mark::cross && side == Mark::nought) ||
						(winner == Mark::nought && side == Mark::cross))
					{	// Terminal node
						delete pnewg;
						return Result::loss;
					}
					// Intermediate node
					Result temp = Minimax(pnewg, node, ((turn == Mark::cross) ? Mark::nought : Mark::cross), side);

				}
		}
	};
}
#pragma once

#include <vector>
#include <iostream>
#include <ranges>
#include <algorithm>

#include "piece.hpp"
#include "common.hpp"

namespace game
{

class GameState
{
public:
	GameState()
	{
		board = std::vector<std::vector<Piece>>(8,
			std::vector<Piece>(8, Piece(Type::NONE, Colour::NONE)));

		// Setup pawns
		board[1] = std::vector<Piece>(8, Piece(Type::PAWN, Colour::BLACK));
		board[6] = std::vector<Piece>(8, Piece(Type::PAWN, Colour::WHITE));

		// Setup rooks
		board[0][0] = Piece(Type::ROOK, Colour::BLACK);
		board[0][7] = Piece(Type::ROOK, Colour::BLACK);
		board[7][0] = Piece(Type::ROOK, Colour::WHITE);
		board[7][7] = Piece(Type::ROOK, Colour::WHITE);

		// Setup knights
		board[0][1] = Piece(Type::KNIGHT, Colour::BLACK);
		board[0][6] = Piece(Type::KNIGHT, Colour::BLACK);
		board[7][1] = Piece(Type::KNIGHT, Colour::WHITE);
		board[7][6] = Piece(Type::KNIGHT, Colour::WHITE);

		// Setup bishops
		board[0][2] = Piece(Type::BISHOP, Colour::BLACK);
		board[0][5] = Piece(Type::BISHOP, Colour::BLACK);
		board[7][2] = Piece(Type::BISHOP, Colour::WHITE);
		board[7][5] = Piece(Type::BISHOP, Colour::WHITE);

		// Setup kings
		board[0][4] = Piece(Type::KING, Colour::BLACK);
		board[7][4] = Piece(Type::KING, Colour::WHITE);

		// Setup queens
		board[0][3] = Piece(Type::QUEEN, Colour::BLACK);
		board[7][3] = Piece(Type::QUEEN, Colour::WHITE);

		update_all();
	}	

	auto get_turn() const -> Colour
	{
		return turn;
	}

	auto get_board() const -> std::vector<std::vector<Piece>>
	{
		return board;
	}

	auto clear_board() -> void
	{
		for (auto& v : board)
			for (auto& p : v)
				p = Piece(Type::NONE, Colour::NONE);
	}

	auto place_at(BoardIndex row, BoardIndex col, Piece piece) -> void
	{
		board[row][col] = piece;
	}

	auto set_turn(Colour new_turn) -> void
	{
		turn = new_turn;
	}

	auto at(Position pos) -> Piece&
	{
		return board[pos.first][pos.second];
	}

	auto move(Position o,
			  Position d,
			  bool validate) -> bool
	{
		auto moved_piece = at(o);

		// TODO: Should be some piece-generic validation here
		// around capturing own pieces/moving off the board.

		if (validate &&
			moved_piece.moves_to.find(d)
			== moved_piece.moves_to.end())
			return false;

		at(d) = moved_piece;
		at(o) = Piece();

		update_all();

		return true;
	}

	auto get_stalemate() const -> bool
	{
		return false;
	}

	auto get_black_wins() const -> bool
	{
		return false;
	}

	auto get_white_wins() const -> bool
	{
		return false;
	}

private:
	Colour turn = Colour::WHITE;
	std::vector<std::vector<Piece>> board;
	std::set<Position> attacked_by_black, attacked_by_white;

	auto update_all() -> void
	{
		attacked_by_black.clear();
		attacked_by_white.clear();

		Position black_king, white_king;

		for (auto i = 0; i < 8; i++)
			for (auto j = 0; j < 8; j++)
				if (at({ i, j })._type != Type::KING)
					update({ i, j });
				else if (at({ i, j })._colour == Colour::BLACK)
					black_king = { i, j };
				else
					white_king = { i, j };

		update(black_king);
		update(white_king);
	}

	auto update(const Position& pos) -> void
	{
		auto& piece = at(pos);

		if (piece._type == Type::NONE)
			return;

		// Pawn promotion... something of a special case.
		if (piece._type == Type::PAWN &&
		   ((piece._colour == Colour::BLACK && pos.first == 7) ||
			(piece._colour == Colour::WHITE && pos.first == 0)))
				piece._type = Type::QUEEN;

		piece.attacks.clear();
		piece.moves_to.clear();

		update_attacks(pos);

		if (piece._colour == Colour::BLACK)
			attacked_by_black.insert(piece.attacks.begin(), piece.attacks.end());
		else
			attacked_by_white.insert(piece.attacks.begin(), piece.attacks.end());
	}

	auto update_attacks(const Position& pos) -> void
	{
		switch (at(pos)._type)
		{
		case Type::PAWN:
			pawn_update_attacks(pos);
			break;
		case Type::KING:
			king_update_attacks(pos);
			break;
		case Type::QUEEN:
			queen_update_attacks(pos);
			break;
		case Type::ROOK:
			rook_update_attacks(pos);
			break;
		case Type::KNIGHT:
			knight_update_attacks(pos);
			break;
		case Type::BISHOP:
			bishop_update_attacks(pos);
			break;
		default:
			std::cout << "ERROR: Unexpected piece.\n";
			break;
		}
	}

	auto pawn_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		auto forward = 0;
		auto home_row = 0;

		if (piece._colour == Colour::WHITE)
		{
			home_row = 6;
			forward = -1;
		}
		else
		{
			home_row = 1;
			forward = 1;
		}

		// Cells pawn may attack

		std::vector<Position> cells;

		if (pos.second != 0)
			cells.push_back({ pos.first + forward, pos.second - 1 });
		if (pos.second != 7)
			cells.push_back({ pos.first + forward, pos.second + 1 });

		for (const auto& c: cells)
		{
			piece.attacks.insert(c);

			if (at(c)._type != Type::NONE &&
				at(c)._colour != piece._colour)
				piece.moves_to.insert(c);
		}

		// Cells pawn may advance into

		Position advance = { pos.first + forward, pos.second };

		if (at(advance)._type == Type::NONE)
			piece.moves_to.insert(advance);

		Position advance_two = { pos.first + ( 2 * forward), pos.second };

		if (pos.first == home_row &&
			at(advance)._type == Type::NONE &&
			at(advance_two)._type == Type::NONE)
			piece.moves_to.insert(advance_two);
	}

	auto attacked_by_oponent(const Position& pos, Colour c) const -> bool
	{
		return (c == Colour::BLACK &&
			attacked_by_white.find(pos) != attacked_by_white.end()) ||
			(c == Colour::WHITE &&
				attacked_by_black.find(pos) != attacked_by_black.end());
	}

	auto king_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		std::vector<int> is = { -1, 0, 1 };

		for (auto i : is)
			for (auto j : is)
			{
				Position p = { pos.first + i, pos.second + j };

				if (i == 0 && j == 0)
					continue;

				if (!in_bounds(p))
					continue;

				piece.attacks.insert(p);

				if (at(p)._colour != piece._colour &&
					!attacked_by_oponent(p, piece._colour))
					piece.moves_to.insert(p);
			}
	}

	auto queen_update_attacks(const Position& pos) -> void
	{
		bishop_update_attacks(pos);
		rook_update_attacks(pos);
	}

	auto check_cell(Piece& piece, Position p) -> bool
	{
		piece.attacks.insert(p);

		if (at(p)._colour != piece._colour)
			piece.moves_to.insert(p);

		if (at(p)._type != Type::NONE)
			return false;

		return true;
	}

	auto rook_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		for (Position p = { pos.first + 1, pos.second };
			p.first <= 7;
			p.first++)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first - 1, pos.second };
			p.first >= 0;
			p.first--)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first, pos.second + 1 };
			p.second <= 7;
			p.second++)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first, pos.second - 1 };
			p.second >= 0;
			p.second--)
		{
			if (!check_cell(piece, p))
				break;
		}
	}

	auto bishop_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		for (Position p = { pos.first + 1, pos.second + 1 };
			p.first <= 7 && p.second <= 7;
			p.first++, p.second++)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first - 1, pos.second - 1 };
			p.first >= 0 && p.second >= 0;
			p.first--, p.second--)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first - 1, pos.second + 1 };
			p.first >= 0 && p.second <= 7;
			p.first--, p.second++)
		{
			if (!check_cell(piece, p))
				break;
		}

		for (Position p = { pos.first + 1, pos.second - 1 };
			p.first <= 7 && p.second >= 0;
			p.first++, p.second--)
		{
			if (!check_cell(piece, p))
				break;
		}
	}

	auto knight_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		std::vector<Position> cards = { {2, 1}, {1, 2} };
		std::vector<int> signs = { {+1, -1} };

		for (auto card :cards)
			for (auto s_first: signs)
				for (auto s_second : signs)
				{
					Position p = { pos.first + card.first * s_first,
									pos.second + card.second * s_second };

					if (in_bounds(p))
					{
						piece.attacks.insert(p);

						if (at(p)._colour != piece._colour)
							piece.moves_to.insert(p);
					}
				}
	}

	static auto get_range(int init, int dest) -> std::vector<int>
	{
		std::vector<int> r(std::abs(init - dest) + 1, 0);
		int direction = init - dest > 0 ? -1 : 1;
		std::ranges::generate(r, [&init, direction]() { init += direction;
		return init - direction;  });
		return r;
	}

	auto in_bounds(Position pos) const -> bool
	{
		return pos.first >= 0 && pos.first <= 7 &&
				pos.second >= 0 && pos.second <= 7;
	}

};

}

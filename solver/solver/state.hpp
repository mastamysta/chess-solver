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

		for (auto i = 0; i < 8; i++)
			for (auto j = 0; j < 8; j++)
				update({i, j});

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

	auto update(const Position& pos) -> void
	{
		auto& piece = at(pos);

		// Pawn promotion... something of a special case.
		if (piece._type == Type::PAWN &&
		   ((piece._colour == Colour::BLACK && pos.first == 7) ||
			(piece._colour == Colour::WHITE && pos.first == 0)))
				piece._type = Type::QUEEN;

		piece.attacks.clear();
		piece.moves_to.clear();

		update_attacks(pos);
		update_moves_to(pos);
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
		case Type::ROOK:
		case Type::KNIGHT:
		case Type::BISHOP:
		default:
			std::cout << "ERROR: Unexpected piece.\n";
			break;
		}
	}

	auto update_moves_to(const Position& pos) -> void
	{
		switch (at(pos)._type)
		{
		case Type::PAWN:
			pawn_update_moves_to(pos);
			break;
		case Type::KING:
			king_update_moves_to(pos);
			break;
		case Type::QUEEN:
		case Type::ROOK:
		case Type::KNIGHT:
		case Type::BISHOP:
		default:
			std::cout << "ERROR: Unexpected piece.\n";
			break;
		}
	}

	auto pawn_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

		auto forward = 0;

		if (piece._colour == Colour::WHITE)
			forward = -1;
		else
			forward = 1;

		if (pos.second != 0)
			piece.attacks.insert({ pos.first + forward, pos.second - 1 });

		if (pos.second != 7)
			piece.attacks.insert({ pos.first + forward, pos.second + 1 });
	}

	auto pawn_update_moves_to(const Position& pos) -> void
	{
		auto& piece = at(pos);

		auto forward = 0;

		if (piece._colour == Colour::WHITE)
			forward = -1;
		else
			forward = 1;

		Position advance = { pos.first + forward, pos.second };

		if (at(advance)._type == Type::NONE)
			piece.moves_to.insert(advance);

		// TODO: Need to add two square move for first move with pawn.

		for (const auto p : piece.attacks)
			if (at(p)._type != Type::NONE)
				piece.moves_to.insert(p);
	}


	auto king_update_attacks(const Position& pos) -> void
	{
		auto& piece = at(pos);

	}

	auto king_update_moves_to(const Position& pos) -> void
	{
	}

	static auto get_range(int init, int dest) -> std::vector<int>
	{
		std::vector<int> r(std::abs(init - dest) + 1, 0);
		int direction = init - dest > 0 ? -1 : 1;
		std::ranges::generate(r, [&init, direction]() { init += direction;
		return init - direction;  });
		return r;
	}

};

}

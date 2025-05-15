#pragma once

#include <vector>
#include <iostream>

namespace game
{

class GameState
{
public:
	using BoardIndex = uint8_t;

	GameState()
	{
		board = std::vector<std::vector<Piece>>(8,
			std::vector<Piece>(8, Piece::NONE));

		// Setup pawns
		board[1] = std::vector<Piece>(8, Piece::BPAWN);
		board[6] = std::vector<Piece>(8, Piece::WPAWN);

		// Setup rooks
		board[0][0] = Piece::BROOK;
		board[0][7] = Piece::BROOK;
		board[7][0] = Piece::WROOK;
		board[7][7] = Piece::WROOK;

		// Setup knights
		board[0][1] = Piece::BKNIGHT;
		board[0][6] = Piece::BKNIGHT;
		board[7][1] = Piece::WKNIGHT;
		board[7][6] = Piece::WKNIGHT;

		// Setup bishops
		board[0][2] = Piece::BBISHOP;
		board[0][5] = Piece::BBISHOP;
		board[7][2] = Piece::WBISHOP;
		board[7][5] = Piece::WBISHOP;

		// Setup kings
		board[0][4] = Piece::BKING;
		board[7][4] = Piece::WKING;

		// Setup queens
		board[0][3] = Piece::BQUEEN;
		board[7][3] = Piece::WQUEEN;
	}	
	
	enum class Turn
	{
		BLACK,
		WHITE
	};

	enum class Piece
	{
		NONE,
		WPAWN,
		WROOK,
		WKNIGHT,
		WBISHOP,
		WKING,
		WQUEEN,
		BPAWN,
		BROOK,
		BKNIGHT,
		BBISHOP,
		BKING,
		BQUEEN
	};

	auto get_turn() const -> Turn
	{
		return turn;
	}

	auto get_board() const -> std::vector<std::vector<Piece>>
	{
		return board;
	}

	auto get_at(BoardIndex row, BoardIndex col) const -> Piece
	{
		return board[row][col];
	}

	auto move(BoardIndex row_o,
			  BoardIndex col_o,
			  BoardIndex row_d,
			  BoardIndex col_d,
			  bool validate) -> bool
	{
		auto moved_piece = board[row_o][col_o];
		bool valid = false;

		switch (moved_piece)
		{
		case Piece::BPAWN:
		case Piece::WPAWN:
			valid = validate_pawn_move(row_o, col_o, row_d, col_d);
			break;
		case Piece::BKING:
		case Piece::WKING:
			valid = true;
			break;
		default:
			std::cout << "ERROR: Unexpected piece.\n";
			break;
		}

		if (!valid)
			return false;

		board[row_d][col_d] = moved_piece;
		board[row_o][col_o] = Piece::NONE;

		return true;
	}

	auto castle(BoardIndex row_o,
		BoardIndex col_o,
		bool validate) -> bool
	{
		return true;
	}

private:
	Turn turn = Turn::WHITE;
	std::vector<std::vector<Piece>> board;

	auto is_black(Piece piece) const -> bool
	{
		switch (piece)
		{
		case Piece::BPAWN:
		case Piece::BROOK:
		case Piece::BKNIGHT:
		case Piece::BBISHOP:
		case Piece::BQUEEN:
		case Piece::BKING:
			return true;
		default:
			break;
		}

		return false;
	}
	
	auto validate_pawn_move(BoardIndex row_o,
							BoardIndex col_o,
							BoardIndex row_d,
							BoardIndex col_d) const -> bool
	{
		auto piece = board[row_o][col_o];
		auto dest_piece = board[row_d][col_d];

		// Rules to avoid moving non-forward.
		if (piece == Piece::BPAWN && row_d <= row_o)
			return false;
		if (piece == Piece::WPAWN && row_d >= row_o)
			return false;

		// Early rules to allow going one diagonally to capture.
		if (std::abs(row_o - row_d) == 1 && std::abs(col_o - col_d) == 1 &&
			dest_piece != Piece::NONE &&
			is_black(piece) != is_black(dest_piece))
			return true;

		// Rule to avoid moving sideways.
		if (col_o != col_d)
			return false;

		auto dist = std::abs(row_o - row_d);

		// Rules to avoid moving two spaces except on first turn.
		if (dist > 2)
			return false;
		if (dist == 2 && piece == Piece::WPAWN && row_o != 6)
			return false;
		if (dist == 2 && piece == Piece::BPAWN && row_o != 1)
			return false;

		// Rules to avoid jumping over pieces.
		if (dist == 2 && piece == Piece::BPAWN && board[row_o + 1][col_o] != Piece::NONE)
			return false;
		if (dist == 2 && piece == Piece::WPAWN && board[row_o - 1][col_o] != Piece::NONE)
			return false;

		// Rules to avoid capturing directly forward.
		if (board[row_d][col_d] != Piece::NONE)
			return false;

		return true;
	}

};

}

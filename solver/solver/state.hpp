#pragma once

#include <vector>
#include <iostream>
#include <ranges>
#include <algorithm>

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

		if (validate && !validate_move(row_o, col_o, row_d, col_d))
			return false;

		board[row_d][col_d] = moved_piece;
		board[row_o][col_o] = Piece::NONE;

		// Pawn promotion... will refactor this to be open-closed later.
		if (moved_piece == Piece::WPAWN && row_d == 0)
			board[row_d][col_d] = Piece::WQUEEN;
		if (moved_piece == Piece::BPAWN && row_d == 7)
			board[row_d][col_d] = Piece::BQUEEN;

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
		if (piece == Piece::NONE) // Yuck exceptions, but lets make debugging easy.
			throw std::invalid_argument("Attempting to check colour on NONE.\n");

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

	auto same_colour(Piece a, Piece b) const -> bool
	{
		return is_black(a) == is_black(b);
	}

	auto validate_move(BoardIndex row_o,
					   BoardIndex col_o,
					   BoardIndex row_d,
					   BoardIndex col_d) const -> bool
	{
		auto moved_piece = board[row_o][col_o];
		auto dest_piece = board[row_d][col_d];

		// Not moving a piece is NEVER allowed.
		if (row_o == row_d && col_o == col_d)
			return false;

		// Capturing own piece is never allowed.
		if (dest_piece != Piece::NONE && same_colour(moved_piece, dest_piece))
			return false;

		switch (moved_piece)
		{
		case Piece::BPAWN:
		case Piece::WPAWN:
			return validate_pawn_move(row_o, col_o, row_d, col_d);
		case Piece::BKING:
		case Piece::WKING:
			return validate_king_move(row_o, col_o, row_d, col_d);
		case Piece::BROOK:
		case Piece::WROOK:
			return validate_rook_move(row_o, col_o, row_d, col_d);
		case Piece::BBISHOP:
		case Piece::WBISHOP:
			return validate_bishop_move(row_o, col_o, row_d, col_d);
		case Piece::BKNIGHT:
		case Piece::WKNIGHT:
			return validate_knight_move(row_o, col_o, row_d, col_d);
		case Piece::BQUEEN:
		case Piece::WQUEEN:
			return validate_queen_move(row_o, col_o, row_d, col_d);
		default:
			std::cout << "ERROR: Unexpected piece.\n";
			break;
		}

		return false;
	}

	auto get_range(int init, int dest) const -> std::vector<int>
	{
		std::vector<int> r(std::abs(init - dest) + 1, 0);
		int direction = init - dest > 0 ? -1 : 1;
		std::ranges::generate(r, [&init, direction]() { init += direction;
		return init - direction;  });
		return r;
	}
	
	auto validate_queen_move(BoardIndex row_o,
							BoardIndex col_o,
							BoardIndex row_d,
							BoardIndex col_d) const -> bool
	{
		return validate_bishop_move(row_o, col_o, row_d, col_d) ||
					validate_rook_move(row_o, col_o, row_d, col_d);
	}
	
	auto validate_knight_move(BoardIndex row_o,
							BoardIndex col_o,
							BoardIndex row_d,
							BoardIndex col_d) const -> bool
	{
		auto vertical = std::abs(row_o - row_d);
		auto horizontal = std::abs(col_o - col_d);

		if (std::min(vertical, horizontal) != 1 ||
			std::max(vertical, horizontal) != 2)
			return false;

		return true;
	}
	
	auto validate_rook_move(BoardIndex row_o,
							BoardIndex col_o,
							BoardIndex row_d,
							BoardIndex col_d) const -> bool
	{
		bool vertical = std::abs(row_o - row_d);
		bool horizontal = std::abs(col_o - col_d);

		// May only move horizontally or vertically.
		if (vertical && horizontal)
			return false;

		auto orig = 0, dest = 0;

		if (vertical)
		{
			orig = row_o;
			dest = row_d;
		}
		else
		{
			orig = col_o;
			dest = col_d;
		}

		auto r = get_range(orig, dest);

		for (auto i : r)
		{
			if (i == dest || i == orig)
				continue;

			if ((vertical && board[i][col_o] != Piece::NONE) ||
				(horizontal && board[row_o][i] != Piece::NONE))
				return false;
		}

		return true;
	}

	auto validate_bishop_move(BoardIndex row_o,
								BoardIndex col_o,
								BoardIndex row_d,
								BoardIndex col_d) const -> bool
	{
		if (std::abs(row_o - row_d) != std::abs(col_o - col_d))
			return false;

		auto dist = std::abs(row_o - row_d);
		auto cols = get_range(col_o, col_d);
		auto rows = get_range(row_o, row_d);

		for (auto i = 0; i < dist; i++)
		{
			int col = cols[i], row = rows[i];

			if (row == row_d || row == row_o)
				continue;

			if (board[row][col] != Piece::NONE)
				return false;
		}

		return true;
	}

	auto validate_king_move(BoardIndex row_o,
							BoardIndex col_o,
							BoardIndex row_d,
							BoardIndex col_d) const -> bool
	{
		auto piece = board[row_o][col_o];
		auto dest_piece = board[row_d][col_d];

		if (std::abs(row_o - row_d) > 1 || std::abs(col_o - col_d) > 1)
			return false;

		return true;
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
			dest_piece != Piece::NONE)
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

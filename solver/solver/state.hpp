#include <vector>

namespace game
{

class GameState
{
public:
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

private:
	Turn turn = Turn::WHITE;
	std::vector<std::vector<Piece>> board;
	
};

}

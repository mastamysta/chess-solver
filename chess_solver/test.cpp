#include "pch.h"

#include "state.hpp"

static constexpr int BLACK_HOME_ROW = 0;
static constexpr int WHITE_HOME_ROW = 7;

// King positions
static constexpr int BLACK_KING_STARTING_ROW = BLACK_HOME_ROW;
static constexpr int KING_STARTING_COL = 4;
static constexpr int WHITE_KING_STARTING_ROW = WHITE_HOME_ROW;

// Pawn positions
static constexpr int BLACK_PAWN_ROW = 1;
static constexpr int WHITE_PAWN_ROW = 6;

// Rook positions
static constexpr int LEFT_ROOK_COL = 0;
static constexpr int RIGHT_ROOK_COL = 7;

// Bishop positions
static constexpr int LEFT_BISHOP_COL = 2;
static constexpr int RIGHT_BISHOP_COL = 5;

// Knight positions
static constexpr int LEFT_KNIGHT_COL = 1;
static constexpr int RIGHT_KNIGHT_COL = 6;

// Queen positions
static constexpr int QUEEN_STARTING_COL = 3;
TEST(GameStateTests, InitialTurnWhite) 
{
  auto game = game::GameState();
  auto colour = game.get_turn();
  ASSERT_EQ(colour, game::GameState::Turn::WHITE);
}

TEST(GameStateTests, GetBoard)
{
	auto game = game::GameState();
	auto board = game.get_board();
}

TEST(GameStateTests, BoardSize)
{
	auto game = game::GameState();
	auto board = game.get_board();
	auto piece = board[7][7];
}

TEST(GameStateTests, InitialPawnRows)
{
	auto game = game::GameState();

	for (int i = 0; i < 8; i++)
	{
		ASSERT_EQ(game.get_board()[1][i], game::GameState::Piece::BPAWN);
		ASSERT_EQ(game.get_board()[6][i], game::GameState::Piece::WPAWN);
	}
}

TEST(GameStateTests, InitialRooks)
{
	auto game = game::GameState();

	ASSERT_EQ(game.get_board()[0][0], game::GameState::Piece::BROOK);
	ASSERT_EQ(game.get_board()[0][7], game::GameState::Piece::BROOK);
	ASSERT_EQ(game.get_board()[7][7], game::GameState::Piece::WROOK);
	ASSERT_EQ(game.get_board()[7][0], game::GameState::Piece::WROOK);
}

TEST(GameStateTests, InitialKnights)
{
	auto game = game::GameState();

	ASSERT_EQ(game.get_board()[0][1], game::GameState::Piece::BKNIGHT);
	ASSERT_EQ(game.get_board()[0][6], game::GameState::Piece::BKNIGHT);
	ASSERT_EQ(game.get_board()[7][1], game::GameState::Piece::WKNIGHT);
	ASSERT_EQ(game.get_board()[7][6], game::GameState::Piece::WKNIGHT);
}

TEST(GameStateTests, InitialBishops)
{
	auto game = game::GameState();

	ASSERT_EQ(game.get_board()[0][2], game::GameState::Piece::BBISHOP);
	ASSERT_EQ(game.get_board()[0][5], game::GameState::Piece::BBISHOP);
	ASSERT_EQ(game.get_board()[7][2], game::GameState::Piece::WBISHOP);
	ASSERT_EQ(game.get_board()[7][5], game::GameState::Piece::WBISHOP);
}

TEST(GameStateTests, InitialKings)
{
	auto game = game::GameState();

	// 0th row is for black king
	ASSERT_EQ(game.get_board()[BLACK_KING_STARTING_ROW][KING_STARTING_COL],
										game::GameState::Piece::BKING);
	ASSERT_EQ(game.get_board()[7][4], game::GameState::Piece::WKING);
}

TEST(GameStateTests, InitialQueens)
{
	auto game = game::GameState();

	// 0th row is for black king
	ASSERT_EQ(game.get_board()[0][3], game::GameState::Piece::BQUEEN);
	ASSERT_EQ(game.get_board()[7][3], game::GameState::Piece::WQUEEN);
}

TEST(GameStateTests, GetAt)
{
	auto game = game::GameState();
	ASSERT_EQ(game.get_at(0, 3), game::GameState::Piece::BQUEEN);
	ASSERT_EQ(game.get_at(7, 4), game::GameState::Piece::WKING);
}


TEST(GameStateTests, CanMovePawnOne)
{
	auto game = game::GameState();

	uint8_t row_o = 1, col_o = 4, row_d = row_o+1, col_d = col_o;

	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	ASSERT_EQ(game.get_at(row_o, col_o), game::GameState::Piece::NONE);
	ASSERT_EQ(game.get_at(row_d, col_d), game::GameState::Piece::BPAWN);
}
TEST(GameStateTests, CanMovePawnTwoOnFirst)
{
	auto game = game::GameState();

	uint8_t row_o = 1, col_o = 4, row_d = row_o+2, col_d = col_o;

	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	ASSERT_EQ(game.get_at(row_o, col_o), game::GameState::Piece::NONE);
	ASSERT_EQ(game.get_at(row_d, col_d), game::GameState::Piece::BPAWN);
}

TEST(GameStateTests, CantMovePawnDiag)
{
	auto game = game::GameState();

	uint8_t row_o = 1, col_o = 4, row_d = row_o+2, col_d = col_o+1;

	ASSERT_FALSE(game.move(row_o, col_o, row_d, col_d, true));
	ASSERT_EQ(game.get_at(row_o, col_o), game::GameState::Piece::BPAWN);
	ASSERT_EQ(game.get_at(row_d, col_d), game::GameState::Piece::NONE);
}

TEST(GameStateTests, CantMoveBlackPawnBack)
{
	auto game = game::GameState();

	// First move the black pawn forward.
	uint8_t row_o = 1, col_o = 4, row_d = row_o+2, col_d = col_o;
	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	// Now try and move it backwards.
	ASSERT_FALSE(game.move(row_d, col_d, row_d-1, col_d, true));
}

TEST(GameStateTests, CantMoveWhitePawnBack)
{
	auto game = game::GameState();

	// First move the white pawn forward.
	uint8_t row_o = 6, col_o = 4, row_d = row_o-2, col_d = col_o;
	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	// Now try and move it backwards.
	ASSERT_FALSE(game.move(row_d, col_d, row_d + 1, col_d, true));
}

TEST(GameStateTests, CantMoveWhitePawnTwoOtherwise)
{
	auto game = game::GameState();

	// First move the white pawn forward.
	uint8_t row_o = 6, col_o = 4, row_d = row_o-1, col_d = col_o;
	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	// Now try and move it forward two.
	ASSERT_FALSE(game.move(row_d, col_d, row_d-2, col_d, true));
}

TEST(GameStateTests, CantMoveBlackPawnTwoOtherwise)
{
	auto game = game::GameState();

	// First move the black pawn forward.
	uint8_t row_o = 1, col_o = 4, row_d = row_o + 1, col_d = col_o;
	ASSERT_TRUE(game.move(row_o, col_o, row_d, col_d, true));
	// Now try and move it forward two.
	ASSERT_FALSE(game.move(row_d, col_d, row_d + 2, col_d, true));
}

TEST(GameStateTests, PawnCantMoveZero)
{
	auto game = game::GameState();

	// First move the black pawn forward.
	uint8_t row_o = 1, col_o = 4, row_d = row_o, col_d = col_o;
	ASSERT_FALSE(game.move(row_o, col_o, row_d, col_d, true));
}

TEST(GameStateTests, BlackPawnCantJump)
{
	auto game = game::GameState();

	// First move the black king in front of a pawn.
	ASSERT_TRUE(game.move(BLACK_KING_STARTING_ROW, 
						  KING_STARTING_COL, 
						  BLACK_KING_STARTING_ROW+2,
						  KING_STARTING_COL,
						  false));
	// Now try to skip pawn over king.
	ASSERT_FALSE(game.move(BLACK_KING_STARTING_ROW+1,
						   KING_STARTING_COL, 
						   BLACK_KING_STARTING_ROW+3, 
						   KING_STARTING_COL,
						   true));
}

TEST(GameStateTests, WhitePawnCantJump)
{
	auto game = game::GameState();

	// First move the white king in front of a pawn.
	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							WHITE_KING_STARTING_ROW - 2,
							KING_STARTING_COL,
							false));
	// Now try to skip pawn over king.
	ASSERT_FALSE(game.move(WHITE_KING_STARTING_ROW - 1,
							KING_STARTING_COL,
							WHITE_KING_STARTING_ROW - 3,
							KING_STARTING_COL,
							true));
}

TEST(GameStateTests, PawnCantCaptureOneForward)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW+1;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
						  KING_STARTING_COL,
						  new_king_row,
						  KING_STARTING_COL,
						  false));
	ASSERT_FALSE(game.move(1,
						   KING_STARTING_COL,
						   new_king_row,
						   KING_STARTING_COL,
						   true));
}

TEST(GameStateTests, PawnCantCaptureTwoForward)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW+2;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_FALSE(game.move(BLACK_PAWN_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							true));
}

TEST(GameStateTests, BlackPawnCanCaptureOneDiagonally)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW+1;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_TRUE(game.move(BLACK_PAWN_ROW,
							KING_STARTING_COL-1,
							new_king_row,
							KING_STARTING_COL,
							true));
}

TEST(GameStateTests, WhitePawnCanCaptureOneDiagonally)
{
	auto game = game::GameState();
	auto new_king_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(BLACK_KING_STARTING_ROW,
				KING_STARTING_COL,
				new_king_row,
				KING_STARTING_COL,
				false));
	ASSERT_TRUE(game.move(WHITE_PAWN_ROW,
				KING_STARTING_COL - 1,
				new_king_row,
				KING_STARTING_COL,
				true));
}

TEST(GameStateTests, PawnCantCaptureOwnPiece)
{
	auto game = game::GameState();
	auto new_pawn_col = 4;
	auto new_pawn_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_PAWN_ROW,
							0,
							new_pawn_row,
							new_pawn_col,
							false));
	ASSERT_FALSE(game.move(WHITE_PAWN_ROW,
							new_pawn_col-1,
							new_pawn_row,
							new_pawn_col,
							true));
}


TEST(GameStateTests, PawnPromotes)
{
	auto game = game::GameState();

	ASSERT_TRUE(game.move(WHITE_PAWN_ROW,
							0,
							BLACK_PAWN_ROW,
							0,
							false));
	ASSERT_TRUE(game.move(BLACK_PAWN_ROW,
							0,
							BLACK_HOME_ROW,
							1,
							true));
	ASSERT_EQ(game.get_at(BLACK_HOME_ROW, 1), game::GameState::Piece::WQUEEN);
}

TEST(GameStateTests, KingCantCaptureOwnPiece)
{
	auto game = game::GameState();

	ASSERT_FALSE(game.move(BLACK_KING_STARTING_ROW,
							KING_STARTING_COL,
							BLACK_KING_STARTING_ROW+1,
							KING_STARTING_COL,
							true));

}


TEST(GameStateTests, KingCantMoveNowhere)
{
	auto game = game::GameState();

	ASSERT_FALSE(game.move(BLACK_KING_STARTING_ROW,
							KING_STARTING_COL,
							BLACK_KING_STARTING_ROW,
							KING_STARTING_COL,
							true));
}


TEST(GameStateTests, KingCantMoveMoreThanOneForward)
{
	auto game = game::GameState();

	ASSERT_FALSE(game.move(BLACK_KING_STARTING_ROW,
							KING_STARTING_COL,
							BLACK_KING_STARTING_ROW+2,
							KING_STARTING_COL,
							true));
}


TEST(GameStateTests, KingCantMoveMoreThanOneSideways)
{
	auto game = game::GameState();
	auto new_king_row = 4;
	
	ASSERT_TRUE(game.move(BLACK_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_FALSE(game.move(new_king_row,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL+2,
							true));
}


TEST(GameStateTests, KingCanCapture)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW + 1;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_TRUE(game.move(new_king_row,
							KING_STARTING_COL,
							BLACK_PAWN_ROW,
							KING_STARTING_COL,
							true));
}


TEST(GameStateTests, KingCanMove)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW + 1;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_TRUE(game.move(new_king_row,
							KING_STARTING_COL,
							new_king_row+1,
							KING_STARTING_COL,
							true));
}

TEST(GameStateTests, KingCantMoveIntoCheck)
{
	auto game = game::GameState();
	auto new_king_row = BLACK_PAWN_ROW + 2;

	ASSERT_TRUE(game.move(WHITE_KING_STARTING_ROW,
							KING_STARTING_COL,
							new_king_row,
							KING_STARTING_COL,
							false));
	ASSERT_FALSE(game.move(new_king_row,
							KING_STARTING_COL,
							new_king_row - 1,
							KING_STARTING_COL,
							true));
}

TEST(GameStateTests, RookCanMove)
{
	auto game = game::GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							LEFT_ROOK_COL,
							new_rook_row,
							LEFT_ROOK_COL,
							false));
	ASSERT_TRUE(game.move(new_rook_row,
							LEFT_ROOK_COL,
							new_rook_row - 1,
							LEFT_ROOK_COL,
							true));
}


TEST(GameStateTests, RookCantMoveDiagonally)
{
	auto game = game::GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							LEFT_ROOK_COL,
							new_rook_row,
							LEFT_ROOK_COL,
							false));
	ASSERT_FALSE(game.move(new_rook_row,
							LEFT_ROOK_COL,
							new_rook_row-1,
							LEFT_ROOK_COL+1,
							true));
}


TEST(GameStateTests, RookCantJump)
{
	auto game = game::GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_FALSE(game.move(WHITE_HOME_ROW,
							LEFT_ROOK_COL,
							new_rook_row,
							LEFT_ROOK_COL,
							true));
}


TEST(GameStateTests, RookCanCapture)
{
	auto game = game::GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							LEFT_ROOK_COL,
							new_rook_row,
							LEFT_ROOK_COL,
							false));
	ASSERT_TRUE(game.move(new_rook_row,
							LEFT_ROOK_COL,
							BLACK_PAWN_ROW,
							LEFT_ROOK_COL,
							true));
}

TEST(GameStateTests, BishopCanMove)
{
	auto game = game::GameState();
	auto new_bishop_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							LEFT_BISHOP_COL,
							new_bishop_row,
							LEFT_BISHOP_COL,
							false));
	ASSERT_TRUE(game.move(new_bishop_row,
							LEFT_BISHOP_COL,
							new_bishop_row-1,
							LEFT_BISHOP_COL+1,
							true));
}


TEST(GameStateTests, BishopCantMoveNonDiagonal)
{
	auto game = game::GameState();
	auto new_bishop_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							LEFT_BISHOP_COL,
							new_bishop_row,
							LEFT_BISHOP_COL,
							false));
	ASSERT_FALSE(game.move(new_bishop_row,
							LEFT_BISHOP_COL,
							new_bishop_row,
							LEFT_BISHOP_COL + 1,
							true));
}


TEST(GameStateTests, BishopCantJump)
{
	auto game = game::GameState();

	ASSERT_FALSE(game.move(WHITE_HOME_ROW,
							LEFT_BISHOP_COL,
							WHITE_HOME_ROW-2,
							LEFT_BISHOP_COL+2,
							true));
}

TEST(GameStateTests, KnightCanMove)
{
	auto game = game::GameState();

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							RIGHT_KNIGHT_COL,
							WHITE_HOME_ROW - 2,
							RIGHT_KNIGHT_COL + 1,
							true));
}

TEST(GameStateTests, KnightCantMoveFar)
{
	auto game = game::GameState();

	ASSERT_FALSE(game.move(WHITE_HOME_ROW,
							RIGHT_KNIGHT_COL,
							WHITE_HOME_ROW - 2,
							RIGHT_KNIGHT_COL - 3,
							true));
}


TEST(GameStateTests, QueenCanMoveStraight)
{
	auto game = game::GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							QUEEN_STARTING_COL,
							new_queen_row,
							QUEEN_STARTING_COL,
							false));
	ASSERT_TRUE(game.move(new_queen_row,
							QUEEN_STARTING_COL,
							new_queen_row-1,
							QUEEN_STARTING_COL,
							true));
}

TEST(GameStateTests, QueenCanMoveDiagonally)
{
	auto game = game::GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							QUEEN_STARTING_COL,
							new_queen_row,
							QUEEN_STARTING_COL,
							false));
	ASSERT_TRUE(game.move(new_queen_row,
							QUEEN_STARTING_COL,
							new_queen_row - 2,
							QUEEN_STARTING_COL+2,
							true));
}

TEST(GameStateTests, QueenCantJump)
{
	auto game = game::GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_FALSE(game.move(BLACK_HOME_ROW,
							QUEEN_STARTING_COL,
							BLACK_HOME_ROW+2,
							QUEEN_STARTING_COL+2,
							true));
}


TEST(GameStateTests, QueenCantMoveRandomly)
{
	auto game = game::GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move(WHITE_HOME_ROW,
							QUEEN_STARTING_COL,
							new_queen_row,
							QUEEN_STARTING_COL,
							false));
	ASSERT_FALSE(game.move(new_queen_row,
							QUEEN_STARTING_COL,
							new_queen_row - 3,
							QUEEN_STARTING_COL + 2,
							true));
}

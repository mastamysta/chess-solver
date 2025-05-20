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

using namespace game;
TEST(GameStateTests, InitialTurnWhite) 
{
  auto game = GameState();
  auto colour = game.get_turn();
  ASSERT_EQ(colour, game::Colour::WHITE);
}

TEST(GameStateTests, GetBoard)
{
	auto game = GameState();
	auto board = game.get_board();
}

TEST(GameStateTests, BoardSize)
{
	auto game = GameState();
	auto board = game.get_board();
	auto piece = board[7][7];
}

TEST(GameStateTests, InitialPawnRows)
{
	auto game = GameState();

	for (int i = 0; i < 8; i++)
	{
		ASSERT_EQ(game.get_board()[1][i], Piece(Type::PAWN, Colour::BLACK));
		ASSERT_EQ(game.get_board()[6][i], Piece(Type::PAWN, Colour::WHITE));
	}
}

TEST(GameStateTests, InitialRooks)
{
	auto game = GameState();

	ASSERT_EQ(game.get_board()[0][0], Piece(Type::ROOK, Colour::BLACK));
	ASSERT_EQ(game.get_board()[0][7], Piece(Type::ROOK, Colour::BLACK));
	ASSERT_EQ(game.get_board()[7][7], Piece(Type::ROOK, Colour::WHITE));
	ASSERT_EQ(game.get_board()[7][0], Piece(Type::ROOK, Colour::WHITE));
}

TEST(GameStateTests, InitialKnights)
{
	auto game = GameState();

	ASSERT_EQ(game.get_board()[0][1], Piece(Type::KNIGHT, Colour::BLACK));
	ASSERT_EQ(game.get_board()[0][6], Piece(Type::KNIGHT, Colour::BLACK));
	ASSERT_EQ(game.get_board()[7][1], Piece(Type::KNIGHT, Colour::WHITE));
	ASSERT_EQ(game.get_board()[7][6], Piece(Type::KNIGHT, Colour::WHITE));
}

TEST(GameStateTests, InitialBishops)
{
	auto game = GameState();

	ASSERT_EQ(game.get_board()[0][2], Piece(Type::BISHOP, Colour::BLACK));
	ASSERT_EQ(game.get_board()[0][5], Piece(Type::BISHOP, Colour::BLACK));
	ASSERT_EQ(game.get_board()[7][2], Piece(Type::BISHOP, Colour::WHITE));
	ASSERT_EQ(game.get_board()[7][5], Piece(Type::BISHOP, Colour::WHITE));
}

TEST(GameStateTests, InitialKings)
{
	auto game = GameState();

	ASSERT_EQ(game.get_board()[BLACK_KING_STARTING_ROW][KING_STARTING_COL],
										Piece(Type::KING, Colour::BLACK));
	ASSERT_EQ(game.get_board()[WHITE_KING_STARTING_ROW][KING_STARTING_COL], 
										Piece(Type::KING, Colour::WHITE));
}

TEST(GameStateTests, InitialQueens)
{
	auto game = GameState();
	ASSERT_EQ(game.get_board()[BLACK_HOME_ROW][QUEEN_STARTING_COL],
									Piece(Type::QUEEN, Colour::BLACK));
	ASSERT_EQ(game.get_board()[WHITE_HOME_ROW][QUEEN_STARTING_COL], 
									Piece(Type::QUEEN, Colour::WHITE));
}

TEST(GameStateTests, GetAt)
{
	auto game = GameState();
	ASSERT_EQ(game.at({ 0, 3 }), Piece(Type::QUEEN, Colour::BLACK));
	ASSERT_EQ(game.at({ 7, 4 }), Piece(Type::KING, Colour::WHITE));
}


TEST(GameStateTests, CanMovePawnOne)
{
	auto game = GameState();

	Position o = { 1, 4 }, d = { o.first + 1, o.second };

	ASSERT_TRUE(game.move(o, d, true));
	ASSERT_EQ(game.at(o), Piece());
	ASSERT_EQ(game.at(d), Piece(Type::PAWN, Colour::BLACK));
}
TEST(GameStateTests, CanMovePawnTwoOnFirst)
{
	auto game = GameState();

	Position o = { 1, 4 }, d = { o.first + 2, o.second };

	ASSERT_TRUE(game.move(o, d, true));
	ASSERT_EQ(game.at(o), Piece());
	ASSERT_EQ(game.at(d), Piece(Type::PAWN, Colour::BLACK));
}

TEST(GameStateTests, CantMovePawnDiag)
{
	auto game = GameState();

	Position o = { 1, 4 }, d = { o.first + 2, o.second + 1 };

	ASSERT_FALSE(game.move(o, d, true));
	ASSERT_EQ(game.at(o), Piece(Type::PAWN, Colour::BLACK));
	ASSERT_EQ(game.at(d), Piece());
}

TEST(GameStateTests, CantMoveBlackPawnBack)
{
	auto game = GameState();

	// First move the black pawn forward.
	Position o = { 1, 4 }, d = { o.first + 2, o.second };
	ASSERT_TRUE(game.move(o, d, true));
	// Now try and move it backwards.
	ASSERT_FALSE(game.move(o, d, true));
}

TEST(GameStateTests, CantMoveWhitePawnBack)
{
	auto game = GameState();

	// First move the white pawn forward.
	Position o = { 6, 4 }, d = { o.first - 2, o.second };
	ASSERT_TRUE(game.move(o, d, true));
	// Now try and move it backwards.
	ASSERT_FALSE(game.move(d, { d.first + 1, d.second}, true));
}

TEST(GameStateTests, CantMoveWhitePawnTwoOtherwise)
{
	auto game = GameState();

	// First move the white pawn forward.
	Position o = { 6, 4 }, d = { o.first - 1, o.second };
	ASSERT_TRUE(game.move(o, d, true));
	// Now try and move it forward two.
	ASSERT_FALSE(game.move(d, { d.first - 2, d.second }, true));
}

TEST(GameStateTests, CantMoveBlackPawnTwoOtherwise)
{
	auto game = GameState();

	Position o = { 1, 4 }, d = { o.first + 1, o.second };
	ASSERT_TRUE(game.move(o, d, true));
	ASSERT_FALSE(game.move(d, { d.first + 2, d.second }, true));
}

TEST(GameStateTests, PawnCantMoveZero)
{
	auto game = GameState();

	Position o = { 1, 4}, d = o;
	ASSERT_FALSE(game.move(o, d, true));
}

TEST(GameStateTests, BlackPawnCantJump)
{
	auto game = GameState();

	// First move the black king in front of a pawn.
	ASSERT_TRUE(game.move({ BLACK_KING_STARTING_ROW,
						  KING_STARTING_COL },
						  {BLACK_KING_STARTING_ROW+2,
						  KING_STARTING_COL},
						  false));
	// Now try to skip pawn over king.
	ASSERT_FALSE(game.move({ BLACK_KING_STARTING_ROW + 1,
						   KING_STARTING_COL },
						   {BLACK_KING_STARTING_ROW+3, 
						   KING_STARTING_COL},
						   true));
}

TEST(GameStateTests, WhitePawnCantJump)
{
	auto game = GameState();

	// First move the white king in front of a pawn.
	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{ WHITE_PAWN_ROW - 1,
							KING_STARTING_COL},
							false));
	// Now try to skip pawn over king.
	ASSERT_FALSE(game.move({ WHITE_PAWN_ROW,
							KING_STARTING_COL },
							{ WHITE_PAWN_ROW - 2,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, PawnCantCaptureOneForward)
{
	auto game = GameState();
	auto new_king_row = BLACK_PAWN_ROW+1;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
						  KING_STARTING_COL },
						  {new_king_row,
						  KING_STARTING_COL},
						  false));
	ASSERT_FALSE(game.move({ 1,
						   KING_STARTING_COL },
						   {new_king_row,
						   KING_STARTING_COL},
						   true));
}

TEST(GameStateTests, PawnCantCaptureTwoForward)
{
	auto game = GameState();
	auto new_king_row = BLACK_PAWN_ROW+2;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_FALSE(game.move({ BLACK_PAWN_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, BlackPawnCanCaptureOneDiagonally)
{
	auto game = GameState();
	auto new_king_row = BLACK_PAWN_ROW+1;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ BLACK_PAWN_ROW,
							KING_STARTING_COL - 1 },
							{new_king_row,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, WhitePawnCanCaptureOneDiagonally)
{
	auto game = GameState();
	auto new_king_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ BLACK_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ WHITE_PAWN_ROW,
							KING_STARTING_COL - 1 },
							{new_king_row,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, PawnCantCaptureOwnPiece)
{
	auto game = GameState();
	auto new_pawn_col = 4;
	auto new_pawn_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_PAWN_ROW,
							0 },
							{new_pawn_row,
							new_pawn_col},
							false));
	ASSERT_FALSE(game.move({ WHITE_PAWN_ROW,
							new_pawn_col - 1 },
							{new_pawn_row,
							new_pawn_col},
							true));
}


TEST(GameStateTests, PawnPromotes)
{
	auto game = GameState();

	ASSERT_TRUE(game.move({ WHITE_PAWN_ROW,
							0 },
							{BLACK_PAWN_ROW,
							0},
							false));
	ASSERT_TRUE(game.move({ BLACK_PAWN_ROW,
							0 },
							{BLACK_HOME_ROW,
							1},
							true));
	ASSERT_EQ(game.at({ BLACK_HOME_ROW, 1 }), Piece(Type::QUEEN, Colour::WHITE));
}

TEST(GameStateTests, KingCantCaptureOwnPiece)
{
	auto game = GameState();

	ASSERT_FALSE(game.move({ BLACK_KING_STARTING_ROW,
							KING_STARTING_COL },
							{BLACK_KING_STARTING_ROW+1,
							KING_STARTING_COL},
							true));

}


TEST(GameStateTests, KingCantMoveNowhere)
{
	auto game = GameState();

	ASSERT_FALSE(game.move({ BLACK_KING_STARTING_ROW,
							KING_STARTING_COL },
							{BLACK_KING_STARTING_ROW,
							KING_STARTING_COL},
							true));
}


TEST(GameStateTests, KingCantMoveMoreThanOneForward)
{
	auto game = GameState();

	ASSERT_FALSE(game.move({ BLACK_KING_STARTING_ROW,
							KING_STARTING_COL },
							{BLACK_KING_STARTING_ROW+2,
							KING_STARTING_COL},
							true));
}


TEST(GameStateTests, KingCantMoveMoreThanOneSideways)
{
	auto game = GameState();
	auto new_king_row = 4;
	
	ASSERT_TRUE(game.move({ BLACK_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_FALSE(game.move({ new_king_row,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL+2},
							true));
}


TEST(GameStateTests, KingCanCapture)
{
	auto game = GameState();
	auto new_king_row = 4;
	auto new_pawn_row = new_king_row-1;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ BLACK_PAWN_ROW,
							KING_STARTING_COL },
							{ new_pawn_row,
							KING_STARTING_COL },
							false));
	ASSERT_TRUE(game.move({ new_king_row,
							KING_STARTING_COL },
							{ new_pawn_row,
							KING_STARTING_COL},
							true));
}


TEST(GameStateTests, KingCanMove)
{
	auto game = GameState();
	auto new_king_row = BLACK_PAWN_ROW + 1;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{ new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ new_king_row,
							KING_STARTING_COL },
							{new_king_row+1,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, KingCantMoveIntoCheck)
{
	auto game = GameState();
	auto new_king_row = BLACK_PAWN_ROW + 2;

	ASSERT_TRUE(game.move({ WHITE_KING_STARTING_ROW,
							KING_STARTING_COL },
							{new_king_row,
							KING_STARTING_COL},
							false));
	ASSERT_FALSE(game.move({ new_king_row,
							KING_STARTING_COL },
							{new_king_row - 1,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, RookCanMove)
{
	auto game = GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{new_rook_row,
							LEFT_ROOK_COL},
							false));
	ASSERT_TRUE(game.move({ new_rook_row,
							LEFT_ROOK_COL },
							{new_rook_row - 1,
							LEFT_ROOK_COL},
							true));
}


TEST(GameStateTests, RookCantMoveDiagonally)
{
	auto game = GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{new_rook_row,
							LEFT_ROOK_COL},
							false));
	ASSERT_FALSE(game.move({ new_rook_row,
							LEFT_ROOK_COL },
							{new_rook_row-1,
							LEFT_ROOK_COL+1},
							true));
}


TEST(GameStateTests, RookCantJump)
{
	auto game = GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_FALSE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{new_rook_row,
							LEFT_ROOK_COL},
							true));
}


TEST(GameStateTests, RookCanCapture)
{
	auto game = GameState();
	auto new_rook_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{new_rook_row,
							LEFT_ROOK_COL},
							false));
	ASSERT_TRUE(game.move({ new_rook_row,
							LEFT_ROOK_COL },
							{BLACK_PAWN_ROW,
							LEFT_ROOK_COL},
							true));
}

TEST(GameStateTests, CanCastle)
{
	auto game = GameState();

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 1 },
							{WHITE_PAWN_ROW-1,
							LEFT_ROOK_COL+1},
							false));
	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 2 },
							{WHITE_PAWN_ROW-1,
							LEFT_ROOK_COL+2},
							false));

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{WHITE_HOME_ROW,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, CantCastleAfterMoving)
{
	auto game = GameState();

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 1 },
							{WHITE_PAWN_ROW - 1,
							LEFT_ROOK_COL + 1},
							false));
	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 2 },
							{WHITE_PAWN_ROW - 1,
							LEFT_ROOK_COL + 2},
							false));

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{WHITE_HOME_ROW,
							LEFT_ROOK_COL+1},
							true));
	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{WHITE_HOME_ROW,
							LEFT_ROOK_COL},
							true));

	ASSERT_FALSE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{WHITE_HOME_ROW,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, CantCastleThroughAttackedCells)
{
	auto game = GameState();

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 1 },
							{WHITE_PAWN_ROW - 1,
							LEFT_ROOK_COL + 1},
							false));
	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL + 2 },
							{WHITE_PAWN_ROW - 1,
							LEFT_ROOK_COL + 2},
							false));
	ASSERT_TRUE(game.move({ BLACK_HOME_ROW,
							LEFT_KNIGHT_COL },
							{WHITE_PAWN_ROW - 1,
							LEFT_BISHOP_COL},
							false));

	ASSERT_FALSE(game.move({ WHITE_HOME_ROW,
							LEFT_ROOK_COL },
							{WHITE_HOME_ROW,
							KING_STARTING_COL},
							true));
}

TEST(GameStateTests, BishopCanMove)
{
	auto game = GameState();
	auto new_bishop_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_BISHOP_COL },
							{new_bishop_row,
							LEFT_BISHOP_COL},
							false));
	ASSERT_TRUE(game.move({ new_bishop_row,
							LEFT_BISHOP_COL },
							{new_bishop_row-1,
							LEFT_BISHOP_COL+1},
							true));
}


TEST(GameStateTests, BishopCantMoveNonDiagonal)
{
	auto game = GameState();
	auto new_bishop_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							LEFT_BISHOP_COL },
							{new_bishop_row,
							LEFT_BISHOP_COL},
							false));
	ASSERT_FALSE(game.move({ new_bishop_row,
							LEFT_BISHOP_COL },
							{new_bishop_row,
							LEFT_BISHOP_COL + 1},
							true));
}


TEST(GameStateTests, BishopCantJump)
{
	auto game = GameState();

	ASSERT_FALSE(game.move({ WHITE_HOME_ROW,
							LEFT_BISHOP_COL },
							{WHITE_HOME_ROW-2,
							LEFT_BISHOP_COL+2},
							true));
}

TEST(GameStateTests, KnightCanMove)
{
	auto game = GameState();

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							RIGHT_KNIGHT_COL },
							{WHITE_HOME_ROW - 2,
							RIGHT_KNIGHT_COL + 1},
							true));
}

TEST(GameStateTests, KnightCantMoveFar)
{
	auto game = GameState();

	ASSERT_FALSE(game.move({ WHITE_HOME_ROW,
							RIGHT_KNIGHT_COL },
							{WHITE_HOME_ROW - 2,
							RIGHT_KNIGHT_COL - 3},
							true));
}


TEST(GameStateTests, QueenCanMoveStraight)
{
	auto game = GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							QUEEN_STARTING_COL },
							{new_queen_row,
							QUEEN_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ new_queen_row,
							QUEEN_STARTING_COL },
							{new_queen_row-1,
							QUEEN_STARTING_COL},
							true));
}

TEST(GameStateTests, QueenCanMoveDiagonally)
{
	auto game = GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							QUEEN_STARTING_COL },
							{new_queen_row,
							QUEEN_STARTING_COL},
							false));
	ASSERT_TRUE(game.move({ new_queen_row,
							QUEEN_STARTING_COL },
							{new_queen_row - 2,
							QUEEN_STARTING_COL+2},
							true));
}

TEST(GameStateTests, QueenCantJump)
{
	auto game = GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_FALSE(game.move({ BLACK_HOME_ROW,
							QUEEN_STARTING_COL },
							{BLACK_HOME_ROW+2,
							QUEEN_STARTING_COL+2},
							true));
}


TEST(GameStateTests, QueenCantMoveRandomly)
{
	auto game = GameState();
	auto new_queen_row = WHITE_PAWN_ROW - 1;

	ASSERT_TRUE(game.move({ WHITE_HOME_ROW,
							QUEEN_STARTING_COL },
							{new_queen_row,
							QUEEN_STARTING_COL},
							false));
	ASSERT_FALSE(game.move({ new_queen_row,
							QUEEN_STARTING_COL },
							{new_queen_row - 3,
							QUEEN_STARTING_COL + 2},
							true));
}

TEST(GameStateTests, StalemateNoMoves)
{
	auto game = GameState();
	game.clear_board();
	game.place_at(0, 0, Piece(Type::KING, Colour::BLACK));
	game.place_at(1, 7, Piece(Type::ROOK, Colour::WHITE));
	game.place_at(7, 1, Piece(Type::ROOK, Colour::WHITE));
	game.set_turn(game::Colour::BLACK);
	game.update_all();
	
	ASSERT_TRUE(game.get_stalemate());
}

TEST(GameStateTests, StalemateInsufficientMaterial)
{
	auto game = GameState();
	game.clear_board();
	game.place_at(0, 0, Piece(Type::KING, Colour::BLACK));
	game.place_at(7, 7, Piece(Type::KING, Colour::WHITE));
	game.set_turn(game::Colour::BLACK);
	game.update_all();

	ASSERT_TRUE(game.get_stalemate());
}

TEST(GameStateTests, BlackCheckMate)
{
	auto game = GameState();
	game.clear_board();
	game.place_at(0, 0, Piece(Type::KING, Colour::BLACK));
	game.place_at(7, 1, Piece(Type::ROOK, Colour::WHITE));
	game.place_at(7, 0, Piece(Type::ROOK, Colour::WHITE));
	game.set_turn(game::Colour::BLACK);
	game.update_all();

	ASSERT_TRUE(game.get_checkmate());
}

TEST(GameStateTests, WhiteCheckMate)
{
	auto game = GameState();
	game.clear_board();
	game.place_at(0, 0, Piece(Type::KING, Colour::WHITE));
	game.place_at(7, 1, Piece(Type::ROOK, Colour::BLACK));
	game.place_at(7, 0, Piece(Type::ROOK, Colour::BLACK));
	game.set_turn(game::Colour::WHITE);
	game.update_all();

	ASSERT_TRUE(game.get_checkmate());
}

TEST(GameStateTests, MustAddressCheck)
{
	auto game = GameState();
	game.clear_board();
	game.place_at(0, 0, Piece(Type::KING, Colour::BLACK));
	game.place_at(4, 2, Piece(Type::PAWN, Colour::BLACK));
	game.place_at(7, 0, Piece(Type::ROOK, Colour::WHITE));
	game.set_turn(game::Colour::BLACK);
	game.update_all();

	ASSERT_FALSE(game.move({ 4,
							2 },
							{5,
							2},
							true));
}
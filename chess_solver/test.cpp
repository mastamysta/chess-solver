#include "pch.h"

#include "state.hpp"

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
	ASSERT_EQ(game.get_board()[0][4], game::GameState::Piece::BKING);
	ASSERT_EQ(game.get_board()[7][4], game::GameState::Piece::WKING);
}

TEST(GameStateTests, InitialQueens)
{
	auto game = game::GameState();

	// 0th row is for black king
	ASSERT_EQ(game.get_board()[0][3], game::GameState::Piece::BQUEEN);
	ASSERT_EQ(game.get_board()[7][3], game::GameState::Piece::WQUEEN);
}
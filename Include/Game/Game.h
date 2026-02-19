#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "GameMode.h"
#include "GameModeManager.h"
#include "Watcher.h"
#include "Rules.h"
#include "Move.h"
#include "GetPlayer.h"
#include "Color.h"
#include "GamePosition.h"
#include "Piece.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"
#include "Joker.h"
#include "Spy.h"
#include "EventManager.h"
#include <string>
#include <vector>
#include <thread>
#include <memory>
#include <limits>
#include <sstream>

using namespace std;

enum class GameStatus
{
    RUNNING,
    CHECKMATE,
    STALEMATE,
    DRAW,
    MISSION_COMPLETED
};

struct MoveInfo
{
    Move move;
    PieceType capturedPieceType;
    Color capturedPieceColor;
    bool wasEmpty;
    int oldTurnCount;
    GameStatus oldStatus;
};

class Game
{
    friend class GameStorage;

public:
    Game();
    ~Game();

    void start();
    bool isOver() const;
    GameStatus getStatus() const;
    void reset();

    string getWhiteName() const;
    string getBlackName() const;
    string getCurrentPlayerName() const;
    Color getCurrentTurn() const;
    Player *getCurrentPlayer() const;
    int getTurnNumber() const;

    Board *getBoard();
    const Board *getBoard() const;

    bool makeMove(const Move &move);
    bool makeMove(const Position &from, const Position &to);
    bool makeMove(int fromRow, int fromCol, int toRow, int toCol);

    bool isValidMove(const Move &move) const;
    bool isLegalMove(const Move &move) const;
    bool isLegalMove(const Position &from, const Position &to) const;

    vector<Move> getAllValidMoves() const;
    vector<Move> getValidMovesForCurrentPlayer() const;
    vector<Move> getValidMovesForPiece(const Position &pos) const;
    EventManager *getEventManager()
    {
        return eventManager.get();
    }

    bool isCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate() const;
    bool isDraw() const;

    void addMoveToHistory(const Move &move);
    void addMoveToHistory(const string &moveNotation);
    vector<Move> getMoveHistory() const;
    vector<string> getMoveHistoryStrings() const;
    Move getLastMove() const;
    int getMoveCount() const;
    void clearHistory();

    void nextTurn();

    void applyMove(const Move &move);
    void applyMove(const Position &from, const Position &to);
    void applyMove(int fromRow, int fromCol, int toRow, int toCol);
    bool undoLastMove();
    void checkSpyRevelation();

    bool performEnPassant(const Position &from, const Position &to);
    bool performCastling(const Position &from, const Position &to);
    void resetEnPassantFlags();
    void showJokerMenu(Joker *joker);
    void showSpyMenu(Spy *spy);

    void setGameMode(GameMode mode);

    void resetTurnCount()
    {
        turnCount = 0;
    }

    void setStatus(GameStatus newStatus)
    {
        status = newStatus;
    }

    GameMode getGameMode() const
    {
        return currentGameMode;
    }
    GameModeManager *getModeManager()
    {
        return modeManager.get();
    }
    const GameModeManager *getModeManager() const
    {
        return modeManager.get();
    }

    void showSuggestedMoves(const Position &pos) const;
    void showAllSuggestedMoves() const;

    string exportGameState() const;
    void importGameState(const string &state);

    Watcher *getWatcher();
    const Watcher *getWatcher() const;

private:
    unique_ptr<Board> board;
    Watcher watcher;
    GameStatus status;
    vector<Move> moveHistory;
    vector<MoveInfo> moveStack;
    GameMode currentGameMode;
    unique_ptr<GameModeManager> modeManager;
    unique_ptr<EventManager> eventManager;
    int turnCount;

    void initialize();
    void checkGameEndConditions();
    void handleGameOver(GameStatus finalStatus);
    void updateGameState();
    void switchTurn();
};

#endif
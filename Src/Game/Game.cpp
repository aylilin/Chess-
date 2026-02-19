#include "Game.h"

using namespace std;

Game::Game() : board(make_unique<Board>()), status(GameStatus::RUNNING), turnCount(0), currentGameMode(GameMode::Standard), modeManager(std::make_unique<GameModeManager>())
{
    initialize();
}

Game::~Game() = default;

void Game::initialize()
{
    if (currentGameMode == GameMode::Mission)
    {
        board->setupMissionModePosition();
    }
    else
    {
        board->setupStandardPosition();
    }
}

void Game::start()
{
    status = GameStatus::RUNNING;
    watcher.switchturn();

    switch (currentGameMode)
    {
    case GameMode::Mission:
        modeManager->generateMissions(Color::White, turnCount);
        modeManager->generateMissions(Color::Black, turnCount);

        cout << endl;
        cout << "MISSION MODE" << endl;
        cout << "White's mission: " << modeManager->getMissionDescription(Color::White) << endl;
        cout << "Black's mission: " << modeManager->getMissionDescription(Color::Black) << endl;
        cout << endl;
        break;

    case GameMode::Energy:
        cout << endl;
        cout << "ENERGY MODE" << endl;
        cout << "Each move costs energy! Pieces explode if not moved for 5 turns." << endl;
        cout << endl;
        break;

    default:
        break;
    }

    cout << "Game started! It's " << getCurrentPlayerName() << "'s turn." << endl;
}

bool Game::isOver() const
{
    return status != GameStatus::RUNNING;
}

GameStatus Game::getStatus() const
{
    return status;
}

void Game::reset()
{
    board = make_unique<Board>();
    moveHistory.clear();
    moveStack.clear();
    watcher.reset();
    status = GameStatus::RUNNING;
    turnCount = 0;
    initialize();
    cout << "Game reset." << endl;
}

void Game::showSuggestedMoves(const Position &pos) const
{
    Piece *piece = board->getPiece(pos);
    if (!piece)
    {
        cout << "No piece at this position!" << endl;
        return;
    }

    if (piece->getColor() != getCurrentTurn())
    {
        cout << "It's not your turn to move this piece!" << endl;
        return;
    }

    auto moves = getValidMovesForPiece(pos);

    if (moves.empty())
    {
        cout << "This piece has no legal moves." << endl;
        return;
    }

    cout << "Suggested moves for " << (piece->getColor() == Color::White ? "White" : "Black") << " " << piece->getSymbol() << " at " << char('a' + pos.col) << 8 - pos.row << ":" << endl;

    for (size_t i = 0; i < moves.size(); ++i)
    {
        cout << "  " << i + 1 << ". " << moves[i].toString() << endl;
    }
}

void Game::showAllSuggestedMoves() const
{
    auto moves = getValidMovesForCurrentPlayer();

    if (moves.empty())
    {
        cout << "No legal moves available!" << endl;
        if (isCheck(getCurrentTurn()))
        {
            cout << "You are in checkmate!" << endl;
        }
        else
        {
            cout << "Stalemate!" << endl;
        }
        return;
    }

    cout << endl;
    cout << "Suggested Moves for " << getCurrentPlayerName() << ":" << endl;
    cout << "Total " << moves.size() << " legal moves available:" << endl;

    vector<pair<Position, vector<Move>>> groupedMoves;

    for (const auto &move : moves)
    {
        bool found = false;
        for (auto &group : groupedMoves)
        {
            if (group.first == move.from)
            {
                group.second.push_back(move);
                found = true;
                break;
            }
        }
        if (!found)
        {
            groupedMoves.push_back({move.from, {move}});
        }
    }

    for (const auto &group : groupedMoves)
    {
        Piece *piece = board->getPiece(group.first);
        if (!piece)
        {
            continue;
        }

        cout << endl;
        cout << char('a' + group.first.col) << 8 - group.first.row << " (" << piece->getSymbol() << "): ";

        for (size_t i = 0; i < group.second.size(); ++i)
        {
            if (i > 0)
                cout << ", ";
            cout << group.second[i].toShortString();
        }
        cout << endl;
    }
}

string Game::getWhiteName() const
{
    Player *white = watcher.getWhitePlayer();
    return white ? white->getName() : "WHITE";
}

string Game::getBlackName() const
{
    Player *black = watcher.getBlackPlayer();
    return black ? black->getName() : "BLACK";
}

string Game::getCurrentPlayerName() const
{
    Player *current = watcher.getCurrentPlayer();
    return current ? current->getName() : "UNKNOWN";
}

Player *Game::getCurrentPlayer() const
{
    return watcher.getCurrentPlayer();
}

Color Game::getCurrentTurn() const
{
    Player *current = watcher.getCurrentPlayer();
    return current ? current->getColor() : Color::None;
}

int Game::getTurnNumber() const
{
    return turnCount / 2 + 1;
}

Board *Game::getBoard()
{
    return board.get();
}

const Board *Game::getBoard() const
{
    return board.get();
}

void Game::switchTurn()
{
    watcher.switchturn();
}

void Game::nextTurn()
{
    switchTurn();
    turnCount++;
}

void Game::resetEnPassantFlags()
{
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Piece *piece = board->getPiece(Position(r, c));
            if (piece && piece->getType() == PieceType::Pawn)
            {
                Pawn *pawn = dynamic_cast<Pawn *>(piece);
                if (pawn)
                {
                    pawn->setJustDoubleMoved(false);
                }
            }
        }
    }
}

bool Game::performEnPassant(const Position &from, const Position &to)
{
    Piece *piece = board->getPiece(from);
    if (!piece || piece->getType() != PieceType::Pawn)
    {
        return false;
    }

    Pawn *pawn = dynamic_cast<Pawn *>(piece);
    if (!pawn)
    {
        return false;
    }

    Position enemyPawnPos(from.row, to.col);
    Piece *enemyPawn = board->getPiece(enemyPawnPos);

    if (enemyPawn && enemyPawn->getType() == PieceType::Pawn &&
        enemyPawn->getColor() != piece->getColor())
    {
        Pawn *enemyPawnPtr = dynamic_cast<Pawn *>(enemyPawn);
        if (enemyPawnPtr && enemyPawnPtr->getJustDoubleMoved())
        {
            board->removePiece(enemyPawnPos);
            board->movePiece(from, to);
            cout << "En Passant performed!" << endl;
            return true;
        }
    }

    return false;
}

bool Game::performCastling(const Position &from, const Position &to)
{
    Piece *king = board->getPiece(from);
    if (!king || king->getType() != PieceType::King)
    {
        return false;
    }

    if (abs(to.col - from.col) != 2)
    {
        return false;
    }

    bool kingSide = (to.col > from.col);

    King *kingPtr = dynamic_cast<King *>(king);
    if (!kingPtr)
    {
        return false;
    }

    Position rookFrom, rookTo;
    if (!kingPtr->performCastling(*board, kingSide, rookFrom, rookTo))
    {
        return false;
    }

    board->movePiece(from, to);
    board->movePiece(rookFrom, rookTo);
    cout << "Castling performed!" << endl;

    return true;
}

bool Game::isValidMove(const Move &move) const
{
    if (!board->isInside(move.from) || !board->isInside(move.to))
    {
        return false;
    }

    Piece *piece = board->getPiece(move.from);
    if (!piece)
    {
        return false;
    }

    if (piece->getColor() != getCurrentTurn())
    {
        return false;
    }

    Piece *targetPiece = board->getPiece(move.to);
    if (targetPiece && targetPiece->getColor() == piece->getColor())
    {
        return false;
    }

    auto legalMoves = piece->getLegalMoves(*board);
    if (find(legalMoves.begin(), legalMoves.end(), move.to) == legalMoves.end())
    {
        return false;
    }

    Board tempBoard;
    board->copyTo(tempBoard);
    tempBoard.movePiece(move.from, move.to);

    Position kingPos(-1, -1);
    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position pos(r, c);
            Piece *p = tempBoard.getPiece(pos);
            if (p && p->getColor() == piece->getColor() && p->getType() == PieceType::King)
            {
                kingPos = pos;
                break;
            }
        }
        if (kingPos.isValid())
        {
            break;
        }
    }

    if (!kingPos.isValid())
    {
        return false;
    }

    Color opponent = (piece->getColor() == Color::White) ? Color::Black : Color::White;

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position from(r, c);
            Piece *attacker = tempBoard.getPiece(from);
            if (attacker && attacker->getColor() == opponent)
            {
                auto attackerMoves = attacker->getLegalMoves(tempBoard);
                for (const auto &to : attackerMoves)
                {
                    if (to == kingPos)
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool Game::isLegalMove(const Move &move) const
{
    return isValidMove(move);
}

bool Game::isLegalMove(const Position &from, const Position &to) const
{
    return isValidMove(Move(from, to));
}

vector<Move> Game::getAllValidMoves() const
{
    vector<Move> validMoves;

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position from(r, c);
            auto moves = getValidMovesForPiece(from);
            validMoves.insert(validMoves.end(), moves.begin(), moves.end());
        }
    }

    return validMoves;
}

vector<Move> Game::getValidMovesForCurrentPlayer() const
{
    Color currentColor = getCurrentTurn();
    vector<Move> validMoves;

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position from(r, c);
            Piece *piece = board->getPiece(from);

            if (piece && piece->getColor() == currentColor)
            {
                auto legalMoves = piece->getLegalMoves(*board);
                for (const auto &to : legalMoves)
                {
                    Move move(from, to);
                    if (isValidMove(move))
                        validMoves.push_back(move);
                }
            }
        }
    }

    return validMoves;
}

vector<Move> Game::getValidMovesForPiece(const Position &pos) const
{
    vector<Move> validMoves;

    Piece *piece = board->getPiece(pos);
    if (!piece || piece->getColor() != getCurrentTurn())
    {
        return validMoves;
    }

    auto legalMoves = piece->getLegalMoves(*board);
    for (const auto &to : legalMoves)
    {
        Move move(pos, to);
        if (isValidMove(move))
        {
            validMoves.push_back(move);
        }
    }

    return validMoves;
}

void Game::checkSpyRevelation()
{
    if (turnCount % 5 == 0)
    {
        for (int r = 0; r < 8; r++)
        {
            for (int c = 0; c < 8; c++)
            {
                Piece *piece = board->getPiece(Position(r, c));
                if (piece && piece->getType() == PieceType::Pawn)
                {
                    Spy *spy = dynamic_cast<Spy *>(piece);
                    if (spy && !spy->isRevealed())
                    {
                        if (rand() % 100 < 30)
                        {
                            spy->reveal();
                        }
                    }
                }
            }
        }
    }
}

void Game::setGameMode(GameMode mode)
{
    currentGameMode = mode;
    modeManager->setMode(mode);
}

bool Game::makeMove(const Position &from, const Position &to)
{
    return makeMove(Move(from, to));
}

bool Game::makeMove(int fromRow, int fromCol, int toRow, int toCol)
{
    return makeMove(Position(fromRow, fromCol), Position(toRow, toCol));
}

bool Game::makeMove(const Move &move)
{
    if (status != GameStatus::RUNNING)
    {
        cout << "Game is already finished." << endl;
        return false;
    }

    Piece *piece = board->getPiece(move.from);
    if (!piece)
    {
        cout << "No piece at source position!!!" << endl;
        return false;
    }

    resetEnPassantFlags();

    if (piece->getType() == PieceType::Pawn)
    {
        Pawn *pawn = dynamic_cast<Pawn *>(piece);
        if (pawn && pawn->getEnPassantTarget() == move.to)
        {
            if (performEnPassant(move.from, move.to))
            {
                modeManager->onMoveMade(move.from, move.to, *board, turnCount);
                nextTurn();
                checkGameEndConditions();
                return true;
            }
        }
    }

    if (piece->getType() == PieceType::King && abs(move.to.col - move.from.col) == 2)
    {
        if (performCastling(move.from, move.to))
        {
            modeManager->onMoveMade(move.from, move.to, *board, turnCount);
            nextTurn();
            checkGameEndConditions();
            return true;
        }
    }

    if (!modeManager->canPieceMove(piece, move.from, move.to, turnCount))
    {
        cout << "Move restricted by current game mode!" << endl;
        return false;
    }

    if (!isValidMove(move))
    {
        cout << "Invalid move!" << endl;
        return false;
    }

    if (piece->getType() == PieceType::Pawn && abs(move.to.row - move.from.row) == 2)
    {
        Pawn *pawn = dynamic_cast<Pawn *>(piece);
        if (pawn)
        {
            pawn->setJustDoubleMoved(true);
        }
    }

    if (currentGameMode == GameMode::Energy)
    {
        modeManager->getTimeBomb()->registerPiece(piece);
    }

    PieceType capturedType = PieceType::None;
    Color capturedColor = Color::None;

    Piece *capturedPiece = board->getPiece(move.to);
    if (capturedPiece)
    {
        capturedType = capturedPiece->getType();
        capturedColor = capturedPiece->getColor();
    }

    applyMove(move);

    if (capturedType != PieceType::None)
    {
        modeManager->onPieceCaptured(capturedType, capturedColor, turnCount);
    }

    modeManager->onMoveMade(move.from, move.to, *board, turnCount);
    modeManager->updateGame(*board, getCurrentTurn(), turnCount);

    if (currentGameMode == GameMode::Mission)
    {
        modeManager->updateMissions(*board, getCurrentTurn(), move);

        cout << endl;
        cout << "Mission Status" << endl;
        cout << "White: " << modeManager->getMissionDescription(Color::White) << endl;
        cout << "Black: " << modeManager->getMissionDescription(Color::Black) << endl;
        cout << endl;

        if (modeManager->isMissionCompleted(getCurrentTurn()))
        {
            cout << endl;
            cout << "MISSION COMPLETED !!!" << endl;
            cout << getCurrentPlayerName() << " wins by completing the mission!" << endl;
            status = GameStatus::DRAW;
            return true;
        }
    }

    nextTurn();
    return true;
}

void Game::applyMove(const Move &move)
{
    if (!isValidMove(move))
    {
        cout << "Invalid move in applyMove!" << endl;
        return;
    }

    MoveInfo info;
    info.move = move;
    info.oldTurnCount = turnCount;
    info.oldStatus = status;

    Piece *captured = board->getPiece(move.to);
    info.wasEmpty = (captured == nullptr);
    if (captured)
    {
        info.capturedPieceType = captured->getType();
        info.capturedPieceColor = captured->getColor();
    }

    Piece *movingPiece = board->getPiece(move.from);
    PieceType movingType = movingPiece ? movingPiece->getType() : PieceType::None;
    Color movingColor = movingPiece ? movingPiece->getColor() : Color::None;

    board->movePiece(move.from, move.to);

    Piece *movedPiece = board->getPiece(move.to);
    if (movedPiece)
    {
        movedPiece->setHasMoved(true);
    }

    moveHistory.push_back(move);
    moveStack.push_back(info);
    checkGameEndConditions();

    cout << "Move applied: " << move.toString();
    if (!info.wasEmpty)
    {
        cout << " (captured " << (info.capturedPieceColor == Color::White ? "White" : "Black") << " ";
        switch (info.capturedPieceType)
        {
        case PieceType::King:
            cout << "King";
            break;
        case PieceType::Queen:
            cout << "Queen";
            break;
        case PieceType::Rook:
            cout << "Rook";
            break;
        case PieceType::Bishop:
            cout << "Bishop";
            break;
        case PieceType::Knight:
            cout << "Knight";
            break;
        case PieceType::Pawn:
            cout << "Pawn";
            break;
        default:
            cout << "Piece";
            break;
        }
        cout << ")";
    }
    cout << endl;

    if (movingType == PieceType::Pawn)
    {
        int promotionRow = (movingColor == Color::White) ? 0 : 7;
        if (move.to.row == promotionRow)
        {
            cout << "Pawn reached promotion square !!!" << endl;
            cout << "Pawn promotion! Choose piece (Q/R/B/N): ";
            char choice;
            cin >> choice;

            Position pos = move.to;
            board->removePiece(pos);

            unique_ptr<Piece> newPiece;
            switch (toupper(choice))
            {
            case 'Q':
                newPiece = make_unique<Queen>(movingColor, pos);
                break;
            case 'R':
                newPiece = make_unique<Rook>(movingColor, pos);
                break;
            case 'B':
                newPiece = make_unique<Bishop>(movingColor, pos);
                break;
            case 'N':
                newPiece = make_unique<Knight>(movingColor, pos);
                break;
            default:
                newPiece = make_unique<Queen>(movingColor, pos);
            }

            board->placePiece(std::move(newPiece));
        }
    }
}

void Game::applyMove(const Position &from, const Position &to)
{
    applyMove(Move(from, to));
}

void Game::applyMove(int fromRow, int fromCol, int toRow, int toCol)
{
    applyMove(Move(fromRow, fromCol, toRow, toCol));
}

bool Game::undoLastMove()
{
    if (moveStack.empty())
    {
        cout << "No moves to undo!" << endl;
        return false;
    }

    MoveInfo lastInfo = moveStack.back();
    moveStack.pop_back();

    board->movePiece(lastInfo.move.to, lastInfo.move.from);

    if (!lastInfo.wasEmpty)
    {
        unique_ptr<Piece> capturedPiece;
        Position capturePos = lastInfo.move.to;

        switch (lastInfo.capturedPieceType)
        {
        case PieceType::King:
            capturedPiece = make_unique<King>(lastInfo.capturedPieceColor, capturePos);
            break;
        case PieceType::Queen:
            capturedPiece = make_unique<Queen>(lastInfo.capturedPieceColor, capturePos);
            break;
        case PieceType::Rook:
            capturedPiece = make_unique<Rook>(lastInfo.capturedPieceColor, capturePos);
            break;
        case PieceType::Bishop:
            capturedPiece = make_unique<Bishop>(lastInfo.capturedPieceColor, capturePos);
            break;
        case PieceType::Knight:
            capturedPiece = make_unique<Knight>(lastInfo.capturedPieceColor, capturePos);
            break;
        case PieceType::Pawn:
            capturedPiece = make_unique<Pawn>(lastInfo.capturedPieceColor, capturePos);
            break;
        default:
            cout << "Warning: Unknown piece type in undo!" << endl;
            break;
        }

        if (capturedPiece)
        {
            board->placePiece(move(capturedPiece));
        }
    }

    moveHistory.pop_back();
    turnCount = lastInfo.oldTurnCount;
    status = lastInfo.oldStatus;
    switchTurn();

    cout << "Move undone: " << lastInfo.move.toString() << endl;
    return true;
}

bool Game::isCheck(Color color) const
{
    if (!board)
    {
        return false;
    }

    Position kingPos(-1, -1);

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position pos(r, c);
            Piece *piece = board->getPiece(pos);
            if (piece && piece->getColor() == color && piece->getType() == PieceType::King)
            {
                kingPos = pos;
                break;
            }
        }
        if (kingPos.isValid())
        {
            break;
        }
    }

    if (!kingPos.isValid())
    {
        return false;
    }

    Color opponent = (color == Color::White) ? Color::Black : Color::White;

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position from(r, c);
            Piece *attacker = board->getPiece(from);

            if (attacker && attacker->getColor() == opponent)
            {
                auto moves = attacker->getLegalMoves(*board);
                for (const auto &to : moves)
                {
                    if (to == kingPos)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool Game::isCheckmate(Color color) const
{
    if (!isCheck(color))
    {
        return false;
    }

    for (int r = 0; r < Board::SIZE; ++r)
    {
        for (int c = 0; c < Board::SIZE; ++c)
        {
            Position from(r, c);
            Piece *piece = board->getPiece(from);
            if (piece && piece->getColor() == color)
            {
                auto legalMoves = piece->getLegalMoves(*board);
                for (const auto &to : legalMoves)
                {
                    Move move(from, to);
                    if (isValidMove(move))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool Game::isStalemate() const
{
    if (isCheck(getCurrentTurn()))
        return false;

    return getValidMovesForCurrentPlayer().empty();
}

bool Game::isDraw() const
{
    return isStalemate();
}

void Game::checkGameEndConditions()
{
    Color opponentColor = (getCurrentTurn() == Color::White) ? Color::Black : Color::White;

    if (isCheckmate(opponentColor))
    {
        status = GameStatus::CHECKMATE;
        handleGameOver(GameStatus::CHECKMATE);
    }
    else if (isStalemate())
    {
        status = GameStatus::STALEMATE;
        handleGameOver(GameStatus::STALEMATE);
    }
}

void Game::handleGameOver(GameStatus finalStatus)
{
    switch (finalStatus)
    {
    case GameStatus::CHECKMATE:
        cout << endl;
        cout << "        CHECKMATE!          " << endl;
        cout << (getCurrentTurn() == Color::White ? "Black" : "White") << " wins!" << endl;
        break;
    case GameStatus::STALEMATE:
        cout << endl;
        cout << "        STALEMATE!          " << endl;
        cout << "Game is draw." << endl;
        break;
    default:
        break;
    }
}

void Game::addMoveToHistory(const Move &move)
{
    moveHistory.push_back(move);
}

void Game::addMoveToHistory(const string &moveNotation)
{
    Move move = Move::fromString(moveNotation);
    moveHistory.push_back(move);
}

vector<Move> Game::getMoveHistory() const
{
    return moveHistory;
}

vector<string> Game::getMoveHistoryStrings() const
{
    vector<string> historyStrings;
    for (const Move &move : moveHistory)
        historyStrings.push_back(move.toString());
    return historyStrings;
}

Move Game::getLastMove() const
{
    if (moveHistory.empty())
        return Move();
    return moveHistory.back();
}

int Game::getMoveCount() const
{
    return moveHistory.size();
}

void Game::clearHistory()
{
    moveHistory.clear();
    moveStack.clear();
}

string Game::exportGameState() const
{
    stringstream ss;
    ss << "Turn: " << turnCount << endl;
    ss << "Status: " << static_cast<int>(status) << endl;
    ss << "GameMode: " << static_cast<int>(currentGameMode) << endl;
    ss << "MoveHistory: ";
    for (const auto &move : moveHistory)
        ss << move.toString() << " ";
    ss << endl;
    return ss.str();
}

void Game::importGameState(const string &state)
{
    board->setupStandardPosition();
    clearHistory();
    status = GameStatus::RUNNING;
    turnCount = 0;
    watcher.reset();
}

Watcher *Game::getWatcher()
{
    return &watcher;
}

const Watcher *Game::getWatcher() const
{
    return &watcher;
}

void Game::showJokerMenu(Joker *joker)
{
    if (!joker || joker->gethasMimickedThisTurn())
    {
        cout << "Joker cannot mimic this turn!" << endl;
        return;
    }

    cout << endl;
    cout << "JOKER MIMIC MENU" << endl;
    cout << "Select piece to mimic:" << endl;
    cout << "1. Queen" << endl;
    cout << "2. Rook" << endl;
    cout << "3. Bishop" << endl;
    cout << "4. Knight" << endl;
    cout << "5. Pawn" << endl;
    cout << "0. Cancel" << endl;
    cout << "Choice: ";

    int choice;
    cin >> choice;

    PieceType type;
    switch (choice)
    {
    case 1:
        type = PieceType::Queen;
        break;
    case 2:
        type = PieceType::Rook;
        break;
    case 3:
        type = PieceType::Bishop;
        break;
    case 4:
        type = PieceType::Knight;
        break;
    case 5:
        type = PieceType::Pawn;
        break;
    default:
        cout << "Mimic cancelled." << endl;
        return;
    }

    if (joker->mimicPiece(type, *board))
    {
        cout << "Joker now mimics a ";
        switch (type)
        {
        case PieceType::Queen:
            cout << "Queen";
            break;
        case PieceType::Rook:
            cout << "Rook";
            break;
        case PieceType::Bishop:
            cout << "Bishop";
            break;
        case PieceType::Knight:
            cout << "Knight";
            break;
        case PieceType::Pawn:
            cout << "Pawn";
            break;
        default:
            break;
        }
        cout << " for this turn!" << endl;
    }
}

void Game::showSpyMenu(Spy *spy)
{
    if (!spy || spy->isRevealed())
    {
        cout << "Spy is already revealed!" << endl;
        return;
    }

    cout << endl;
    cout << "SPY REVEAL MENU" << endl;
    cout << "Do you want to reveal the spy now?" << endl;
    cout << "1. Yes (reveal and gain queen abilities)" << endl;
    cout << "2. No (stay disguised)" << endl;
    cout << "Choice: ";

    int choice;
    cin >> choice;

    if (choice == 1)
    {
        spy->reveal();
    }
    else
    {
        cout << "Spy remains disguised." << endl;
    }
}
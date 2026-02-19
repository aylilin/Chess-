#include "Board.h"

//constructor
Board::Board()
{
    clear();
}

//clear the board
void Board::clear()
{
    grid.resize(SIZE);
    for (auto &row : grid)
    {
        row.resize(SIZE);
        for (auto &cell : row)
        {
            cell.reset();
        }
    }
}

//check the board
bool Board::isInside(const Position &pos) const
{
    return pos.row >= 0 && pos.row < SIZE && pos.col >= 0 && pos.col < SIZE;
}

bool Board::isEmpty(const Position &pos) const
{
    return isInside(pos) && !grid[pos.row][pos.col];
}

Piece *Board::getPiece(const Position &pos) const
{
    if (!isInside(pos))
    {
        return nullptr;
    }
    return grid[pos.row][pos.col].get();
}

void Board::placePiece(unique_ptr<Piece> piece)
{
    if (!piece)
    {
        return;
    }

    Position pos = piece->getPosition();
    if (!isInside(pos))
    {
        return;
    }

    grid[pos.row][pos.col] = move(piece);
}

unique_ptr<Piece> Board::removePiece(const Position &pos)
{
    if (!isInside(pos))
    {
        return nullptr;
    }

    auto removed = move(grid[pos.row][pos.col]);
    grid[pos.row][pos.col].reset();
    return removed;
}

void Board::movePiece(const Position &from, const Position &to)
{
    if (!isInside(from) || !isInside(to))
    {
        return;
    }

    auto piece = removePiece(from);
    if (piece)
    {
        piece->setPosition(to);
        piece->setHasMoved(true);
        placePiece(move(piece));
    }
}

// doing something for evey piece
void Board::forEachPiece(const function<void(const Piece &)> &func) const
{
    for (const auto &row : grid)
    {
        for (const auto &cell : row)
        {
            if (cell)
            {
                func(*cell);
            }
        }
    }
}

void Board::print() const
{
    cout << endl;
    cout << "  a b c d e f g h" << endl;
    cout << "  -----------------" << endl;

    for (int r = 0; r < SIZE; ++r)
    {
        cout << 8 - r << "|";
        for (int c = 0; c < SIZE; ++c)
        {
            if ((r + c) % 2 == 0)
            {
                cout << "\033[47m";
            }
            else
            {
                cout << "\033[40m";
            }

            if (grid[r][c])
            {
                if (grid[r][c]->getColor() == Color::White)
                {
                    cout << "\033[1;37m";
                }
                else
                {
                    cout << "\033[1;30m";
                }

                cout << " " << grid[r][c]->getSymbol();
            }
            else
            {
                cout << " .";
            }

            cout << "\033[0m";
        }
        cout << "|" << 8 - r << endl;
    }
    cout << "  -----------------" << endl;
    cout << "  a b c d e f g h" << endl;
    cout << endl;
}

void Board::copyTo(Board &other) const
{
    other.clear();

    for (int r = 0; r < SIZE; ++r)
    {
        for (int c = 0; c < SIZE; ++c)
        {
            if (grid[r][c])
            {
                const Piece *original = grid[r][c].get();
                Position pos(r, c);
                Color color = original->getColor();
                bool hasMoved = original->getHasMoved();

                switch (original->getType())
                {
                case PieceType::King:
                {
                    auto newPiece = make_unique<King>(color, pos);
                    newPiece->setHasMoved(hasMoved);
                    other.placePiece(move(newPiece));
                    break;
                }

                case PieceType::Queen:
                {
                    // checking the armored queen
                    const ArmoredQueen *aq = dynamic_cast<const ArmoredQueen *>(original);
                    if (aq)
                    {
                        auto newQueen = make_unique<ArmoredQueen>(color, pos);
                        newQueen->setHasMoved(hasMoved);
                        //copy armor
                        int armorLevel = aq->getArmor();
                        for (int i = 0; i < 2 - armorLevel; i++) {
                            newQueen->takeHit();
                        }
                        other.placePiece(move(newQueen));
                    }
                    else
                    {
                        auto newQueen = make_unique<Queen>(color, pos);
                        newQueen->setHasMoved(hasMoved);
                        other.placePiece(move(newQueen));
                    }
                    break;
                }

                case PieceType::Rook:
                {
                    auto newRook = make_unique<Rook>(color, pos);
                    newRook->setHasMoved(hasMoved);
                    other.placePiece(move(newRook));
                    break;
                }

                case PieceType::Bishop:
                {
                    auto newBishop = make_unique<Bishop>(color, pos);
                    newBishop->setHasMoved(hasMoved);
                    other.placePiece(move(newBishop));
                    break;
                }

                case PieceType::Knight:
                {
                    auto newKnight = make_unique<Knight>(color, pos);
                    newKnight->setHasMoved(hasMoved);
                    other.placePiece(move(newKnight));
                    break;
                }

                case PieceType::Pawn:
                {
                    //checking the pawn status
                    const SpecialPawn *sp = dynamic_cast<const SpecialPawn *>(original);
                    if (sp)
                    {
                        auto newPawn = make_unique<SpecialPawn>(color, pos);
                        newPawn->setHasMoved(hasMoved);
                        other.placePiece(move(newPawn));
                    }
                    else
                    {
                        const Joker *joker = dynamic_cast<const Joker *>(original);
                        if (joker)
                        {
                            auto newJoker = make_unique<Joker>(color, pos);
                            newJoker->setHasMoved(hasMoved);
                            
                            // copy joker
                            if (joker->isMimicking()) {
                                PieceType mimickedType = joker->getMimickedType();
                                newJoker->setMimickedTypeDirect(mimickedType);
                            }
                            
                            other.placePiece(move(newJoker));
                        }
                        else
                        {
                            const Spy *spy = dynamic_cast<const Spy *>(original);
                            if (spy)
                            {
                                auto newSpy = make_unique<Spy>(color, pos);
                                newSpy->setHasMoved(hasMoved);
                                
                                // copy spy status
                                if (spy->isRevealed()) {
                                    newSpy->forceReveal();
                                }
                                
                                other.placePiece(move(newSpy));
                            }
                            else
                            {
                                auto newPawn = make_unique<Pawn>(color, pos);
                                newPawn->setHasMoved(hasMoved);
                                
                                // checking en passant
                                const Pawn *originalPawn = dynamic_cast<const Pawn*>(original);
                                if (originalPawn) {
                                    Pawn* newPawnPtr = dynamic_cast<Pawn*>(newPawn.get());
                                    if (newPawnPtr) {
                                        newPawnPtr->setJustDoubleMoved(originalPawn->getJustDoubleMoved());
                                    }
                                }
                                
                                other.placePiece(move(newPawn));
                            }
                        }
                    }
                    break;
                }

                case PieceType::ArmoredQueen:
                {
                    auto newQueen = make_unique<ArmoredQueen>(color, pos);
                    newQueen->setHasMoved(hasMoved);
                    const ArmoredQueen *aq = dynamic_cast<const ArmoredQueen *>(original);
                    if (aq) {
                        int armorLevel = aq->getArmor();
                        for (int i = 0; i < 2 - armorLevel; i++) {
                            newQueen->takeHit();
                        }
                    }
                    other.placePiece(move(newQueen));
                    break;
                }

                case PieceType::Joker:
                {
                    auto newJoker = make_unique<Joker>(color, pos);
                    newJoker->setHasMoved(hasMoved);
                    const Joker *joker = dynamic_cast<const Joker *>(original);
                    if (joker && joker->isMimicking()) {
                        newJoker->setMimickedTypeDirect(joker->getMimickedType());
                    }
                    other.placePiece(move(newJoker));
                    break;
                }

                case PieceType::Spy:
                {
                    auto newSpy = make_unique<Spy>(color, pos);
                    newSpy->setHasMoved(hasMoved);
                    const Spy *spy = dynamic_cast<const Spy *>(original);
                    if (spy && spy->isRevealed()) {
                        newSpy->forceReveal();
                    }
                    other.placePiece(move(newSpy));
                    break;
                }

                case PieceType::SpecialPawn:
                {
                    auto newPawn = make_unique<SpecialPawn>(color, pos);
                    newPawn->setHasMoved(hasMoved);
                    other.placePiece(move(newPawn));
                    break;
                }

                default:
                    break;
                }
            }
        }
    }
}

//setup pieces
void Board::setupStandardPosition()
{
    clear();

    placePiece(make_unique<Rook>(Color::Black, Position(0, 0)));
    placePiece(make_unique<Knight>(Color::Black, Position(0, 1)));
    placePiece(make_unique<Bishop>(Color::Black, Position(0, 2)));
    placePiece(make_unique<Queen>(Color::Black, Position(0, 3)));
    placePiece(make_unique<King>(Color::Black, Position(0, 4)));
    placePiece(make_unique<Bishop>(Color::Black, Position(0, 5)));
    placePiece(make_unique<Knight>(Color::Black, Position(0, 6)));
    placePiece(make_unique<Rook>(Color::Black, Position(0, 7)));

    for (int c = 0; c < SIZE; ++c)
    {
        placePiece(make_unique<Pawn>(Color::Black, Position(1, c)));
    }

    for (int c = 0; c < SIZE; ++c)
    {
        placePiece(make_unique<Pawn>(Color::White, Position(6, c)));
    }

    placePiece(make_unique<Rook>(Color::White, Position(7, 0)));
    placePiece(make_unique<Knight>(Color::White, Position(7, 1)));
    placePiece(make_unique<Bishop>(Color::White, Position(7, 2)));
    placePiece(make_unique<Queen>(Color::White, Position(7, 3)));
    placePiece(make_unique<King>(Color::White, Position(7, 4)));
    placePiece(make_unique<Bishop>(Color::White, Position(7, 5)));
    placePiece(make_unique<Knight>(Color::White, Position(7, 6)));
    placePiece(make_unique<Rook>(Color::White, Position(7, 7)));
}

//setup for mission mode
void Board::setupMissionModePosition()
{
    setupStandardPosition();

    Position whiteQueenPos(7, 3);
    removePiece(whiteQueenPos);
    placePiece(make_unique<ArmoredQueen>(Color::White, whiteQueenPos));

    Position blackQueenPos(0, 3);
    removePiece(blackQueenPos);
    placePiece(make_unique<ArmoredQueen>(Color::Black, blackQueenPos));

    Position whiteSpecialPawnPos(6, 4);
    removePiece(whiteSpecialPawnPos);
    placePiece(make_unique<SpecialPawn>(Color::White, whiteSpecialPawnPos));

    Position blackSpecialPawnPos(1, 3);
    removePiece(blackSpecialPawnPos);
    placePiece(make_unique<SpecialPawn>(Color::Black, blackSpecialPawnPos));

    Position whiteJokerPos(6, 2);
    removePiece(whiteJokerPos);
    placePiece(make_unique<Joker>(Color::White, whiteJokerPos));

    Position blackSpyPos(1, 5);
    removePiece(blackSpyPos);
    placePiece(make_unique<Spy>(Color::Black, blackSpyPos));
}

// counter for every piece
int Board::countPieces(Color color) const
{
    int count = 0;
    for (int r = 0; r < SIZE; ++r)
    {
        for (int c = 0; c < SIZE; ++c)
        {
            if (grid[r][c] && grid[r][c]->getColor() == color)
            {
                count++;
            }
        }
    }
    return count;
}

//finding pieces
bool Board::hasPieceAt(const Position &pos, Color color, PieceType type) const
{
    Piece *piece = getPiece(pos);
    if (!piece)
        return false;
    return (piece->getColor() == color && piece->getType() == type);
}

Position Board::findKing(Color color) const
{
    for (int r = 0; r < SIZE; ++r)
    {
        for (int c = 0; c < SIZE; ++c)
        {
            Position pos(r, c);
            Piece *piece = getPiece(pos);
            if (piece && piece->getColor() == color && piece->getType() == PieceType::King)
            {
                return pos;
            }
        }
    }
    return Position(-1, -1);
}
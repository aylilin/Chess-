#include "GameStorage.h"

using namespace std;

void GameStorage::setWatcher(Watcher *w)
{
    watcher = w;
}

// get current time from the system
string GameStorage::currenttime()
{
    auto now = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&time_t_now), "%Y/%m/%d %H:%M:%S");
    return ss.str();
}


// save system for saving the Game
bool GameStorage::SaveGame(const Game &game, const string &Saves)
{
    filesystem::create_directories("saves");

    string fullPath;
    if (Saves.find('/') == string::npos && Saves.find('\\') == string::npos)
    {
        fullPath = "saves/" + Saves;
    }
    else
    {
        fullPath = Saves;
    }

    ofstream file(fullPath);

    if (!file.is_open())
    {
        cout << "Could not open the save file!" << endl;
        return false;
    }

    file << "CHESS++ SAVE FILE" << endl;
    file << "time: " << currenttime() << endl;
    file << "game mode: " << static_cast<int>(game.getGameMode()) << endl;

    if (watcher)
    {
        if (watcher->getWhitePlayer())
        {
            file << "white: " << watcher->getWhitePlayer()->getName() << endl;
        }
        if (watcher->getBlackPlayer())
        {
            file << "black: " << watcher->getBlackPlayer()->getName() << endl;
        }
        if (watcher->getCurrentPlayer())
        {
            file << "current: " << watcher->getCurrentPlayer()->getName() << endl;
        }
    }

    file << "turn count: " << game.getTurnNumber() << endl;
    file << "status: " << static_cast<int>(game.getStatus()) << endl;
    file << endl;

    file << "side: " << static_cast<int>(game.getCurrentTurn()) << endl;

    file << "[BOARD]" << endl;
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            Position pos(r, c);
            Piece *piece = game.getBoard()->getPiece(pos);
            if (piece)
            {
                file << r << " " << c << " " << static_cast<int>(piece->getColor()) << " " << static_cast<int>(piece->getType()) << " " << piece->getHasMoved();

                const ArmoredQueen *aq = dynamic_cast<const ArmoredQueen *>(piece);
                if (aq)
                {
                    file << " armor:" << aq->getArmor();
                }

                const Joker *joker = dynamic_cast<const Joker *>(piece);
                if (joker && joker->isMimicking())
                {
                    file << " mimic:" << static_cast<int>(joker->getMimickedType()) << " mimicked:" << joker->isMimicking();
                }

                const Spy *spy = dynamic_cast<const Spy *>(piece);
                if (spy && spy->isRevealed())
                {
                    file << " revealed:1";
                }

                const Pawn *pawn = dynamic_cast<const Pawn *>(piece);
                if (pawn && pawn->getJustDoubleMoved())
                {
                    file << " doublemoved:1";
                }

                file << endl;
            }
        }
    }
    file << "[END BOARD]" << endl;
    file << endl;

    file << "[MOVE HISTORY]" << endl;
    auto moves = game.getMoveHistoryStrings();
    bool isWhiteTurn = true;

    for (const auto &move : moves)
    {
        if (isWhiteTurn)
        {
            file << watcher->getWhitePlayer()->getName() << ": ";
        }
        else
        {
            file << watcher->getBlackPlayer()->getName() << ": ";
        }
        file << move << endl;

        isWhiteTurn = !isWhiteTurn;
    }

    file << "[END HISTORY]" << endl;
    file << endl;

    file << "saving completed." << endl;
    file.close();

    cout << "Game saved successfully to " << Saves << endl;
    return true;
}

// delete unwanted tabs and spaces
static string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos)
    {
        return "";
    }
    size_t last = str.find_last_not_of(" \t");

    return str.substr(first, last - first + 1);
}

// making pieces for save and load
static unique_ptr<Piece> createPieceFromData(int r, int c, int colorInt, int typeInt, bool hasMoved, const string &extraData)
{
    Color color = static_cast<Color>(colorInt);
    PieceType type = static_cast<PieceType>(typeInt);
    Position pos(r, c);

    if (extraData.find("armor:") != string::npos)
    {
        size_t armorPos = extraData.find("armor:");
        if (armorPos != string::npos)
        {
            int armor = stoi(extraData.substr(armorPos + 6, 1));
            auto queen = make_unique<ArmoredQueen>(color, pos);

            for (int i = 0; i < 2 - armor; i++)
            {
                queen->takeHit();
            }
            if (hasMoved)
            {
                queen->setHasMoved(true);
            }

            return queen;
        }
    }

    if (extraData.find("mimic:") != string::npos)
    {
        auto joker = make_unique<Joker>(color, pos);

        size_t mimicPos = extraData.find("mimic:");
        if (mimicPos != string::npos && mimicPos + 7 < extraData.length())
        {
            int mimicType = stoi(extraData.substr(mimicPos + 6, 1));

            size_t mimickedPos = extraData.find("mimicked:");
            if (mimickedPos != string::npos && mimickedPos + 10 < extraData.length())
            {
                bool mimicked = (extraData.substr(mimickedPos + 9, 1) == "1");

                if (mimicked && mimicType != 0)
                {
                    Board tempBoard;
                    joker->mimicPiece(static_cast<PieceType>(mimicType), tempBoard);
                }
            }
        }

        if (hasMoved)
        {
            joker->setHasMoved(true);
        }
        return joker;
    }

    if (extraData.find("revealed:") != string::npos)
    {
        auto spy = make_unique<Spy>(color, pos);

        size_t revealedPos = extraData.find("revealed:");
        if (revealedPos != string::npos && revealedPos + 9 < extraData.length())
        {
            bool revealed = (extraData.substr(revealedPos + 9, 1) == "1");
            if (revealed)
            {
                spy->reveal();
            }
        }

        if (hasMoved)
        {
            spy->setHasMoved(true);
        }
        return spy;
    }

    if (type == PieceType::Pawn && extraData.find("special:") != string::npos)
    {
        auto specialPawn = make_unique<SpecialPawn>(color, pos);

        if (hasMoved)
        {
            specialPawn->setHasMoved(true);
        }

        return specialPawn;
    }

    unique_ptr<Piece> piece;
    switch (type)
    {
    case PieceType::King:
        piece = make_unique<King>(color, pos);
        break;
    case PieceType::Queen:
        piece = make_unique<Queen>(color, pos);
        break;
    case PieceType::Rook:
        piece = make_unique<Rook>(color, pos);
        break;
    case PieceType::Bishop:
        piece = make_unique<Bishop>(color, pos);
        break;
    case PieceType::Knight:
        piece = make_unique<Knight>(color, pos);
        break;
    case PieceType::Pawn:
        piece = make_unique<Pawn>(color, pos);
        break;
    case PieceType::ArmoredQueen:
        piece = make_unique<ArmoredQueen>(color, pos);
        break;

    case PieceType::Joker:
        piece = make_unique<Joker>(color, pos);
        break;

    case PieceType::Spy:
        piece = make_unique<Spy>(color, pos);
        break;

    case PieceType::SpecialPawn:
        piece = make_unique<SpecialPawn>(color, pos);
        break;
    default:
        return nullptr;
    }

    if (hasMoved)
    {
        piece->setHasMoved(true);
    }
    return piece;
}

// loading method 
unique_ptr<Game> GameStorage::LoadGame(const string &Saves)
{
    string fullPath;
    if (Saves.find('/') == string::npos && Saves.find('\\') == string::npos)
    {
        fullPath = "saves/" + Saves;
    }
    else
    {
        fullPath = Saves;
    }

    ifstream file(fullPath);
    if (!file.is_open())
    {
        cout << "Error: Could not open save file: " << fullPath << endl;
        return nullptr;
    }

    cout << "Loading game from " << Saves << " ..." << endl;

    unique_ptr<Game> game = make_unique<Game>();

    game->getBoard()->clear();
    game->clearHistory();
    game->resetTurnCount();
    game->setStatus(GameStatus::RUNNING);

    string line;
    bool readingBoard = false;
    bool readingHistory = false;
    int expectedGameMode = -1;
    string whitePlayerName = "White";
    string blackPlayerName = "Black";
    string currentPlayerName = "";
    int turnCount = 0;
    int currentSide = -1;

    while (getline(file, line))
    {
        string trimmedLine = trim(line);

        if (trimmedLine.empty())
        {
            continue;
        }

        if (trimmedLine == "[BOARD]")
        {
            readingBoard = true;
            readingHistory = false;
            continue;
        }
        else if (trimmedLine == "[END BOARD]")
        {
            readingBoard = false;
            continue;
        }
        else if (trimmedLine == "[MOVE HISTORY]")
        {
            readingHistory = true;
            readingBoard = false;
            continue;
        }
        else if (trimmedLine == "[END HISTORY]")
        {
            readingHistory = false;
            continue;
        }

        if (!readingBoard && !readingHistory)
        {
            if (trimmedLine.find("game mode:") == 0)
            {
                expectedGameMode = stoi(trimmedLine.substr(10));
                game->setGameMode(static_cast<GameMode>(expectedGameMode));
            }
            else if (trimmedLine.find("white:") == 0)
            {
                whitePlayerName = trim(trimmedLine.substr(6));
            }
            else if (trimmedLine.find("black:") == 0)
            {
                blackPlayerName = trim(trimmedLine.substr(6));
            }
            else if (trimmedLine.find("current:") == 0)
            {
                currentPlayerName = trim(trimmedLine.substr(8));
            }
            else if (trimmedLine.find("turn count:") == 0)
            {
                turnCount = stoi(trimmedLine.substr(11));
            }
            else if (trimmedLine.find("side:") == 0)
            {
                currentSide = stoi(trimmedLine.substr(5));
            }
        }

        if (readingBoard)
        {
            istringstream iss(trimmedLine);
            int r, c, colorInt, typeInt;
            bool hasMoved;
            string extraInfo = "";

            if (iss >> r >> c >> colorInt >> typeInt >> hasMoved)
            {
                string remaining;
                if (getline(iss, remaining))
                    extraInfo = remaining;

                auto piece = createPieceFromData(r, c, colorInt, typeInt, hasMoved, extraInfo);
                if (piece)
                    game->getBoard()->placePiece(move(piece));
            }
        }
    }

    file.close();

    savedWhitePlayer = make_unique<GetPlayer>(whitePlayerName, Color::White);
    savedBlackPlayer = make_unique<GetPlayer>(blackPlayerName, Color::Black);

    game->getWatcher()->setwhitePlayer(savedWhitePlayer.get());
    game->getWatcher()->setBlackPlayer(savedBlackPlayer.get());

    for (int i = 0; i < turnCount * 2; i++)
    {
        game->nextTurn();
    }

    if (currentSide != -1)
    {
        Color currentColor = static_cast<Color>(currentSide);
        if (game->getCurrentTurn() != currentColor)
        {
            game->switchTurn();
        }
    }

    cout << "Game Loaded Successfully" << endl;
    return game;
}

//deleting the save file
bool GameStorage::deleteSave(const string &Saves)
{
    string fullPath;

    if (Saves.find('/') == string::npos && Saves.find('\\') == string::npos)
    {
        fullPath = "saves/" + Saves;
    }
    else
    {
        fullPath = Saves;
    }

    if (remove(fullPath.c_str()) != 0)
    {
        cout << "Could not delete the save file: " << fullPath << endl;
        return false;
    }

    cout << "Save file deleted successfully: " << fullPath << endl;
    return true;
}

vector<string> GameStorage::listSaves(const string &directory)
{
    vector<string> saves;
    cout << "Available saves in " << directory << ":" << endl;

    try
    {
        for (const auto &entry : filesystem::directory_iterator(directory))
        {
            if (entry.is_regular_file())
            {
                string filename = entry.path().filename().string();
                saves.push_back(filename);
                cout << "- " << filename << " (" << entry.file_size() << " bytes)" << endl;
            }
        }
    }
    catch (const exception &e)
    {
        cout << "Error reading directory: " << e.what() << endl;
    }

    if (saves.empty())
    {
        cout << "No save files found." << endl;
    }

    return saves;
}
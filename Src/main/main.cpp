#include "Game.h"
#include "GameStorage.h"
#include "EventManager.h"

using namespace std;

// helper to apply event effects to the game
void applyEventToGame(EventManager &eventManager, Game &game, int currentTurn, const Move &lastMove)
{
    eventManager.checkAndTriggerEvent(currentTurn, *game.getBoard(), game.getCurrentTurn());
    eventManager.applyEventEffects(*game.getBoard(), game.getCurrentTurn());

    if (lastMove.isValid())
    {
        eventManager.updateEvents(currentTurn, *game.getBoard(), lastMove);
    }
}

int main()
{
    string whiteName, blackName;
    int modeChoice;
    string gameChoice;

    cout << endl;
    cout << "[CHESS GAME]" << endl;

    while (true)
    {
        cout << "Do you want to load a saved game or start a new game? (load/start): ";
        getline(cin >> ws, gameChoice);

        if (gameChoice == "load" || gameChoice == "start")
        {
            break;
        }
        else
        {
            cout << "Invalid choice! Please enter 'load' or 'start': ";
        }
    }

    if (gameChoice == "load")
    {
        if (filesystem::exists("Saves.txt"))
        {
            cout << "Loading saved game..." << endl;
            GameStorage storage;
            auto loadedGame = storage.LoadGame("Saves.txt");

            if (loadedGame)
            {
                unique_ptr<Game> game = std::move(loadedGame);
                storage.setWatcher(game->getWatcher());

                cout << "Game loaded successfully!" << endl;
                game->getBoard()->print();

                return 0;
            }
            else
            {
                cout << "Failed to load game! Starting new game instead." << endl;
                gameChoice = "start";
            }
        }
        else
        {
            cout << "No saved game found! Starting new game instead." << endl;
            gameChoice = "start";
        }
    }

    if (gameChoice == "start")
    {
        cout << "Starting new game..." << endl;

        cout << "Select Game Mode:" << endl;
        cout << "1- Standard" << endl;
        cout << "2- Energy" << endl;
        cout << "3- Mission" << endl;

        while (true)
        {
            cin >> modeChoice;
            if (modeChoice >= 1 && modeChoice <= 3)
            {
                break;
            }
            else
            {
                cout << "Invalid choice! Please enter 1, 2, or 3: ";
            }
        }

        cout << "Enter White player name: ";
        getline(cin >> ws, whiteName);
        cout << "Enter Black player name: ";
        getline(cin >> ws, blackName);

        unique_ptr<Game> game = make_unique<Game>();
        GameStorage storage;
        storage.setWatcher(game->getWatcher());

        // create event manager
        EventManager eventManager;
        int turnCount = 0;
        Move lastMove(Position(-1, -1), Position(-1, -1));

        const string SAVE_FILE = "Saves.txt";

        switch (modeChoice)
        {
        case 2:
            game->setGameMode(GameMode::Energy);
            break;
        case 3:
            game->setGameMode(GameMode::Mission);
            break;
        default:
            game->setGameMode(GameMode::Standard);
            break;
        }

        GetPlayer white(whiteName, Color::White);
        GetPlayer black(blackName, Color::Black);
        game->getWatcher()->setwhitePlayer(&white);
        game->getWatcher()->setBlackPlayer(&black);

        game->start();
        game->getBoard()->print();

        cout << endl;
        cout << "Initial save..." << endl;
        storage.SaveGame(*game, SAVE_FILE);

        bool gameRunning = true;

        while (gameRunning)
        {
            if (game->isOver())
            {
                gameRunning = false;
                cout << "GAME OVER!" << endl;

                cout << endl;
                cout << "Final save..." << endl;
                storage.SaveGame(*game, SAVE_FILE);
                break;
            }

            cout << endl;
            cout << game->getCurrentPlayerName() << " turn" << endl;

            // show active event
            if (eventManager.hasActiveEvent())
            {
                cout << "[EVENT] " << eventManager.getCurrentEventDescription() << endl;

                // show more info based on event type
                string eventDesc = eventManager.getCurrentEventDescription();
                if (eventDesc.find("Season") != string::npos)
                {
                    cout << ">> " << eventManager.getSeasonEvent()->getSeasonName() << endl;
                }
                else if (eventDesc.find("Mission") != string::npos)
                {
                    cout << ">> White: " << eventManager.getMissionManager()->getMissionDescription(Color::White) << endl;
                    cout << ">> Black: " << eventManager.getMissionManager()->getMissionDescription(Color::Black) << endl;
                }
                else if (eventDesc.find("Time Bomb") != string::npos)
                {
                    cout << ">> Pieces will explode if not moved for 5 turns!" << endl;
                }
                else if (eventDesc.find("Massacre") != string::npos)
                {
                    cout << ">> Capture queen to get double move!" << endl;
                }
            }

            if (game->getGameMode() == GameMode::Energy)
            {
                cout << "[Energy Mode Active]" << endl;
            }
            else if (game->getGameMode() == GameMode::Mission)
            {
                cout << "[Mission Mode Active]" << endl;
            }

            cout << "Enter command: (help for commands, save to save game, load to load game)" << endl;

            string input;
            getline(cin >> ws, input);

            if (input == "exit")
            {
                gameRunning = false;
                cout << "Game ended." << endl;

                cout << endl;
                cout << "Saving before exit..." << endl;
                storage.SaveGame(*game, SAVE_FILE);
                break;
            }
            else if (input == "help")
            {
                cout << endl;
                cout << "[HELP]" << endl;
                cout << "Move format: [from] [to] Example: e2 e4" << endl;
                cout << "Columns: a-h, Rows: 1-8" << endl;
                cout << "Special commands:" << endl;
                cout << "suggest [position] - Show moves for a piece Example: (suggest e2)" << endl;
                cout << "suggest all - Show every possible moves" << endl;
                cout << "joker - Activate Joker mimic ability" << endl;
                cout << "spy - Reveal spy (if you have one)" << endl;
                cout << "save - Save current game to Saves.txt" << endl;
                cout << "load - Load game from Saves.txt" << endl;
                cout << "event - Show current active event" << endl;
                cout << "exit - Exit the game" << endl;

                if (game->getGameMode() == GameMode::Energy)
                {
                    cout << endl;
                    cout << "[Energy Mode Tips]" << endl;
                    cout << "- Pieces explode after 5 moves without moving!" << endl;
                    cout << "- Register your pieces by moving them!" << endl;
                }
                else if (game->getGameMode() == GameMode::Mission)
                {
                    cout << endl;
                    cout << "[Mission Mode Tips]" << endl;
                    cout << "- Complete your mission to win!" << endl;
                    cout << "- Seasons change every 10 turns!" << endl;
                }

                cout << endl;
            }
            else if (input == "event")
            {
                if (eventManager.hasActiveEvent())
                {
                    cout << "Active Event: " << eventManager.getCurrentEventDescription() << endl;

                    if (eventManager.getCurrentEventDescription().find("Season") != string::npos)
                    {
                        cout << "Current Season: " << eventManager.getSeasonEvent()->getSeasonName() << endl;
                    }
                    else if (eventManager.getCurrentEventDescription().find("Mission") != string::npos)
                    {
                        cout << "White Mission: " << eventManager.getMissionManager()->getMissionDescription(Color::White) << endl;
                        cout << "Black Mission: " << eventManager.getMissionManager()->getMissionDescription(Color::Black) << endl;
                    }
                    else if (eventManager.getCurrentEventDescription().find("Time Bomb") != string::npos)
                    {
                        cout << "Pieces explode if they don't move for 5 turns!" << endl;
                    }
                    else if (eventManager.getCurrentEventDescription().find("Massacre") != string::npos)
                    {
                        cout << "Capture queen to get double move!" << endl;
                    }
                }
                else
                {
                    cout << "No active event." << endl;
                }
            }
            else if (input == "save")
            {
                if (storage.SaveGame(*game, SAVE_FILE))
                {
                    cout << "Game saved successfully to " << SAVE_FILE << endl;
                }
            }
            else if (input == "load")
            {
                auto loadedGame = storage.LoadGame(SAVE_FILE);

                if (loadedGame)
                {
                    game = std::move(loadedGame);
                    storage.setWatcher(game->getWatcher());
                    cout << "Game loaded successfully from " << SAVE_FILE << endl;
                    game->getBoard()->print();

                    // reset event manager after loading
                    turnCount = 0;
                    lastMove = Move(Position(-1, -1), Position(-1, -1));
                }
                else
                {
                    cout << "Failed to load game!" << endl;
                }
            }
            else if (input == "joker")
            {
                bool foundJoker = false;
                Color currentColor = game->getCurrentTurn();

                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        Position pos(r, c);
                        Piece *piece = game->getBoard()->getPiece(pos);
                        if (piece && piece->getColor() == currentColor)
                        {
                            Joker *joker = dynamic_cast<Joker *>(piece);
                            if (joker)
                            {
                                foundJoker = true;
                                cout << endl;
                                cout << "JOKER MENU" << endl;
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
                                cin.ignore();

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
                                    continue;
                                }

                                if (joker->mimicPiece(type, *game->getBoard()))
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

                                    storage.SaveGame(*game, SAVE_FILE);
                                }
                                break;
                            }
                        }
                    }
                    if (foundJoker)
                    {
                        break;
                    }
                }

                if (!foundJoker)
                {
                    cout << "You don't have a Joker piece!" << endl;
                }
            }
            else if (input == "spy")
            {
                bool foundSpy = false;
                Color currentColor = game->getCurrentTurn();

                for (int r = 0; r < 8; r++)
                {
                    for (int c = 0; c < 8; c++)
                    {
                        Position pos(r, c);
                        Piece *piece = game->getBoard()->getPiece(pos);
                        if (piece && piece->getColor() == currentColor)
                        {
                            Spy *spy = dynamic_cast<Spy *>(piece);
                            if (spy && !spy->isRevealed())
                            {
                                foundSpy = true;
                                cout << endl;
                                cout << "SPY MENU" << endl;
                                cout << "Do you want to reveal the spy now?" << endl;
                                cout << "1. Yes (reveal and gain queen abilities)" << endl;
                                cout << "2. No (stay disguised)" << endl;
                                cout << "Choice: ";

                                int choice;
                                cin >> choice;
                                cin.ignore();

                                if (choice == 1)
                                {
                                    spy->reveal();
                                    storage.SaveGame(*game, SAVE_FILE);
                                }
                                else
                                {
                                    cout << "Spy remains disguised." << endl;
                                }
                                break;
                            }
                        }
                    }
                    if (foundSpy)
                    {
                        break;
                    }
                }

                if (!foundSpy)
                {
                    cout << "You don't have a disguised Spy!" << endl;
                }
            }
            else if (input.substr(0, 7) == "suggest")
            {
                if (input.length() >= 8)
                {
                    string posStr = input.substr(8);
                    if (posStr.length() >= 2)
                    {
                        char file = posStr[0];
                        char rankChar = posStr[1];

                        if (file >= 'a' && file <= 'h' && rankChar >= '1' && rankChar <= '8')
                        {
                            int col = file - 'a';
                            int row = 8 - (rankChar - '0');
                            game->showSuggestedMoves(Position(row, col));
                        }
                        else
                        {
                            cout << "Invalid position! Use format: suggest e2" << endl;
                        }
                    }
                }
            }
            else if (input == "all")
            {
                game->showAllSuggestedMoves();
            }
            else
            {
                istringstream iss(input);
                char fromFile, toFile;
                int fromRank, toRank;

                if (iss >> fromFile >> fromRank >> toFile >> toRank)
                {
                    if (fromFile >= 'a' && fromFile <= 'h' && toFile >= 'a' && toFile <= 'h' && fromRank >= 1 && fromRank <= 8 && toRank >= 1 && toRank <= 8)
                    {
                        int fromCol = fromFile - 'a';
                        int fromRow = 8 - fromRank;
                        int toCol = toFile - 'a';
                        int toRow = 8 - toRank;

                        Move currentMove(Position(fromRow, fromCol), Position(toRow, toCol));

                        // save position before move for TimeBomb
                        Position fromPos = Position(fromRow, fromCol);

                        bool moveSuccessful = game->makeMove(currentMove);

                        if (moveSuccessful)
                        {
                            cout << "Move successful!" << endl;

                            // increase turn count
                            turnCount++;

                            // update TimeBomb that piece moved
                            if (eventManager.hasActiveEvent() &&
                                eventManager.getCurrentEventDescription().find("Time Bomb") != string::npos)
                            {
                                eventManager.getTimeBomb()->pieceMoved(fromPos, Position(toRow, toCol));
                            }

                            // update SeasonEvent that piece moved
                            if (eventManager.hasActiveEvent())
                            {
                                Piece *movedPiece = game->getBoard()->getPiece(Position(toRow, toCol));
                                if (movedPiece)
                                {
                                    eventManager.getSeasonEvent()->registerPieceMove(movedPiece, turnCount);
                                }
                            }

                            // save last move
                            lastMove = currentMove;

                            // check for random event
                            applyEventToGame(eventManager, *game, turnCount, lastMove);

                            game->getBoard()->print();

                            storage.SaveGame(*game, SAVE_FILE);
                            cout << "Game auto-saved to " << SAVE_FILE << endl;

                            if (game->isOver())
                            {
                                gameRunning = false;
                                cout << "GAME OVER!" << endl;
                            }
                            else
                            {
                                cout << "White in check? " << (game->isCheck(Color::White) ? "Yes" : "No") << endl;
                                cout << "Black in check? " << (game->isCheck(Color::Black) ? "Yes" : "No") << endl;
                            }
                        }
                        else
                        {
                            cout << "Invalid move! Try again." << endl;
                        }
                    }
                    else
                    {
                        cout << "Invalid input! Use format: e2 e4" << endl;
                    }
                }
                else
                {
                    cout << "Invalid format! Use: e2 e4 or type 'help' for commands" << endl;
                }
            }
        }

        return 0;
    }
}
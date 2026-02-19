#include "GetPlayer.h"
#include <limits>

GetPlayer::GetPlayer(string name, Color color) : Player(name, color)
{
}

Move GetPlayer::getMove(const Board &board)
{
    char fromFile, toFile;
    int fromRank, toRank;

    while (true)
    {
        cout << endl;
        cout << getName() << " turn (" << (getColor() == Color::White ? "White" : "Black") << ")" << endl;
        cout << "Enter move (e.g., e2 e4): ";

        if (cin >> fromFile >> fromRank >> toFile >> toRank)
        {
            if (fromFile >= 'a' && fromFile <= 'h' &&
                toFile >= 'a' && toFile <= 'h' &&
                fromRank >= 1 && fromRank <= 8 &&
                toRank >= 1 && toRank <= 8)
            {
                break;
            }
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please use format like: e2 e4" << endl;
    }

    int fromCol = fromFile - 'a';
    int fromRow = 8 - fromRank;
    int toCol = toFile - 'a';
    int toRow = 8 - toRank;

    return Move(Position(fromRow, fromCol), Position(toRow, toCol));
}
#include "Player.h"

using namespace std;

Player::Player(string n, Color c) : name(n), color(c), score(0), active(false)
{
}

Player::Player() : name(""), color(Color::None), score(0), active(false)
{
}

void Player::setName(string x)
{
    name = x;
}

string Player::getName() const
{
    return name;
}

void Player::setColor(Color c)
{
    color = c;
}

Color Player::getColor() const
{
    return color;
}

void Player::addScore(int z)
{
    score += z;
}

int Player::getScore() const
{
    return score;
}

void Player::setActive(bool t)
{
    active = t;
}

bool Player::isActive() const
{
    return active;
}

Player::~Player()
{
}
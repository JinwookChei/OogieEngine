#include "stdafx.h"
#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	CleanUp();
}

void Player::Tick(double deltaTime)
{
	Actor::Tick(deltaTime);
}

void Player::BeginPlay()
{
}

void Player::Render(bool isFirst)
{
	Actor::Render(isFirst);
}

void Player::CleanUp()
{
}

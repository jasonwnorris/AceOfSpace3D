// Boss.cpp

// AOS Includes
#include "Boss.hpp"
#include "Sound.hpp"

Boss Boss::FinalBoss;

Boss::Boss()
{
  boss = nullptr;
  spawned = false;
  killed = false;
}

void Boss::RemoveBoss()
{
  FinalBoss.boss = nullptr;
  FinalBoss.spawned = false;
  FinalBoss.killed = false;
}

void Boss::SpawnBoss()
{
  FinalBoss.boss = new BossEnemy("Boss");
  FinalBoss.spawned = true;
  Sound::PlaySound("Boss");
}

void Boss::DestroyBoss()
{
  FinalBoss.boss = nullptr;
  FinalBoss.killed = true;
}

// BossEnemy.cpp //

#include "Bullet.hpp"
#include "Player.hpp"

BossEnemy::BossEnemy(const std::string& keyname) : Enemy(keyname)
{
  pointValue = BossPointValue;

  position = Vector(ScreenWidth / 2, -sprite->origin.Y);
  direction = Vector::Down;
  speed = BossSpeed;
  health = BossHealth;

  childrenAngle = 0.0f;
  fireAngle = 0.0f;
  lastFired = 0.0f;

  leftHand = new Miniboss("LeftHand");
  rightHand = new Miniboss("RightHand");
}

// basic movement behavior, comes part way down screen then patrols left-right
void BossEnemy::Update(float deltaTime)
{
  Enemy::Update(deltaTime);

  UpdateChildren();

  if (direction.X < 0)
  {
    if (position.X < 300)
    {
      direction = Vector::Right;
    }
  }
  else if (direction.X > 0)
  {
    if (position.X > ScreenWidth - 300)
    {
      direction = Vector::Left;
    }
  }
  else
  {
    if (position.Y > ScreenHeight / 3)
    {
      if (rand() % 2 == 0)
      {
        direction = Vector::Right;
      }
      else
      {
        direction = Vector::Left;
      }
    }
  }

  childrenAngle += BossChildrenRotation;

  // increase challenge when half dead (or half alive?)
  if (health > BossHealth / 2)
  {
    fireAngle += BossFireRotation;
    lastFired += deltaTime;
  }
  else
  {
    fireAngle += BossFireRotation * 2;
    lastFired += deltaTime * 2;
  }

  if (lastFired > BossFireDelay)
  {
    FireBullet();
  }
}

// helper function to update objects that this parent controls
void BossEnemy::UpdateChildren()
{
  if (leftHand != nullptr)
  {
    if (leftHand->health > 0)
    {
      leftHand->position = position + Vector::Left * 200.0f + Vector::Down * 75.0f + Vector::CalculateDirection(-childrenAngle) * 25.0f;
    }
    else
    {
      leftHand = nullptr;
    }
  }

  if (rightHand != nullptr)
  {
    if (rightHand->health > 0)
    {
      rightHand->position = position + Vector::Right * 200.0f + Vector::Down * 75.0f + Vector::CalculateDirection(childrenAngle) * -25.0f;
    }
    else
    {
      rightHand = nullptr;
    }
  }
}

void BossEnemy::RemoveOffScreen()
{
  SDL_Rect screenBounds = {-ScreenWidth, -ScreenHeight, ScreenWidth * 3, ScreenHeight * 3};
  SDL_Rect objectBounds = GetBounds();

  if (SDL_HasIntersection(&screenBounds, &objectBounds) == SDL_FALSE)
  {
    Remove();
  }
}

void BossEnemy::Remove()
{
  Boss::DestroyBoss();

  Enemy::Remove();
}

// fire bullets in a rotating spiral
// it looks neat!
void BossEnemy::FireBullet()
{
  Bullet* bullet = new Bullet("NeonOrb");
  bullet->position = position;
  bullet->direction = Vector::CalculateDirection(fireAngle);
  bullet->speed = 100.0f;
  bullet->CollisionList = &PlayerShip::PlayerShipList;

  lastFired = 0.0f;
}

// Boss.cpp

// AOS Includes
#include "Boss.hpp"
#include "Sound.hpp"

Boss Boss::FinalBoss;

Boss::Boss()
{
  m_Boss = nullptr;
  m_IsSpawned = false;
  m_IsKilled = false;
}

void Boss::RemoveBoss()
{
  FinalBoss.m_Boss = nullptr;
  FinalBoss.m_IsSpawned = false;
  FinalBoss.m_IsKilled = false;
}

void Boss::SpawnBoss()
{
  FinalBoss.m_Boss = new BossEnemy("Boss");
  FinalBoss.m_IsSpawned = true;
  Sound::PlaySound("Boss");
}

void Boss::DestroyBoss()
{
  FinalBoss.m_Boss = nullptr;
  FinalBoss.m_IsKilled = true;
}

// BossEnemy.cpp //

#include "Bullet.hpp"
#include "Player.hpp"

BossEnemy::BossEnemy(const std::string& keyname) : Enemy(keyname)
{
  m_PointValue = BossPointValue;

  m_Position = Vector2f(ScreenWidth / 2, -m_Sprite->m_Origin.Y);
  m_Direction = Vector2f::Down;
  m_Speed = BossSpeed;
  m_Health = BossHealth;

  m_ChildrenAngle = 0.0f;
  m_FireAngle = 0.0f;
  m_LastFired = 0.0f;

  m_LeftHand = new Miniboss("LeftHand");
  m_RightHand = new Miniboss("RightHand");
}

// basic movement behavior, comes part way down screen then patrols left-right
void BossEnemy::Update(float deltaTime)
{
  Enemy::Update(deltaTime);

  UpdateChildren();

  if (m_Direction.X < 0)
  {
    if (m_Position.X < 300)
    {
      m_Direction = Vector2f::Right;
    }
  }
  else if (m_Direction.X > 0)
  {
    if (m_Position.X > ScreenWidth - 300)
    {
      m_Direction = Vector2f::Left;
    }
  }
  else
  {
    if (m_Position.Y > ScreenHeight / 3)
    {
      if (rand() % 2 == 0)
      {
        m_Direction = Vector2f::Right;
      }
      else
      {
        m_Direction = Vector2f::Left;
      }
    }
  }

  m_ChildrenAngle += BossChildrenRotation;

  // increase challenge when half dead (or half alive?)
  if (m_Health > BossHealth / 2)
  {
    m_FireAngle += BossFireRotation;
    m_LastFired += deltaTime;
  }
  else
  {
    m_FireAngle += BossFireRotation * 2;
    m_LastFired += deltaTime * 2;
  }

  if (m_LastFired > BossFireDelay)
  {
    FireBullet();
  }
}

// helper function to update objects that this parent controls
void BossEnemy::UpdateChildren()
{
  if (m_LeftHand != nullptr)
  {
    if (m_LeftHand->m_Health > 0)
    {
      m_LeftHand->m_Position = m_Position + Vector2f::Left * 200.0f + Vector2f::Down * 75.0f + Vector2f::CalculateDirection(-m_ChildrenAngle) * 25.0f;
    }
    else
    {
      m_LeftHand = nullptr;
    }
  }

  if (m_RightHand != nullptr)
  {
    if (m_RightHand->m_Health > 0)
    {
      m_RightHand->m_Position = m_Position + Vector2f::Right * 200.0f + Vector2f::Down * 75.0f + Vector2f::CalculateDirection(m_ChildrenAngle) * -25.0f;
    }
    else
    {
      m_RightHand = nullptr;
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
  bullet->m_Position = m_Position;
  bullet->m_Direction = Vector2f::CalculateDirection(m_FireAngle);
  bullet->m_Speed = 100.0f;
  bullet->CollisionList = &PlayerShip::PlayerShipList;

  m_LastFired = 0.0f;
}

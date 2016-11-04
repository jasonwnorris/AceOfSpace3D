// Player.cpp

// STL Includes
#include <sstream>
// AOS Includes
#include "Player.hpp"
#include "Sound.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Missile.hpp"
#include "Fireball.hpp"

#define PLAYER_COUNT 2

SDL_Keycode StartKey[2] = { SDLK_SPACE, SDLK_RSHIFT };
SDL_Keycode UpKey[2]    = { SDLK_w, SDLK_UP };
SDL_Keycode DownKey[2]  = { SDLK_s, SDLK_DOWN };
SDL_Keycode LeftKey[2]  = { SDLK_a, SDLK_LEFT };
SDL_Keycode RightKey[2] = { SDLK_d, SDLK_RIGHT };
SDL_Keycode FireKey[2]  = { SDLK_j, SDLK_RCTRL };

Player* Player::Players = nullptr;

Player::Player()
{
  m_Ship = nullptr;
  m_Lives = PlayerLives;
  m_Score = 0;
}

void Player::AddPlayers()
{
  Players = new Player[PLAYER_COUNT];

  for (int i = 0; i < PLAYER_COUNT; ++i)
  {
    Players[i] = Player();
  }
}

void Player::RemovePlayers()
{
  delete[] Players;
  Players = nullptr;
}

void Player::SpawnPlayer(int p_Index)
{
  if (Players[p_Index].m_Ship == nullptr && Players[p_Index].m_Lives > 0)
  {
    std::stringstream ss;
    ss << "Player" << (p_Index + 1);
    Players[p_Index].m_Ship = new PlayerShip(ss.str());
  }
}

void Player::DestroyPlayer(PlayerShip* p_Ship)
{
  for (int i = 0; i < PLAYER_COUNT; ++i)
  {
    if (Players[i].m_Ship == p_Ship)
    {
      Players[i].m_Ship = nullptr;
      --Players[i].m_Lives;
    }
  }
}

void Player::ProcessInput(SDL_Event& p_Event)
{
  for (int i = 0; i < PLAYER_COUNT; ++i)
  {
    Players[i].ProcessInput(p_Event, i);
  }
}

void Player::ProcessInput(SDL_Event& p_Event, int p_Index)
{
  if (m_Ship == nullptr)
  {
    if (p_Event.type == SDL_KEYDOWN)
    {
      if (p_Event.key.keysym.sym == StartKey[p_Index])
      {
        SpawnPlayer(p_Index);
      }
    }
  }
  else
  {
    if (p_Event.type == SDL_KEYDOWN)
    {
      if (p_Event.key.keysym.sym == UpKey[p_Index])
      {
        m_Ship->m_IsMovingUp = true;
      }
      if (p_Event.key.keysym.sym == DownKey[p_Index])
      {
        m_Ship->m_IsMovingDown = true;
      }
      if (p_Event.key.keysym.sym == LeftKey[p_Index])
      {
        m_Ship->m_IsMovingLeft = true;
      }
      if (p_Event.key.keysym.sym == RightKey[p_Index])
      {
        m_Ship->m_IsMovingRight = true;
      }
      if (p_Event.key.keysym.sym == FireKey[p_Index])
      {
        m_Ship->m_IsShooting = true;
      }
    }
    else if (p_Event.type == SDL_KEYUP)
    {
      if (p_Event.key.keysym.sym == UpKey[p_Index])
      {
        m_Ship->m_IsMovingUp = false;
      }
      if (p_Event.key.keysym.sym == DownKey[p_Index])
      {
        m_Ship->m_IsMovingDown = false;
      }
      if (p_Event.key.keysym.sym == LeftKey[p_Index])
      {
        m_Ship->m_IsMovingLeft = false;
      }
      if (p_Event.key.keysym.sym == RightKey[p_Index])
      {
        m_Ship->m_IsMovingRight = false;
      }
      if (p_Event.key.keysym.sym == FireKey[p_Index])
      {
        m_Ship->m_IsShooting = false;
      }
    }
  }
}

void Player::AwardPoints(int p_Amount)
{
  for (int i = 0; i < PLAYER_COUNT; ++i)
  {
    Players[i].m_Score += p_Amount;
  }
}

// PlayerShip.cpp //

std::vector<GameObject*> PlayerShip::PlayerShipList;

PlayerShip::PlayerShip(const std::string& p_Keyname) : GameObject(p_Keyname)
{
  m_Position = Vector2f(ScreenWidth / 2.0f, static_cast<float>(ScreenHeight - m_Sprite->m_Texture->m_TileHeight - PlayerSpawnOffset));
  m_Speed = PlayerSpeed;
  m_Health = PlayerHealth;
  m_ExplosionKeyname = "Explosion";
  m_WeaponType = WEAPON_BULLET;
  CollisionList = &Enemy::EnemyList;

  m_IsMovingUp = false;
  m_IsMovingDown = false;
  m_IsMovingLeft = false;
  m_IsMovingRight = false;
  m_IsShooting = false;
  m_LastFired = 0.0f;

  PlayerShipList.push_back(this);
}

void PlayerShip::Update(float p_DeltaTime)
{
  m_Direction = Vector2f::Zero;

  if (m_IsMovingUp)
  {
    m_Direction.Y -= 1;
  }
  if (m_IsMovingDown)
  {
    m_Direction.Y += 1;
  }
  if (m_IsMovingLeft)
  {
    m_Direction.X -= 1;
  }
  if (m_IsMovingRight)
  {
    m_Direction.X += 1;
  }

  m_LastFired += p_DeltaTime;
  FireBullet();

  GameObject::Update(p_DeltaTime);

  // Horizontally constraint sprite to screen.
  if (m_Position.X < m_Sprite->m_Origin.X)
  {
    m_Position.X = m_Sprite->m_Origin.X;
  }
  else if (m_Position.X > ScreenWidth - m_Sprite->m_Origin.X)
  {
    m_Position.X = ScreenWidth - m_Sprite->m_Origin.X;
  }

  // Vertically constraint sprite to screen.
  if (m_Position.Y < m_Sprite->m_Origin.Y)
  {
    m_Position.Y = m_Sprite->m_Origin.Y;
  }
  else if (m_Position.Y > ScreenHeight - m_Sprite->m_Origin.Y)
  {
    m_Position.Y = ScreenHeight - m_Sprite->m_Origin.Y;
  }
}

void PlayerShip::RemoveKilled()
{
  for (std::vector<GameObject*>::iterator Iter = PlayerShipList.begin(); Iter != PlayerShipList.end(); Iter += 0)
  {
    if ((*Iter)->m_IsDead)
    {
      Iter = PlayerShipList.erase(Iter);
    }
    else
    {
      ++Iter;
    }
  }
}

void PlayerShip::RemoveAll()
{
  for (std::vector<GameObject*>::iterator Iter = PlayerShipList.begin(); Iter != PlayerShipList.end(); Iter += 0)
  {
    Iter = PlayerShipList.erase(Iter);
  }
}

void PlayerShip::Remove()
{
  Player::DestroyPlayer(this);

  GameObject::Remove();
}

void PlayerShip::Collide(GameObject* p_Object)
{
  int objectHealth = p_Object->m_Health;
  p_Object->TakeDamage(m_Health);
  this->TakeDamage(objectHealth);
}

void PlayerShip::FireBullet()
{
  if (m_IsShooting)
  {
    if (m_WeaponType == WEAPON_BULLET)
    {
      if (m_LastFired > PlayerFireDelay)
      {
        for (int i = 0; i < 3; ++i)
        {
          Projectile* projectile = new Bullet("Bullet");
          projectile->m_Position = m_Position + Vector2f::Left * (1 - i) * 5.0f + Vector2f::Up * 20.0f;
          projectile->m_Direction = Vector2f::Up;
          projectile->CollisionList = &Enemy::EnemyList;
        }

        m_LastFired = 0.0f;
      }
    }
    else if (m_WeaponType == WEAPON_MISSILE || m_WeaponType == WEAPON_PLASMA)
    {
      if (m_LastFired > PlayerFireDelay)
      {
        int total = 5;
        for (int i = 0; i < total; ++i)
        {
          Projectile* projectile = nullptr;

          if (m_WeaponType == WEAPON_MISSILE)
          {
            projectile = new Missile("Missile");
          }
          else if (m_WeaponType == WEAPON_PLASMA)
          {
            projectile = new Bullet("Plasma");
          }

          projectile->m_Position = m_Position + Vector2f::Up * 20.0f;
          projectile->m_Direction = Vector2f::Right * (i - (total / 2)) / 10.0f + Vector2f::Up;
          projectile->CollisionList = &Enemy::EnemyList;
        }

        m_LastFired = 0.0f;
      }
    }
    else if (m_WeaponType == WEAPON_FIREBALL)
    {
      if (m_LastFired > FireballFireDelay)
      {
        Projectile* projectile = new Fireball("Flames");
        projectile->m_Position = m_Position + Vector2f::Up * 28.0f;
        projectile->m_Direction = Vector2f::Right * (rand() % 101 - 50) / 100.0f + Vector2f::Up;
        projectile->CollisionList = &Enemy::EnemyList;

        m_LastFired = 0.0f;
      }
    }
  }
}

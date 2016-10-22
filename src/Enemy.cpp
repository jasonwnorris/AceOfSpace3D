// Enemy.cpp //
#include "Enemy.h"

#include "Player.h"
#include "Powerup.h"

vector<GameObject*> Enemy::EnemyList;

Enemy::Enemy(string keyname) : GameObject(keyname)
{
    direction = Vector(0, 1);
  explosionKeyname = "Explosion";

  EnemyList.push_back(this);
}

void Enemy::RemoveKilled()
{
    for(vector<GameObject*>::iterator Iter = EnemyList.begin(); Iter != EnemyList.end(); Iter += 0)
        if((*Iter)->dead)
        Iter = EnemyList.erase(Iter);
    else
        Iter++;
}

void Enemy::RemoveAll()
{
    for(vector<GameObject*>::iterator Iter = EnemyList.begin(); Iter != EnemyList.end(); Iter += 0)
    Iter = EnemyList.erase(Iter);
}

void Enemy::Explode()
{
  GameObject::Explode();
  
  Player::AwardPoints(pointValue);
  DropItem();
}

void Enemy::DropItem()
{
  // 20% drop chance
  if(rand() % 5 == 0)
  {
    int dropType = rand() % 5;
    Item* item;
    
    if(dropType == 0)
        item = new Powerup("ItemHealth");
    else if(dropType == 1)
        item = new Powerup("ItemSpeed");
    else if(dropType == 2)
      item = new Powerup("ItemMissiles");
    else if(dropType == 3)
      item = new Powerup("ItemFireball");
    else if(dropType == 4)
      item = new Powerup("ItemPlasma");
      
    item->position = position;
  }
}

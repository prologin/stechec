/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `COPYING' file in the root directory.
**
** Copyright (C) 2005, 2006 Prologin
*/


#include "ServerResolver.hh"

void         ServerResolver::ApplyResolver(CommandListRef cmdList[])
{
  CommandListRef::iterator it;

  // bouge tt les coleos avec leur liste.
  MoveColeoptera(cmdList[MOVE_COLEOPTERA]);

  // prend les msg les uns apres les autres
  for (int i = 0; i < LAST_MSG; i++)
    if (i != MOVE_COLEOPTERA)
      for (it = cmdList[i].begin();
           it != cmdList[i].end(); ++it)
        switch (i)
          {
          case DOCK_TO_COLEOPTERA:
            Dock_to_coleoptera(*it);
            break;

          case DOCK_TO_BUILDING:
            Dock_to_building(*it);
            break;

          case TRANSFERT:
            Transfert(*it);
            break;

          case MAKE_BUILDING:
            MakeBuilding(*it);
            break;

          case MAKE_COLEOPTERA:
            MakeColeoptera(*it);
            break;

          case THROW_PLASMA:
            ThrowPlasma(*it);
            break;

          case COLEOPTERA_CHANGE_STATE:
            ColeopteraChangeState(*it);
            break;

          case REPAIR_COLEOPTERA:
            RepairColeoptera(*it);
            break;

          case REPAIR_COLEOPTERA_BY_FACTORY:
            RepairColeopteraByFactory(*it);
            break;
          }
}

void                ServerResolver::MoveColeoptera(CommandListRef& cmdList)
{
  const StechecPkt* elt;
  bool changed = true;

  while (changed)
    {
      changed = false;
      for (CommandListRef::iterator it = cmdList.begin();
           it != cmdList.end(); ++it)
        {
          elt = *it;
          if (elt == NULL)
            continue;

          Player* p = &g_->players[elt->client_id];
          int id = elt->arg[0];
          int x = elt->arg[1];
          int y = elt->arg[2];
          Coleoptere* col = &p->coleopteres[id];

          // case not empty
          if (g_->terrain_coleoptere[x][y] != NULL)
            continue;
          changed = true;

          g_->terrain_coleoptere[x][y] = col;
          g_->terrain_coleoptere[col->get_x()][col->get_y()] = NULL;
          col->set_x(x);
          col->set_y(y);
          SendToAll(*elt);

          *it = NULL;
        }
    }
}

void                ServerResolver::Dock_to_coleoptera(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int                id1 = elt->arg[0];
  int                id2 = elt->arg[1];
  Coleoptere*        col1 = &p->coleopteres[id1];
  Coleoptere*        col2 = &p->coleopteres[id2];

  col1->state = STATE_DOCKING;
  col1->dock_col = col2;
  col2->state = STATE_DOCKING;
  col2->dock_col = col1;
  SendToAll(*elt);
}

void                ServerResolver::Dock_to_building(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int                id = elt->arg[0];
  Coleoptere*        col = &p->coleopteres[id];

  col->state = STATE_DOCKING;
  col->dock_build = &(*g_->terrain_building[col->get_x()][col->get_y()]);
  LOG2("Docked to building %1", col->dock_build->type);
  SendToAll(*elt);
}

void                ServerResolver::Transfert(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int           id = elt->arg[0];
  int                ore = elt->arg[1];
  int                plasma = elt->arg[2];
  Coleoptere*        col = &p->coleopteres[id];

  if (col->dock_col != 0)
    {
      if (plasma > 20)
        {
          // pouf, c'est tout.
          s_->ChangeState(col->dock_col, STATE_DEAD);
          Explosion(col->dock_col->get_x(), col->dock_col->get_y());
        }
      else
        {
          col->transfert_ore = ore;
          if (plasma > 0)
            {
              s_->UpdatePlasma(col, col->stock_plasma - plasma);
              s_->UpdatePlasma(col->dock_col, col->dock_col->stock_plasma + plasma);
            }
        }
    }
  else /* transfert to a factory */
    {
      col->transfert_ore = ore;
      if (plasma > 0)
        {
          Factory* fact = static_cast<Factory*>(col->dock_build);
          s_->UpdatePlasma(col, col->stock_plasma - plasma);
          s_->UpdatePlasma(fact, fact->stock_plasma + plasma);
        }
    }

  // pas de transfert de minerai -> desarime.
  if ((col->dock_col != NULL && plasma <= 20 || col->dock_build != NULL) && ore == 0)
    s_->ChangeState(col, STATE_NORMAL);
}

void                ServerResolver::Explosion(int x, int y)
{
  int                i;
  int                j;
  Coleoptere        *col;

  for (i = -2; i <= 2; i++)
    for (j = -2; j <= 2; j++)
      if (in_map(x + i, y + j) && (col = g_->terrain_coleoptere[x + i][y + j]))
        {
          if (abs(i) + abs(j) == 1)
            col->add_damage(EXPLOSION_CASE_1);
          if (abs(i) + abs(j) == 2)
            col->add_damage(EXPLOSION_CASE_2);
          if (col->state == STATE_DEAD)
            {
              s_->ChangeState(col, STATE_DEAD);
              LOG2("coleo explosed: id %1, player %2", col->get_id(), col->get_player());
              Explosion(i, j);
            }
        }
}

// open or close canal from api
void                ServerResolver::ColeopteraChangeState(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int                id = elt->arg[0];
  Coleoptere*        col = &p->coleopteres[id];

  col->state = elt->arg[1];
  SendToAll(*elt);
}

void                ServerResolver::ThrowPlasma(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int           id = elt->arg[0];
  Coleoptere*        col = &p->coleopteres[id];
  int                quant = elt->arg[1];
  int                dirx = elt->arg[2];
  int                diry = elt->arg[3];
  int                tmpx = col->get_x() + dirx;
  int                tmpy = col->get_y() + diry;

  // update les ressources en plasma
  s_->UpdatePlasma(col, col->stock_plasma - quant);

  // pour la visu uniquement
  SendToAll(*elt);

  // lance le tir
  while (in_map(tmpx, tmpy) && g_->terrain_coleoptere[tmpx][tmpy] == NULL)
    {
      tmpx += dirx;
      tmpy += diry;
    }
  if (!in_map(tmpx, tmpy))
    return;

  // on a touche une cible
  col = g_->terrain_coleoptere[tmpx][tmpy];
  if (col->state == STATE_CANAL_OPEN)
    {
      if (quant > 20 || col->stock_plasma + quant > 100)
        {
          // t'es mort
          s_->ChangeState(col, STATE_DEAD);
          Explosion(tmpx, tmpy);
        }
      else
        {
          // bonne reception
          s_->UpdatePlasma(col, col->stock_plasma + quant);
        }
    }
   else
    {
      col->add_damage(5 * quant);
      if (col->state == STATE_DEAD)
        {
          // t'es mort.
          s_->ChangeState(col, STATE_DEAD);
          Explosion(tmpx, tmpy);
        }
    }
}

void                ServerResolver::MakeBuilding(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int           id = elt->arg[0];
  int           type = elt->arg[1];
  Coleoptere*        col = &p->coleopteres[id];
  Building*        b;

  switch (type)
    {
    case HOTEL:
      if (p->hotel_count >= MAX_HOTEL)
        return;
      b = &p->hotels[p->hotel_count];
      b->set_id(p->hotel_count++);
      break;
    case MINE:
      if (p->mine_count >= MAX_MINE)
        return;
      b = &p->mines[p->mine_count];
      b->set_id(p->mine_count++);
      break;
    case CENTRAL:
      if (p->central_count >= MAX_CENTRAL)
        return;
      b = &p->centrals[p->central_count];
      b->set_id(p->central_count++);
      break;
    case FACTORY:
      if (p->factory_count >= MAX_FACTORY)
        return;
      b = &p->factories[p->factory_count];
      b->set_id(p->factory_count++);
      break;
    }

  col->build_time = MAKE_BUILDING_TIME + 1;
  s_->ChangeState(col, STATE_BUILDING);
  s_->UpdatePlasma(col, col->stock_plasma - PLASMA_BUILDING);
  s_->UpdateOre(col, col->stock_ore - ORE_BUILDING);

  b->type = type;
  b->state = b_in_construction;
  b->set_x(col->get_x());
  b->set_y(col->get_y());
  b->set_player(col->get_player());
  col->dock_build = b;
  SendToAll(*elt);
}

void                ServerResolver::MakeColeoptera(const StechecPkt* elt)
{
  Player*        p = &g_->players[elt->client_id];
  int           id = elt->arg[0];
  int                count = elt->arg[1];
  Factory*        f = &p->factories[id];

  s_->UpdateOre(f, f->stock_ore - count * ORE_COLEOPTERA);
  s_->UpdatePlasma(f, f->stock_plasma - count * PLASMA_COLEOPTERA);

  f->buildlist += count * MAKE_COLEOPTERA_TIME;

  SendToAll(*elt);
}

void                ServerResolver::RepairColeoptera(StechecPkt* elt)
{
  int id1 = elt->arg[0]; // reparateur
  int id2 = elt->arg[1]; // appareil en train d'etre repare

  Coleoptere* col1 = &g_->p->coleopteres[id1];
  Coleoptere* col2 = &g_->p->coleopteres[id2];

  s_->UpdateOre(col1, col1->stock_ore - 50);
  s_->UpdatePlasma(col1, col1->stock_plasma -= 10);

  // repare l'appareil
  col2->set_damage(col2->get_damage() - 10);
  elt->arg[0] = id2;
  elt->arg[1] = col2->get_damage();
  SendToAll(*elt);
}

void                ServerResolver::RepairColeopteraByFactory(const StechecPkt* elt)
{
  SendToAll(*elt);
}

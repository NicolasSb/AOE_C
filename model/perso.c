#include "perso.h"

/* to use Cmocka :*/
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/**
*
*   \file perso.c
*   \brief Personage management
*
*   \author Sourbier N.
*
*   \version 1.0
*
*   \date 23/02/2017
*
*   \note Test coverage is 99.42%
*   \todo change some functions' scope to static
*/


/** \todo set Struct accessor to change Perso Skills (improvements... ) on another version of this file
*   \brief initialisation of a Perso with respect to his type
*/

Perso* initPerso(int type)
{
    Perso* p = calloc(1, sizeof(Perso));
    if (!p)
        return NULL;
    p->skills.hp=0;
    p->perso_image.x=65;
    p->perso_image.y=200;
    p->perso_image.h=40;
    p->perso_image.w=20;
    p->team = 0;
    p->t.t= NULL;
    p->t.type=TARGET_INVALID;
    p->dist_to_cam = 0;
    switch(type){
    case VILLAGER:
        p->skills.armor = VILLAGER_ARMOR;
        p->skills.attack_range = VILLAGER_ATTACK_RANGE;
        p->skills.damage=VILLAGER_DAMAGE;
        p->skills.range=VILLAGER_RANGE;
        p->type = VILLAGER;
        p->generation_time = 25;
        p->action_flag =  WAIT;
        break;
    case ARCHER:
        p->skills.armor = ARCHER_ARMOR;
        p->skills.attack_range = ARCHER_ATTACK_RANGE;
        p->skills.damage=ARCHER_DAMAGE;
        p->skills.range=ARCHER_RANGE;
        p->type = ARCHER;
        p->generation_time = 25;
        p->action_flag =  WAIT ;
        break;
    case SWORDMAN:
        p->skills.armor = SWORDMAN_ARMOR;
        p->skills.attack_range = SWORDMAN_ATTACK_RANGE;
        p->skills.damage=SWORDMAN_DAMAGE;
        p->skills.range=SWORDMAN_RANGE;
        p->type = SWORDMAN;
        p->generation_time = 25;
        p->action_flag =  WAIT;
        break;
    }
    return p;
}

/**
* \brief free Perso's allocated memory
* \param Perso the Perso to free
*/

void freePerso(Perso* p){
    if (p){
        p->t.t=NULL;
        free(p);
    }
}

/**
*   \brief Perso \a player attacks Perso \a enemy
*   if they are too distant, \a player follows \a enemy until he reachs him
*   \todo comment the conditions
*/

int attack(Perso* player)
{
    int attack_distance, enemy_team_number, player_team_number, tmp_distPlX, tmp_distPlY, tmpDist_enX, tmpDist_enY;
    Perso * enemy;
    if (player != NULL && player->t.t !=NULL){
        enemy = (Perso*) player->t.t;
        enemy_team_number = getTeam(enemy);
        player_team_number = getTeam(player);
        getCoordinates(player, &tmp_distPlX, &tmp_distPlY);
        getCoordinates(enemy, &tmpDist_enX, &tmpDist_enY);
        tmp_distPlX +=player->perso_image.w/2;
        tmp_distPlY +=player->perso_image.h/2;
        tmpDist_enX +=enemy->perso_image.w/2;
        tmpDist_enX +=enemy->perso_image.h/2;
        attack_distance  = computeDistance(tmp_distPlX,tmp_distPlY,tmpDist_enX,tmpDist_enY);
        if(player->skills.attack_range > attack_distance && enemy_team_number != player_team_number){
           if (enemy->skills.hp <=0){
                    setActionFlagP(player, WAIT);
                    return 1;
           }
           else if(enemy->skills.hp >0){
                enemy->skills.hp-=abs((player->skills.damage)-enemy->skills.armor);
                setAttacked(enemy, player);
           }
        }
        else {
            follow_p(player, enemy);
        }

    return 0;
    }
    return -1;
}

/**
* \brief update the attribute \a isAttacked from Perso Player
*
* \param Perso, the Perso we are updating
*/
int setAttacked(Perso* player, Perso* attacker)
{
    if( player !=NULL && attacker != NULL){
        setTarget(player, (void*) attacker, TARGET_PERSO);
        if (player->action_flag == WAIT || player->action_flag == MOVE)
            setActionFlagP(player, ATTACK);
        return 0;
    }
    return -1;
}



/**
*   \brief allow a Perso to move from  to B
*   \param \a p, the Perso to move \a x and \a y new Perso's coordinates
*
*
*   based on a distance calcul and a direction code
*
*   4 1 2
*   6 p 3
*   7 8 5
*   the number choosen between 1 and 8 represents the direction of our Perso
*   this code is computed by chooseMoveCode()
*
*   \todo my Perso should not step back
*/
int move_p(Perso* p, int new_x, int new_y)
{
    if (p!= NULL){
        if (p->perso_image.x!=new_x || p->perso_image.y != new_y){
            int moveCode = chooseMoveCode(p, new_x, new_y);
            switch (moveCode)
            {
            case 1 :
                setCoord(p, p->perso_image.x, --(p->perso_image.y));
                break;
            case 2 :
                setCoord(p,++(p->perso_image.x), --(p->perso_image.y));
                break;
            case 3 :
                setCoord(p, ++(p->perso_image.x), p->perso_image.y);
                break;
            case 4 :
                setCoord(p,--(p->perso_image.x),--(p->perso_image.y));
                break;
            case 5 :
                setCoord(p,++(p->perso_image.x),++(p->perso_image.y));
                break;
            case 6 :
                setCoord(p,--(p->perso_image.x), p->perso_image.y);
                break;
            case 7 :
                setCoord(p, --(p->perso_image.x), ++(p->perso_image.y));
                break;
            case 8 :
                setCoord(p, p->perso_image.x, ++(p->perso_image.y));
                break;
            }
        }
        else{
            int flag = getActionFlagP(p);
            if (flag == MOVE) {
            setActionFlagP(p, WAIT);
            }
        }
        return 0;
    }

    return -1;

}

/**
*   \brief compute the code direction of a Perso
*
*   \todo add a condition to know if he can walk here or not
*/
int chooseMoveCode(Perso* p, int new_x, int new_y)
{

    double min = 5000;
    double tmp_min;
    int code = -1;
    if ( (tmp_min = computeDistance((p->perso_image.x),(p->perso_image.y)-1, new_x, new_y)) < min){
        min = tmp_min;
        code = 1;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)+1,(p->perso_image.y)-1, new_x, new_y)) < min){
        min = tmp_min;
        code = 2;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)+1,(p->perso_image.y), new_x, new_y)) < min){
        min = tmp_min;
        code = 3;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)-1,(p->perso_image.y)-1, new_x, new_y)) < min){
        min = tmp_min;
        code = 4;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)+1,(p->perso_image.y)+1, new_x, new_y)) < min){
        min = tmp_min;
        code = 5;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)-1,(p->perso_image.y), new_x, new_y)) < min){
        min = tmp_min;
        code = 6;
    }
    if ((tmp_min = computeDistance((p->perso_image.x)-1,(p->perso_image.y)+1, new_x, new_y)) < min){
        min = tmp_min;
        code = 7;
    }
    if ((tmp_min = computeDistance((p->perso_image.x),(p->perso_image.y)+1, new_x, new_y)) < min){
        min = tmp_min;
        code = 8;
    }
    return code;
}
/**
*   \brief update Perso's coordinates
*/

void setCoord(Perso* p, int x1, int y1)
{
    if (p!= NULL){
        p->perso_image.x = x1;
        p->perso_image.y = y1;
    }
}
/**
*   \brief compute the distance between two points
*
*
*/

double computeDistance(int x1, int y1, int x2, int y2)
{
    int x = x2-x1;
    int y = y2-y1;
    return sqrt(pow(x,2)+pow(y,2));
}

/**
*   \brief allow a Perso to follow an other
*
*
*
*   \alert not tested ! => should work
*/
int follow_p(Perso* player, Perso* enemy)
{
    if(player!=NULL && enemy != NULL){
        int x_enemy;
        int y_enemy;
        getCoordinates(enemy, &x_enemy, &y_enemy);
        move_p(player, x_enemy, y_enemy);
        return 0;
    }
    return -1;
}


/**
*   \brief  get the current Perso's coordinates
*
*
*/
void getCoordinates(Perso* p, int * x , int* y)
 {
     if(p !=NULL){
        *x = p->perso_image.x;
        *y = p->perso_image.y;
     }
     else {
        *x=0;
        *y=0;
     }
 }

/**
*   \brief used to get the team number of a personage
*   \ param \a p the personage we want to know the team number
*/
int getTeam(Perso *p)
{
    if (!p)
        return -1;
    return p->team;
}

/**
*   \brief to set the team number of a personage
*   \param \a p, the personage to set the team number \a team_number
*
*/
void setTeam(Perso *p, int team_number)
{
    if (p !=NULL)
        p->team = team_number;
}

/**
*   \brief to update the target of a personage
*   \param \a p the personage to set the new \a target
*/
void setTarget(Perso *p, void* target, int target_type)
{
    if (p!=NULL){
        p->t.t = target;
        p->t.type=target_type;
    }
}

/**
*   \brief to update the action flag of a personage
*   \param \a p the personage to set the new \a action_flag
*/
void setActionFlagP(Perso *p, int action_flag)
{
    if (p!=NULL){
        p->action_flag = action_flag;
    }
}

/**
*   \brief to access the action flag of a personage
*   \param \a p the personage
*   \return the action flag of a personage
*/
int getActionFlagP(Perso *p)
{
    if (p!= NULL)
        return p->action_flag;
    return -1;
}

/**
*   \brief to set a SDL_rect to a personage
*/
void setRect(Perso *p, SDL_Rect r)
{
    if (p)
        p->perso_image =r;
}

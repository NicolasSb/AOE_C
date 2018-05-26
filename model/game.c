#include "game.h"

/**
*   \file game.c to initialize, free, save and load a game
*
*   \author Nicolas Sourbier
*   \date   2/04/2017
*/


/*----------------------------------------------------------
*        IMPORTANT : THE REAL PLAYER TEAM NUMBER IS 0
*              OTHERS ARE UPDATED BY THE AI
*---------------------------------------------------------*/


/**
*   \brief to initialize a game
*   \param nbPlayers the number of players in the game
*/

Game *initGame(int nbPlayers)
{
    int teamNb = 0,i;
    GameMap *m = NULL;
    Animals *ani;
    Building *b;
    Game *g = calloc(1, sizeof(Game));
    g->nbPlayer = nbPlayers;
    if (!g)
        return NULL;
    m = loadFromTextFile(m, "media/huge.map");

    g->b=initList(LIST_BUILDING);
    g->p=initList(LIST_PERSO);
    g->r=initList(LIST_RESOURCE);
    g->a=initList(LIST_ANIMALS);

    /*random animal created here to run animal functions */
    ani  = initBoar();
    change_coordinates (ani, -300, 600);
    ani->r.h = 30;
    ani->r.w = 30;
    insert_first(g->a, (void*)ani, teamNb);

    ani  = initSheep();
    change_coordinates (ani, 250, 200);
    ani->r.h = 30;
    ani->r.w = 30;
    insert_first(g->a, (void*)ani, teamNb);

    ani  = initWolf();
    change_coordinates (ani, -400, 350);
    ani->r.h = 30;
    ani->r.w = 30;
    insert_first(g->a, (void*)ani, teamNb);

    ani  = initStag();
    change_coordinates (ani, 600, 550);
    ani->r.h = 30;
    ani->r.w = 30;
    insert_first(g->a, (void*)ani, teamNb);

    for (teamNb=0; teamNb<nbPlayers; teamNb++)
    {
        g->pl[teamNb]=initPlayer(teamNb);
        for (i=0; i<5; i++)
        {
            Perso *p = initPerso(VILLAGER);
            p->skills.hp = VILLAGER_HP;
            p->team = teamNb;
            setCoord(p, 65+10*i+500*teamNb, 130+3*i+1000*teamNb);
            insert_first(g->p, (void*) p, teamNb);
        }
        b = init_building(CITY_HALL);
        b->hp = CITY_HALL_HP;
        b->building_r.x+=450*teamNb;
        b->building_r.y+=950*teamNb;
        g->pl[teamNb]->main_buildingX = b->building_r.x;
        g->pl[teamNb]->main_buildingY = b->building_r.y;
        b->team=teamNb;
        insert_first(g->b, (void*)b, teamNb);
    }
    g->m=m;
    return g;
}


/**
*   \brief to free the game
*/
void freeGame(Game *g)
{
    int i;
    if (g)
    {
        for (i=0; i<g->nbPlayer+1; i++)
        {
            if (g->pl[i])
                freePlayer(g->pl[i]);
        }
        if (g->m)
            freeMap(g->m);
        freeList(g->b);
        freeList(g->p);
        freeList(g->r);
        freeList(g->a);
        free(g);
    }
}



/**
*   \brief allow an animal to act as in the real aoe game
*
*/


int action_animal(Game *g, Animals *ani)
{
    static int count=0;
    int res = HumanDetected_by_animal(ani,g);
    if (res==0 || (ani->type == SHEEP || ani->type ==STAG))
    {
        ani->target = NULL;
        count+=1;
        count = count%400;
        if (count<350)
        {
            ani->direction = rand()%8;
        }
        else
        {
            switch (ani->direction)
            {
            case 0 :
                move_left_above (ani);
                break;
            case 1 :
                move_left_bellow (ani);
                break;
            case 2 :
                move_right_above (ani);
                break;
            case 3 :
                move_right_bellow (ani);
                break;
            case 4 :
                move_left (ani);
                break;
            case 5 :
                move_right (ani);
                break;
            case 6 :
                move_above (ani);
                break;
            case 7 :
                move_bellow (ani);
                break;
            }
        }
        return 0;
    }
    else if (res == 1 && (ani->type == WOLF || ani->type ==BOAR))
    {
        AnimalATT_perso(g,ani);
        return 0;
    }
    return -1;
}

static int action_IA_Perso(Game *g, Perso *p, int action)
{
    int target_type;
    static int nbCall;
    Player *pl = g->pl[p->team];
    Building *b = NULL;
    static int build=0, type_build = -1;
    if (p->team != 0)
    {
        if(nbCall%5000 == 0)
        {
            build=0;
            type_build=0;
            nbCall=0;
        }
        switch (action)
        {
        case CONSTRUCT:
            constructB(p);
            if(p->action_flag == MOVE)
            {
                build = 0;
                type_build=-1;
            }
            break;
        case BUILD:
            pl->selectP[0]=p;
            if (build == 1)
            {
                chooseValidCoordinate(pl, p);
                build=2;
            }
            b = buildB(pl, type_build);
            if (b)
                insert_first(g->b, (void*) b, pl->team_number);

            break;
        case MOVE:
            moveP(p, p->dest_x, p->dest_y);
            break;
        case ATTACK:
            target_type = targetType(p);
            switch (target_type)
            {
            case TARGET_PERSO:
                attackCharacter(g, p);
                break;
            case TARGET_BUILDING:
                attackBuilding(g, p);
                break;
            case TARGET_ANIMALS:
                attackAnimals(g, p);
                break;
            default:
                break;
            }
            break;
        case COLLECT:
            if (pl->nbCharacters < ((pl->nbHouses)*4+MAX_PERSO) || build !=0)
            {
                collectResources_general(g, pl, p);
            }
            else if (build == 0)
            {
                if (pl->nbHouses >= 2 && pl->nbBarracks==0)
                {
                    setActionFlagP(p, BUILD);
                    type_build=BARRACKS;
                }
                else
                {
                    setActionFlagP(p, BUILD);
                    type_build=HOUSE;
                }

                build=1;
                nbCall=0;
                break;
            }
            break;
        case WAIT:

            if (pl->nbCharacters == pl->nbHouses*4 + MAX_PERSO && build ==0)
            {
                if (pl->nbHouses >= 2 && pl->nbBarracks==0)
                {
                    setActionFlagP(p, BUILD);
                    type_build=BARRACKS;
                }
                else
                {
                    setActionFlagP(p, BUILD);
                    type_build=HOUSE;
                }

                build=1;
                nbCall=0;
                break;
            }

            else if (pl->lumbermen < 10 || pl->goldFinder < 10 || pl->picker<10 || pl->stoneCutter < 10)
            {
                autoCollectResources(g, pl, p);
                break;
            }
        default:
            if (getActionFlagP(p)== DO_NOTHING && p->skills.hp == VILLAGER_HP)
                setActionFlagP(p, WAIT);
            break;
        }
        nbCall ++;
        return 0;
    }
    return -1;
}

static int action_IA_Building (Game *g, Building *b, int action)
{
    static int nbCall, nbSoldier, type;
    Player *pl = g->pl[b->team];
    Perso *p =NULL;
    if (b->team !=0)
    {
        switch (action)
        {
        case CONSTRUCT_P:
            constructP(b);
            break;
        case SLEEP:
            switch(b->type)
            {
            case HOUSE:
                break;
            case BARRACKS:
                if(nbCall%200==0 && type == SWORDMAN)
                {
                    p = createPerso(b, pl, SWORDMAN);
                    insert_first(g->p, (void*)p, pl->team_number);
                    nbCall=0;
                    nbSoldier++;
                    if (nbSoldier%5 == 0)
                        type =ARCHER;
                }
                else
                {
                    p = createPerso(b, pl, ARCHER);
                    insert_first(g->p, (void*)p, pl->team_number);
                    nbCall=0;
                    nbSoldier++;
                    if (nbSoldier%5 == 0)
                        type =SWORDMAN;
                }
                nbSoldier++;
                break;
            case CITY_HALL:
                if (pl->nbVillagers<40 && nbCall%150==0)
                {
                    p = createPerso(b, pl, VILLAGER);
                    insert_first(g->p, (void*)p, pl->team_number);
                    nbCall=0;
                }
                nbCall ++;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        return 0;
    }
    return -1;
}

/**
*   \brief make a personage act depending on his action flag.
*
*   \note the half part of it is not working now
*
*   \todo complete the functions required to increase his action possibilities
*/


int action_perso(Game *g, Perso *p)
{
    if(p)
    {
        int action = getActionFlagP(p);
        int target_type ;
        if (p->team == 0)
        {
            switch (action)
            {
            case CONSTRUCT:
                constructB(p);
                break;
            case MOVE:
                moveP(p, p->dest_x, p->dest_y);
                break;
            case ATTACK:
                target_type = targetType(p);
                switch (target_type)
                {
                case TARGET_PERSO:
                    attackCharacter(g, p);
                    break;
                case TARGET_BUILDING:
                    attackBuilding(g, p);
                    break;
                case TARGET_ANIMALS:
                    attackAnimals(g, p);
                    break;
                default:
                    break;
                }

                break;
            case COLLECT:
                collectResources_general(g,g->pl[0],p);
                break;
            default:
                break;
            }
            return 0;;
        }
        return action_IA_Perso(g, p, action);
    }
    return -1;
}

/**
*   \brief make a Building act depending on his action flag.
*
*   \todo complete the functions required to increase his action possibilities
*/
int action_Building(Game *g, Building *b)
{
    if(b)
    {
        int action = getActionFlag(b);
        if (b->team ==0)
        {
            switch (action)
            {
            case CONSTRUCT_P:
                constructP(b);
                break;
            default:
                break;
            }
            return 0;
        }
        return action_IA_Building(g, b, action);
    }
    return -1;
}






/**
*   \brief to check if an enemy was clicked on or not
*
*/
void * enemyDetected(Game *g, int mouse_x, int mouse_y, int * type)
{
    int x, y;
    if (g)
    {
        Element *current = g->p->first;
        do
        {
            if(current)
            {
                if (current->pointer)
                {
                    Perso *p= (Perso*) current->pointer;
                    getCoordinates(p, &x, &y);
                    if (x<mouse_x+10 && x+p->perso_image.w>mouse_x+10 &&
                            y<mouse_y+35 && y+p->perso_image.h>mouse_y+35)
                    {
                        *type = TARGET_PERSO;
                        return p;
                    }
                }
                current = current->next;
            }
        }
        while(current);


        current = g->b->first;

        do
        {
            if(current)
            {
                if (current->pointer)
                {
                    Building *b= (Building*) current->pointer;
                    x = b->building_r.x;
                    y = b->building_r.y;
                    if (x<mouse_x && x+b->building_r.w>mouse_x &&
                            y<mouse_y+50 && y+b->building_r.h>mouse_y+50)
                    {
                        *type = TARGET_BUILDING;
                        return b;
                    }
                }
                current=current->next;
            }
        }
        while(current);



        current = g->a->first;

        do
        {
            if(current)
            {
                if (current->pointer)
                {
                    Animals *ani= (Animals*) current->pointer;
                    get_coordo_animals(ani, &x, &y);
                    if (x<mouse_x+10 && x+ani->r.w>mouse_x-10 &&
                            y<mouse_y+10 && y+ani->r.h>mouse_y-10)
                    {
                        *type = TARGET_ANIMALS;
                        return ani;
                    }
                }
                current = current->next;
            }
        }
        while(current);
    }
    return NULL;
}

void *resourceFound(Game *g, int mouse_x, int mouse_y, int *type)
{
    int x, y;
    if (g)
    {
        Element *current = g->r->first;
        do
        {
            if(current)
            {
                if (current->pointer)
                {
                    Resource *r = (Resource*) current->pointer;
                    x = r->r.x;
                    y = r->r.y;
                    if (x<mouse_x+10 && x+r->r.w>mouse_x+10 &&
                            y<mouse_y+35 && y+r->r.h>mouse_y+35)
                    {
                        *type = TARGET_RESSOURCE;
                        return r;
                    }
                }
                current = current->next;
            }
        }
        while(current);
    }
    return NULL;
}
/**
*   \brief allows an animal to detect a character
*
*/

int HumanDetected_by_animal(Animals * ani, Game *g)
{
    int x, y;
    if (g)
    {
        Element *current = g->p->first;
        do
        {
            if(current)
            {
                if (current->pointer)
                {
                    Perso *p= (Perso*) current->pointer;
                    getCoordinates(p, &x, &y);
                    if (computeDistance(x, y, ani->r.x + ani->r.w/2, ani->r.y + ani->r.h/2) < (ani->view_range))
                    {
                        if (ani->target == NULL)
                        {
                            ani->target = (void*)p;
                            return 1;
                        }
                        return 1;
                    }
                }
                current = current->next;
            }
        }
        while(current);
    }
    return 0;
}

/**
*   \brief to decrease the number of Characters when a player destruct an enemy
*/
static void decreaseCharactersNb(Game *g, int team, int type)
{
    if (g->pl[team])
    {
        switch (type)
        {
        case VILLAGER:
            g->pl[team]->nbVillagers -= 1;
            g->pl[team]->nbCharacters -=1;
            break;
        default:
            g->pl[team]->nbCharacters -=1;
            break;
        }
    }
}



/**
*   \brief to make a character attack an opponent
*/

void attackCharacter(Game *g, Perso *p)
{
    Element *current = NULL;
    Perso *p2 = NULL;
    void * pointer = NULL;
    Perso * toDel= NULL;
    if (p->t.t)
    {
        if(attack(p)==1)
        {
            pointer = (void*)p->t.t;
            toDel = (Perso*)p->t.t;
            decreaseCharactersNb(g, toDel->team, toDel->type);
            deleteBySearch(g->p, pointer);
            current = g->p->first;
            do
            {
                if(current)
                {
                    p2 = (Perso *) current->pointer;
                    if(p2 && p2->team != toDel->team)
                    {
                        if(p2->t.t == pointer)
                        {
                            p2->t.t=NULL;
                            p2->t.type=TARGET_INVALID;
                        }
                    }
                    current=current->next;
                }
            }
            while(current);
        }
    }
}

/**
*   \brief allow a character to attack an animal
*/

void attackAnimals(Game *g, Perso *p)
{
    void * pointer = NULL;
    Element *current = NULL;
    Perso *p2 = NULL;
    if (p->t.t)
    {
        if(attack_A(p)==1)
        {
            pointer = (void*)p->t.t;
            deleteBySearch(g->a, pointer);
            current = g->p->first;
            do
            {
                if(current)
                {
                    p2 = (Perso *) current->pointer;
                    if(p2)
                    {
                        if(p2->t.t == pointer)
                        {
                            p2->t.t=NULL;
                            p2->t.type=TARGET_INVALID;
                            setActionFlagP(p, WAIT);
                        }
                    }
                    current=current->next;
                }
            }
            while(current);
        }
    }
}

/**
*   \brief allow an animal to attack a character
*/


void AnimalATT_perso(Game *g, Animals * ani)
{
    void * pointer = NULL;
    Element *current = NULL;
    Perso *p2 = NULL;
    if (ani->target)
    {
        if(animal_ATT(ani)==1)
        {
            pointer = (void*)ani->target;
            deleteBySearch(g->p, pointer);
            ani->target = NULL;
            current = g->p->first;
            do
            {
                if (current)
                {
                    p2 = (Perso *) current->pointer;
                    if(p2)
                    {
                        if(p2->t.t == pointer)
                        {
                            p2->t.t=NULL;
                            p2->t.type=TARGET_INVALID;
                        }
                    }
                    current=current->next;
                }
            }
            while(current);
        }
    }
}

/**
*   \brief to make a character collect a resource
*/

void collectResources_general(Game *g, Player *pl, Perso *p)
{
    void * pointer = NULL;
    Resource *r = NULL;
    Element *current = NULL;
    Perso *p2 = NULL;
    int collection=0, type, count =0;
    if (p->t.t)
    {
        r = (Resource *)p->t.t ;
        collection = collectResources(p, r);
        if (collection!=0 && collection!=-1)
        {
            pl->available_resources[r->type]+=collection;
        }
        if(collection==-1)
        {
            pointer = (void*)p->t.t;
            type = r->type;
            deleteBySearch(g->r, pointer);
            current = g->p->first;
            do
            {
                if (current)
                {
                    p2 = (Perso *) current->pointer;
                    if(p2)
                    {
                        if(p2->t.t == pointer)
                        {
                            setTarget(p2, NULL, TARGET_INVALID);
                            setActionFlagP(p2, WAIT);
                            ++count;
                        }
                    }
                    current = current->next;
                }
            }
            while(current);
            switch (type)
            {
            case WOOD:
                pl->lumbermen-=count;
                break;
            case STONE:
                pl->stoneCutter-=count;
                break;
            case GOLD:
                pl->goldFinder-=count;
                break;
            case FOOD:
                pl->picker-=count;
                break;
            }
        }
    }
}

/**
*   \brief to decrease the number of Building of a type when a player destruct an opponent's building
*/
static void decreaseBuildingNb(Game *g, int team, int type)
{
    if (g->pl[team])
    {
        switch (type)
        {
        case CITY_HALL:
            g->pl[team]->nbCityHall -= 1;
            break;
        case HOUSE:
            g->pl[team]->nbHouses -= 1;
            break;
        case BARRACKS:
            g->pl[team]->nbBarracks -= 1;
            break;

        case TOWER:
            g->pl[team]->nbTower -= 1;
            break;
        }
    }
}

/**
*   \brief to make a character attack a building
*/

void attackBuilding(Game *g, Perso *p)
{
    int type;
    Building *b = NULL;
    void * pointer = NULL;
    Element *current = NULL;
    Perso *p2 = NULL;
    if (p->t.t)
    {
        if(attackB(p)==1)
        {
            pointer = (void*)p->t.t;
            b = (Building *)p->t.t;
            type = b->type;
            decreaseBuildingNb(g, b->team, type);
            deleteBySearch(g->b, pointer);
            current = g->p->first;
            do
            {
                if(current)
                {
                    p2 = (Perso *) current->pointer;
                    if(p2)
                    {
                        if(p2->t.t == pointer)
                        {
                            p2->t.t=NULL;
                            p2->t.type=TARGET_INVALID;
                        }
                    }
                    current=current->next;
                }
            }
            while(current);
        }
    }
}




/**
*	\brief to compute the distance of a building / a resource to the camera
*   \todo OPTIMIZE THAT
*/

void updateDistToCam(Game *g, List *l)
{
    Element *current = NULL;
    if (g && l)
    {
        switch(l->type)
        {
        case LIST_BUILDING:
            current = g->b->first;
            do
            {
                if (current)
                {
                    Building *b = (Building*) current->pointer;
                    b->dist_to_cam = computeDistance(0, b->building_r.y, 0, 640);
                    current = current->next;
                }
            }
            while(current);
            break;
        case LIST_PERSO:
            current = g->p->first;
            do
            {
                if(current)
                {
                    Perso *p = (Perso*) current->pointer;
                    if(p)
                        if (p->action_flag == MOVE)
                            p->dist_to_cam = computeDistance(0, p->perso_image.y, 0, 640);
                    current = current->next;
                }
            }
            while(current);
            break;
        case LIST_RESOURCE:
            current = g->r->first;
            do
            {
                if (current)
                {
                    Resource *r = (Resource*) current->pointer;
                    r->dist_to_cam = computeDistance(0, r->r.y, 0, 640);
                    current = current->next;
                }
            }
            while(current);
            break;
        case LIST_ANIMALS:
            current = g->a->first;
            do
            {
                if(current)
                {
                    Animals *a= (Animals*) current->pointer;
                    if(a)
                        a->dist_to_cam= computeDistance(0, a->r.y, 0, 640);
                    current = current->next;
                }
            }
            while(current);
            break;
        }



    }

}



/**
*   \brief used to select several personages
*
*   \param \a pl, the player that will select the perso at the coordinate \a mouse_x \a mouse_y
*
*   \todo OPTIMIZE THAT CODE !!!!   -by dividing the screen in parts
*/
void select_perso(Game *g, SDL_Rect r)
{
    Element *current = NULL;
    if (g)
    {
        clear_selectP(g->pl[0]);
        current = g->p->first;
        do
        {
            if (current)
            {
                Perso *p= (Perso*) current->pointer;
                if (p)
                {
                    if (p->perso_image.x>r.x && p->perso_image.x<r.x+r.w &&
                            p->perso_image.y>r.y && p->perso_image.y<r.y+r.h)
                        if (current->team==0)
                            addSelectedP(g->pl[0], p);
                }
                current = current->next;
            }
        }
        while(current);
    }
}


/**
*   \brief used to select one personage
*
*   \param \a pl, the player that will select the perso at the coordinate \a mouse_x \a mouse_y
*
*   \todo OPTIMIZE THAT CODE !!!!   -by dividing the screen in parts
*/
void select_onePerso(Game *g, int mouse_x, int mouse_y)
{
    int x, y, w, h;
    Element *current = NULL;
    Perso *p = NULL;
    if (g)
    {
        clear_selectP(g->pl[0]);
        current = g->p->first;
        do
        {
            if (current)
            {
                p= (Perso*) current->pointer;

                getCoordinates(p, &x, &y);
                if(p)
                {
                    w = p->perso_image.w;
                    h = p->perso_image.h;
                    if (x-w/2-5<mouse_x && x+5>mouse_x &&
                            y-h/2-5<mouse_y && y+5>mouse_y)
                    {
                        if (current->team==0)
                            g->pl[0]->selectP[0]=p;
                        break;
                    }
                }
                current = current->next;
            }
        }
        while(current);
    }
}


/**
*   \brief to select a building or a resource.
*
*   \todo same optimization as above
*/
void select_Something(Game *g,int mouse_x,int mouse_y)
{
    int x, y;
    int select=0;
    Element *current = NULL;
    if (g)
    {
        current = g->b->first;
        do
        {
            if (select == 1)
                break;
            if (current)
            {
                Building *b= (Building*) current->pointer;
                if(b)
                {
                    x= b->building_r.x;
                    y= b->building_r.y;
                    if (x<mouse_x+50 && x+b->building_r.w>mouse_x+50 &&
                            y<mouse_y+50 && y+b->building_r.h>mouse_y+50)
                    {
                        g->pl[0]->s.target=b;
                        g->pl[0]->s.target_type=SELECT_BUILDING;
                        select =1;
                        break;
                    }
                }
                current=current->next;
            }
        }
        while(current);


        current = g->r->first;
        do
        {
            if(current)
            {
                if (select == 1)
                    break;
                if (current->pointer)
                {
                    Resource *res= (Resource*) current->pointer;
                    x= res->r.x;
                    y= res->r.y;
                    if (x<mouse_x && x+res->r.w>mouse_x &&
                            y<mouse_y&& y+res->r.h>mouse_y)
                    {
                        g->pl[0]->s.target=res;
                        g->pl[0]->s.target_type=SELECT_RESOURCE;
                        select =1;
                        break;
                    }
                }
                current=current->next;
            }
        }
        while(current);

        current = g->a->first;
        do
        {
            if (select == 1)
                break;
            if (current)
            {
                Animals *ani= (Animals*) current->pointer;
                x= ani->r.x;
                y= ani->r.y;
                if (x<mouse_x && x+ani->r.w>mouse_x &&
                        y<mouse_y && y+ani->r.h>mouse_y)
                {
                    g->pl[0]->s.target=ani;
                    g->pl[0]->s.target_type=SELECT_ANIMALS;
                    select =1;
                    break;
                }
                current=current->next;
            }
        }
        while(current);
    }
}


void initResourceList(Game *g, GameMap *m)
{
    int i;
    List *rscr = initList(LIST_RESOURCE);
    Resource *r = NULL;
    for(i=0; i<m->nb_rsrc; i++)
    {
        r = initResource(m->rsrc[i][0]);
        r->r.x=m->rsrc[i][1];
        r->r.y=m->rsrc[i][2];
        insert_first(rscr, (void*)r, -1);
    }
    g->r = rscr;
}

/*
*----------------------------------------------------------
*            This section is about the AI
*----------------------------------------------------------
*/

static Resource *find_resource(Game *g, Perso*p, int type)
{
    Element *current = g->r->first;
    Resource *r = NULL, *r2 = NULL;
    int x = p->perso_image.x;
    int y = p->perso_image.y;
    int dist =1000000, dist_tmp;
    do
    {
        if(current)
        {
            r = (Resource *) current->pointer;
            if(r->type == type)
            {
                if ((dist_tmp=computeDistance(x, y,r->r.x, r->r.y))< dist)
                {
                    dist = computeDistance(x,y,r->r.x, r->r.y);
                    r2 = r;
                }
            }
            current = current->next;
        }
    }
    while(current);
    return r2;
}

static int min(Player *pl)
{
    const int a=pl->lumbermen, b=pl->stoneCutter, c=pl->picker, d=pl->goldFinder;

    int res = a;
    res = res<b ? res : b;
    res = res<c ? res : c;
    res = res<d ? res : d;

    if (res == a)
        return WOOD;
    else if (res == c)
        return FOOD;
    else if (res == b)
        return STONE;
    else if (res == d)
        return GOLD;
    return -1;
}


void autoCollectResources(Game *g, Player *pl, Perso *p)
{
    int min_collect;
    Resource *r = NULL;
    if (pl && p)
    {
        if(p->action_flag == WAIT)
        {
            min_collect = min(pl);
            r = find_resource(g, p, min_collect);
            if (r)
                setTarget(p, (void *)r, TARGET_RESSOURCE);
            else
                setTarget(p, NULL, TARGET_INVALID);
            switch(min_collect)
            {
            case WOOD:
                pl->lumbermen+=1;
                break;
            case STONE:
                pl->stoneCutter+=1;
                break;
            case GOLD:
                pl->goldFinder+=1;
                break;
            case FOOD:
                pl->picker+=1;
                break;
            }
            setActionFlagP(p, COLLECT);
        }
    }
}

void chooseValidCoordinate(Player *pl, Perso *p)
{
    static int a;
    static int b;
    static int c;
    static int count;
    int tab_a[3]= {-1,0,1};
    int x, y;
    count++;
    x= pl->main_buildingX;
    y= pl->main_buildingY;

    if(tab_a[a%3] == 0 && tab_a[b%3]==0)
        a+=1;
    p->dest_x=(x+50)+tab_a[b]*100 +tab_a[b]*100*c;
    p->dest_y=(y+50)+tab_a[a]*100 +tab_a[a]*100*c;
    a+=1;
    if(count%8==0)
    {
        a=0;
        b=0;
        c+=1;
    }
    if(a%3==0)
    {
        a=0;
        b+=1;
    }
    if(b%3==0)
        b=0;
}

/*
*----------------------------------------------------------
*   This section is about saving a game in a binary file
*----------------------------------------------------------
*/

static void saveBuildingBinary (Building *b, FILE *f)
{
    if (b && f)
    {
        fwrite(&(b->type), 1, sizeof(int), f);
        fwrite(&(b->team), 1, sizeof(int), f);
        fwrite(&(b->hp), 1, sizeof(int),f);
        fwrite(&(b->building_r.h), 1, sizeof(int),f);
        fwrite(&(b->building_r.w), 1, sizeof(int),f);
        fwrite(&(b->building_r.x), 1, sizeof(int),f);
        fwrite(&(b->building_r.y), 1, sizeof(int),f);
    }

}

static void savePersoBinary (Perso *p, FILE *f)
{
    if(p && f)
    {
        fwrite(&(p->type), 1, sizeof(int), f);
        fwrite(&(p->team), 1, sizeof(int), f);
        fwrite(&(p->skills.hp), 1, sizeof(int),f);
        fwrite(&(p->perso_image.h), 1, sizeof(int),f);
        fwrite(&(p->perso_image.w), 1, sizeof(int),f);
        fwrite(&(p->perso_image.x), 1, sizeof(int),f);
        fwrite(&(p->perso_image.y), 1, sizeof(int),f);
    }

}

static void saveResourceBinary(Resource *r, FILE *f)
{
    if (r && f)
    {
        fwrite(&(r->type), 1, sizeof(int), f);
        fwrite(&(r->r.x), 1, sizeof(int),f);
        fwrite(&(r->r.y), 1, sizeof(int),f);
        fwrite(&(r->amount), 1, sizeof(int),f);
        fwrite(&(r->level), 1, sizeof(int),f);
    }

}

static void saveListBinary(List *l, FILE *f)
{
    Element *current = NULL;
    if (l)
    {
        int i;
        fwrite(&(l->nbElts), 1, sizeof(int), f);
        current = l->first;
        switch(l->type)
        {
        case (LIST_BUILDING):
            for (i=0; i<l->nbElts; i++)
            {
                if (current->pointer)
                {
                    saveBuildingBinary((Building*)current->pointer, f);
                    if(current->next)
                        current=current->next;
                    else
                        break;
                }

            }
            break;
        case (LIST_PERSO):
            for (i=0; i<l->nbElts; i++)
            {
                if(current->pointer)
                {
                    savePersoBinary((Perso*)current->pointer, f);
                    if(current->next)
                        current=current->next;
                    else
                        break;
                }

            }
            break;
        case (LIST_RESOURCE):
            for (i=0; i<l->nbElts; i++)
            {
                if (current->pointer)
                {
                    saveResourceBinary((Resource*)current->pointer, f);
                    if(current->next)
                        current=current->next;
                    else
                        break;
                }

            }
        }
    }
}

static void savePlayerDataBinary(Game *g, FILE *f)
{
    int i, j;
    for (j=0; j<g->nbPlayer; j++)
    {
        fwrite(&(g->pl[j]->team_number),1,sizeof(int),f);
        for(i=0; i<NB_RESOURCE_TYPE; i++)
        {
            fwrite(&(g->pl[j]->available_resources[i]), 1, sizeof(int), f);
        }
    }
}

void saveGameBinary(Game *g)
{
    if(g)
    {
        FILE *f = openFile("game.save", "wb+");
        fwrite(&(g->nbPlayer), 1, sizeof(int), f);
        savePlayerDataBinary(g, f);
        saveListBinary(g->p,f);
        saveListBinary(g->b,f);
        saveListBinary(g->r,f);
        closeFile(f);

        printf("sauvegarde effectuee avec succes \n");
    }
}


/*
*-------------------------------------------------------------
*   This section is about loading a game from a binary file
*-------------------------------------------------------------
*/


static Perso *loadPersofromBinaryFile(FILE *f)
{
    int type;
    Perso *p = NULL;
    SDL_Rect r;
    fread(&(type), 1, sizeof(int), f);
    p = initPerso(type);
    fread(&(p->team),1, sizeof(int),f);
    fread(&(p->skills.hp), 1, sizeof(int),f);
    fread(&(r.h), 1, sizeof(int),f);
    fread(&(r.w), 1, sizeof(int),f);
    fread(&(r.x), 1, sizeof(int),f);
    fread(&(r.y), 1, sizeof(int),f);

    p->perso_image = r;
    return p;
}

static Building *loadBuildingfromBinaryFile(FILE *f)
{
    int type;
    Building *b = NULL;
    fread(&(type), 1, sizeof(int), f);
    b = init_building(type);
    fread(&(b->team),1, sizeof(int),f);
    fread(&(b->hp), 1, sizeof(int),f);
    fread(&(b->building_r.h), 1, sizeof(int),f);
    fread(&(b->building_r.w), 1, sizeof(int),f);
    fread(&(b->building_r.x), 1, sizeof(int),f);
    fread(&(b->building_r.y), 1, sizeof(int),f);
    return b;
}

static Resource *loadResourcefromBinaryFile(FILE *f)
{
    Resource *r;
    int type;
    fread(&(type), 1, sizeof(int), f);
    r = initResource(type);
    fread(&(r->r.x), 1, sizeof(int),f);
    fread(&(r->r.y), 1, sizeof(int),f);
    fread(&(r->amount), 1, sizeof(int),f);
    fread(&(r->level), 1, sizeof(int),f);
    return r;
}

static void loadResourceListfromBinaryFile(List *l, FILE *f)
{
    int i, nbElts;
    fread(&(nbElts),1,sizeof(int),f);
    for(i=0; i<nbElts; i++)
    {
        Resource *r = loadResourcefromBinaryFile(f);
        insert_first(l, (void*)r, -1);
    }
}

static void loadPersoListfromBinaryFile(List *l, FILE *f)
{
    int i, nbElts;
    fread(&(nbElts),1,sizeof(int),f);
    for(i=0; i<nbElts; i++)
    {
        Perso *p = loadPersofromBinaryFile(f);
        insert_first(l, (void*)p, p->team);
    }
}

static void loadBuildingListfromBinaryFile(List *l, FILE *f)
{
    int i, nbElts;
    fread(&(nbElts),1,sizeof(int),f);
    for(i=0; i<nbElts; i++)
    {
        Building *b = loadBuildingfromBinaryFile(f);
        insert_first(l, (void*)b, b->team);
    }
}

static Player*loadPlayerFromBinaryFile(FILE *f)
{
    int i=0;
    Player *p = initPlayer(0);
    fread(&(p->team_number),1, sizeof(int), f);
    for (i=0; i<NB_RESOURCE_TYPE; i++)
    {
        fread(&(p->available_resources[i]),1, sizeof(int),f);
    }

    return p;
}

Game *loadGameFromFileBinary()
{
    int i;
    Game *g = initGame(0);
    FILE *f = openFile("game.save", "rb");
    fread(&(g->nbPlayer),1,sizeof(int), f);
    for(i=0; i<g->nbPlayer; i++)
    {
        g->pl[i] = loadPlayerFromBinaryFile(f);
    }
    loadPersoListfromBinaryFile(g->p,f);
    loadBuildingListfromBinaryFile(g->b,f);
    loadResourceListfromBinaryFile(g->r,f);
    closeFile(f);
    return g;
}



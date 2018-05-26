#include "../model/interaction.h"
#include "../model/game.h"


/**

#define LENGTH_SCREEN 1280
#define HEIGHT_SCREEN 720

#define OPTION_POSITION_Y 12
#define BUTTON_POSITION_X 755

#define OFFSET_Y_BUTTON 25
#define OFFSET_X_OPTION 877
#define NUMBER_OF_BUTTONS 4
#define NUMBER_OF_OPTIONS 6

#define LENGTH_SELECT_BUTTON 110
#define HEIGHT_SELECT_BUTTON 40

#define LENGTH_SELECT_OPTION 60
#define HEIGHT_SELECT_OPTION 60

*/
void banner_control(Game *g, Banner *b, int x, int y, int eventType)
{
    int buttonSelect = 0;
    int offsetXOpt=0, offsetYOpt=0, offsetXbut=0, offsetYbut=0;
    if (b)
    {
        offsetXOpt = b->position.x + OFFSET_X_OPTION;
        offsetYOpt = b->position.y + OPTION_POSITION_Y;
        offsetXbut = b->position.x + BUTTON_POSITION_X;
        offsetYbut = b->position.y + OFFSET_Y_BUTTON;
        switch(eventType)
        {
        case EVENT_CLIC:
            if (x>offsetXOpt && y>offsetYOpt)
            {
                b->option_selected = optionHovered(b, x);
                buttonSelect =1;
            }
            if (buttonSelect)
            {
                action_on_banner(g, b->banner_type, b->option_selected);
            }
            break;
        case EVENT_MOVEMENT:
            if (x>offsetXOpt && y>offsetYOpt)
            {
                b->option_selected = optionHovered(b, x);
            }
            else if ((x> offsetXbut) && (y>offsetYbut) && (x < offsetXOpt) )
            {
                b->button_selected = buttonHovered(b, y);
            }
            break;
        default:
            break;
        }
    }
}

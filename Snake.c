#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
                                        // the values for the keys used to play 
#define UP_ARROW 72
#define DOWN_ARROW 80
#define RIGHT_ARROW 77
#define LEFT_ARROW 75
#define ENTER 13
#define ESCAPE 27
                                        // the values for the board limits where the snake moves
#define UPPER_BARRIER 5
#define LOWER_BARRIER 39
#define RIGHT_BARRIER 166
#define LEFT_BARRIER 45
                                        // the values for both the max snake length + the ascii value for the special character used as a barrier  
#define MAX_LENGTH 257
#define BARRIER 178
                                        // the values for the different speeds that the snake can take
#define DEFAULT_SPEED 115
#define SLOW_SPEED 130
#define MEDIUM_SPEED 100
                                        // the values for the colors that the user can pick that are not defined in the windows.h
#define HIGH_SPEED 71
#define LIGHT_BLUE 3
#define LIGHT_PURPLE 5
#define LIGHT_YELLOW 6
#define LIGHT_white 7
#define GREY 8
#define BLUE 9
#define GREEN 10
#define CYAN 11
#define RED 12
#define PURPLE 13
#define YELLOW 14
#define WHITE 15
                                        // this is the struct variable which is the key for making this project work perfectly it contained 2 int variables which
                                        //  are (x,y) these are the coordinates for each block of the snake
typedef struct
{
    int x, y;
} position;
                                        // these are all of the used functions which are used in this programme. head to their definitions for further informations
                                        // about how they work and their role
void setColor(int);
void gotoxy(int, int);
void move(char);
void update_score(position *);
void draw_grid();
void draw_snake();
void draw_apple();
void display_lost_screen();
void set_data();
int change_difficulty();
int change_snake_color();
int change_apple_color_shape();
int update_snake_place();
int settings_menu();
int display_move(int, int, int, int);
position apple_location();
                                        // here we define an array of type 'position' meaning that each element in this array containes their own (x,y) coordinates
                                        // and togther they represent the snake  
position snake[MAX_LENGTH], apple;
                                        // we declare a variable of type time which willbe used to seed and help setting a random location for the apple
time_t t;
                                        // 1st we have the standard i,j variables which are used loops, length is the size of the snake, last_detected move
                                        // takes the value a value depending on the last pressed key which determines the direction of the snake so that it keeps
                                        // heading towards until it takes a different value when a different key is pressed, speed takes the snake speed which will
                                        // be used in Sleep() function, the lower value it takes the faster the snake will head, head_X, head_Y are the coordinates
                                        // of the snake head, tail_X, tail_Y are the coordinates of the last place after the snake tail, they're used to erase the snake 
                                        // track when the tail leaves that place. snake_color is set to GREEN and apple to RED, reset is  set to  2, which indicates that
                                        // the game is running, colors[] containes all of the colors available for snake + apple customization, head_shape[] containes
                                        // the ascii values for 4 different arrows special characters, each pointing to a different direction, apple_shape[] containes
                                        // the ascii values for different special characters shapes which the user can pick as the apple, apple_shape is set to 3 as 
                                        // default which is a heart shape <3 

int i, j;
int length, last_detected_move, speed=DEFAULT_SPEED;
int head_X, head_Y, tail_X, tail_Y;
int snake_color = FOREGROUND_GREEN, apple_color = FOREGROUND_RED;
int reset = 2;
int colors[15] = {FOREGROUND_BLUE, FOREGROUND_GREEN, LIGHT_BLUE, FOREGROUND_RED, LIGHT_PURPLE, LIGHT_YELLOW, LIGHT_white, GREY, BLUE, GREEN, CYAN, RED, PURPLE, YELLOW, WHITE};

char head_shape[4] = {30, 31, 16, 17};
char apple_shapes[15] = {3, 4, 5, 6, 14, 15, 35, 36, 38, 48, 64, 88, 157, 184, 220};
char apple_shape = 3;
                                         
int main()
                                        // we start by opening the console is max size for better gaming experience 
{
    ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
                                        // we seed a random number 
    srand((unsigned)time(&t));
                                        // this is where we store each hey_bent made during the entire game
    char key_bend;
                                        // reset is set to 2 which will initiate the while loop, it breakes for reset=0
    while (reset)
    {
                                        // set=2, hence we set the primary data and variables to default values which starts a new game
        if (reset)
            set_data();
                                        // we start by drawing: the playing grid which is a set of barriers + the snake

        draw_grid();
        draw_snake();
                                        // displa message to the user to indicate that the game have started and wait for a key press to start
        gotoxy(RIGHT_BARRIER + 5, LOWER_BARRIER+4);
        setColor(WHITE);
        printf("PRESS ANY KEY TO START .....");
        getch();
                                        // erase the "PRESS ANY KEY TO START....." message using blankes
        gotoxy(RIGHT_BARRIER + 5, LOWER_BARRIER+4);
        printf("                            ");
        
                                        // this is where all the magic happenes
        while (TRUE)
        {
                                        // we call the apple_location() which returns random (x,y) coordinates for the apple location, then we draw the apple
            apple = apple_location();
            draw_apple();
            while (TRUE)
            {
                                        // we check for any key input which indicates that the user wants to change the snake direction or pause the game 
                if (kbhit())
                {
                                        // if the user pressed 'esc' or 'enter' the game pauses and display an options menu, else it will call the move function 
                                        // which will take the pressed arrow key value and set last_detected_move to a specific value depending on the pressed 
                                        // arrow key, and the snake will keep moving towards that direction until a different arrow key is pressed
                    key_bend = getch();
                    if (key_bend == ESCAPE || key_bend == ENTER)
                        settings_menu();
                    else
                        move(key_bend);
                }
                                        // update_snake_place function returns the value 1 if the snake didn't hit any barrier or hit itself, hence !1=0
                                        // otherwise it returns 0 !0=1, which will break the playing loop
                if (!update_snake_place())
                    break;
                                        // we draw the snake after updating it's location
                draw_snake();
                                        // Sleep() takes snake speed and decide how fast it can go, the lower the better
                Sleep(speed);
                
                                        // if the snake eats the apple we call the update score to increast the score by 1, and break out of this loop
                                        // to draw the apple in a random place again 
                if (head_X == apple.x && head_Y == apple.y)
                {
                    update_score(&apple);
                    break;
                }
            }
                                        // reset value can change during parts of the game, if it stil set to 2, we continue, otherwise, it breakes
                                        // if reset=1, we break from this  loop and reset the score and data and start again, if 0 the game is terminated
            if (reset != 2)
                break;
        }
    }
}
                                        // this fucntin is used to change the text color, it's called with the color number that we want to set
void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
                                        // this is the gotoxy fucntion which takes th (x,y) coordinats for where we want to print our text or snake
}
void gotoxy(int x, int y)
{
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
                                        // the move function takes the code for the arrow key pressed and set last_detected_move to the value which represents the 
                                        // direction that the snake will keep heading towards until it's called with a different key input and change the
                                        // last_detected_move to the new direction
void move(char direction)
{
    switch (direction)
    {
    case UP_ARROW:
        last_detected_move = 0;
        break;
    case DOWN_ARROW:
        last_detected_move = 1;
        break;
    case RIGHT_ARROW:
        last_detected_move = 2;
        break;
    case LEFT_ARROW:
        last_detected_move = 3;
        break;
    default:

        break;
    }
}
                                        // when the snake eats an apple this function is called to increase the snake length by 1 for the apple eaten, and we use a
                                        // pointer to call the apple_location() to get new coordinates for the new apple and give it to the apple variable
void update_score(position *apple)
{
    length++;
    *apple = apple_location();
}
                                        // we simply draw the grid using the special ascii character in the range of barriers
void draw_grid()
{
    setColor(GREY);
    gotoxy(RIGHT_BARRIER + 5, LOWER_BARRIER-1);
    printf("PRESS 'ESC' TO PAUSE");
    setColor(FOREGROUND_RED);
    int x = LEFT_BARRIER - 1, y = UPPER_BARRIER;
    for (i = 0; i < 35; i++)
    {
        gotoxy(x, y + i);
        printf("%c%c", BARRIER, BARRIER);
        for (j = 1; j <= 60; j++)
        {
            if (i == 0 || i == 34)
                printf("%c%c", BARRIER, BARRIER);
            else
                printf("  ");
        }
        printf("%c%c\n", BARRIER, BARRIER);
    }
}
                                        // after giving the snake it's new coordinates, we erase the old location of the tail using ' ', then we goto to the head location
                                        // and we print the head, we use the head_shape array which contains 4 ascii values for 4 arrows each going in a different direction
                                        // and we decide wchich one we use according to the last detected move. the snake is composed of the head 'arrow' and body '0'
void draw_snake()
{
    setColor(snake_color);
    gotoxy(tail_X, tail_Y);
    printf(" ");
    gotoxy(snake[0].x, snake[0].y);
    printf("%c", head_shape[last_detected_move]);
    for (i = 1; i < length; i++)
    {
        gotoxy(snake[i].x, snake[i].y);
        printf("O");
    }
    
}
                                        // we call gotoxy using the apple location (x,y) coordinates as parameters and draw it in that location then we print the new score, this function 
                                        // is called only when we leave the menu to continue playing or when the snake eats the apple. hence we display the score using it too
void draw_apple()
{
    setColor(apple_color);
    gotoxy(apple.x, apple.y);
    printf("%c", apple_shape);
    setColor(GREY);
    gotoxy(RIGHT_BARRIER + 5, UPPER_BARRIER + 3);
    printf("score: ");
    setColor(LIGHT_white);
    printf("%d", length - 3);
}
                                        // if the snake hits the barrier or itself, we display 'lost' message using gotoxy, and ask the user to restart or exit the game
                                        // and we set the reset value using display_move function, which if the player choose to restart it returns 1, reset=  1
                                        // otherwise if he chooses to exit it returns 0, reset = 0
void display_lost_screen()
{
    gotoxy(0, 0);
    Sleep(3000);
    system("cls");
    setColor(FOREGROUND_RED);
    gotoxy(77, 10);
    printf(" _________________________________________________________");
    for (i = 0, j = 11; i < 6; i++)
    {
        gotoxy(77, j++);
        printf("|                                                         |");
    }
    gotoxy(77, j);
    printf("|_________________________________________________________|");

    gotoxy(103, 14);
    printf("WASTED");
    gotoxy(0, 0);
    Sleep(2700);
    gotoxy(92, 22);
    system("pause");

    setColor(GREY);
    gotoxy(101, 25);
    printf("RESTART");
    gotoxy(101, 27);
    printf("EXIT");
    reset = display_move(4, 2, 94, 25);
}
                                        // this fucntin is called when the player starts the game, it sets the primary data to default
void set_data()
{
    length = 3, last_detected_move = 2;
    head_X = 70, head_Y = 20, tail_X = 68, tail_Y = 20;
    reset = 2;
    apple = apple_location();
    snake[0].x = head_X;
    snake[0].y = head_Y;
    snake[1].x = head_X - 1;
    snake[1].y = head_Y;
    snake[2].x = head_X - 2;
    snake[2].y = head_Y;
    tail_X = snake[2].x;
    tail_Y = snake[2].y;
}
                                        // this menu is used to change the snake speed by calling the display_move function
int change_difficulty()
{

    system("cls");

    gotoxy(84, 9);
    printf("  ___________________________________________\n");
    for (j = 0; j < 12; j++)
    {
        printf("\t\t\t\t\t\t\t\t\t\t     |                                           |\n");
    }
    gotoxy(84, 21);
    printf(" |___________________________________________|\n");
    gotoxy(100, 11);
    printf("CHOOSE DIFFICULTY");
    gotoxy(96, 13);
    setColor(GREY);
    printf("EASY ");
    gotoxy(96, 15);
    printf("MEDIUM");
    gotoxy(96, 17);
    printf("DEADLY");
    setColor(WHITE);
    return display_move(1, 3, 90, 13);
}
                                        // this menu is used to change the snake color by displaying the available colors from the colors array
                                        // and display_move to pick the choosen color
int change_snake_color()
{

    system("cls");

    gotoxy(84, 1);
    printf("  ___________________________________________\n");
    for (j = 0; j < 34; j++)
    {
        printf("\t\t\t\t\t\t\t\t\t\t     |                                           |\n");
    }
    gotoxy(84, 36);
    printf(" |___________________________________________|\n");
    gotoxy(98, 3);
    printf("CHOOSE SNAKE COLOR");
    for (j = 5, i = 1; j < 35;)
    {
        setColor(i);
        gotoxy(95, j);
        printf("      0000000000%c", head_shape[2]);
        j += 2;
        i++;
    }
    return display_move(2, 15, 89, 5);
}
                                        // this menu is used to change the apple color and shape by displaying all of the available colors from the colors array 
                                        // and the available shapes from the shapes array, and display_move to pick the choosen ones
int change_apple_color_shape()
{

    system("cls");

    gotoxy(84, 1);
    printf("  ___________________________________________\n");
    for (j = 0; j < 34; j++)
    {
        printf("\t\t\t\t\t\t\t\t\t\t     |                                           |\n");
    }
    gotoxy(84, 36);
    printf(" |___________________________________________|\n");
    gotoxy(94, 3);
    printf("CHOOSE APPLE COLOR & SHAPE");
    for (j = 5, i = 1; j < 35;)
    {
        setColor(i);
        gotoxy(95, j);
        printf("  %c", apple_shape);
        setColor(apple_color);
        printf("                  %c", apple_shapes[i - 1]);
        j += 2;
        i++;
    }
    return display_move(3, 15, 89, 5);
}
                                        // here we relocate the snake to it's new coordinates, the switch takes the last_detected_move and decides which direction the snake
                                        // is going and change the snake head location by that, then we chack if the new head location is equal to that of one of the 4 barriers
                                        // 'the snake crashed into a wall', we print 'X' which indicates to a broken head and call the display_lost_screen()
                                        // then we check if the new head location is equal to that of one of the snake bits, 'the snake bited  itself' we print a 'X' and call 
                                        // display_lost_screen
                                        // if none happened, we relocate rest of the snake to a new position 
int update_snake_place()
{

    i = 0;
    switch (last_detected_move)
    {
    case 0:
        head_Y--;
        break;
    case 1:
        head_Y++;
        break;
    case 2:
        head_X++;
        break;
    case 3:
        head_X--;
        break;
    default:

        break;
    }
    if (head_X == LEFT_BARRIER || head_X == RIGHT_BARRIER || head_Y == UPPER_BARRIER || head_Y == LOWER_BARRIER)
                {
                    gotoxy(snake[0].x, snake[0].y);
                    setColor(FOREGROUND_RED);
                    printf("X");
                    display_lost_screen();
                    return 0;
                }
    for (i = 1; i < length - 1; i++)
    {
        if (head_X == snake[i].x && head_Y == snake[i].y)
        {

            draw_snake();
            gotoxy(head_X, head_Y);
            setColor(FOREGROUND_RED);
            printf("X");
            display_lost_screen();
            return 0;
        }
    }
    tail_X = snake[length + -1].x;
    tail_Y = snake[length + -1].y;
    for (i = length + -1; i > 0; i--)
    {
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    snake[0].x = head_X;
    snake[0].y = head_Y;

    return 1;
}
                                        // the settings menu
int settings_menu()
{
    setColor(WHITE);
    while (TRUE)
    {
        system("cls");

        gotoxy(84, 9);
        printf("  ___________________________________________\n");
        for (j = 0; j < 13; j++)
        {
            printf("\t\t\t\t\t\t\t\t\t\t     |                                           |\n");
        }
        gotoxy(84, 22);
        printf(" |___________________________________________|\n");
        gotoxy(103, 11);
        printf("SETTINGS");
        gotoxy(96, 13);
        setColor(GREY);
        printf("CHOOSE DIFFICULTY");
        gotoxy(96, 15);
        printf("CHOOSE SNAKE COLOR");
        gotoxy(96, 17);
        printf("CHOOSE APPLE COLOR & SHAPE");
        gotoxy(96, 19);
        printf("EXIT");
        setColor(WHITE);
        if (display_move(0, 4, 90, 13) == 0)
            break;
    }
}
                                        // this function takes 4 parameters, the window type each window has specific number of options, the number of options of the window
                                        // and the (x,y) coordinates to place the arrow on the very first option on that window, the function relies on gotoxy function
                                        // it prints '>>>>' which represents the  cursor next to the first option. when the user presses any one of the arrow keys, 
                                        // if he goes down it prints '    ' to erase the previous '>>>>' and prints it on the new location, it travels up and down by 2 lines, 
                                        // and has corX, corY which indicates to the location of the arrow, all windown have their arrow going only up and down, excepet for 
                                        // the snake shape and color window where the available colors and shapes are printed in the form of 2 coloms and require the cursor 
                                        // to go right and left as well, if the user presses 'esc' no changes are made and is derected to the previous window
                                        // else if he presses 'enter' it sets the new parameters using a switch
int display_move(int window, int number_of_options, int x, int y)
{
    setColor(WHITE);
    char a;
    int X = x, Y = y;
    int corX = 0, corY = 0;
    gotoxy(X, Y);
    printf(">>>>");
    while (TRUE)
    {
        a = getch();
        switch (a)
        {

        case UP_ARROW: //    up
            if (corY != 0)
            {
                gotoxy(X, Y);
                printf("    ");
                Y -= 2;
                corY--;
                gotoxy(X, Y);
                printf(">>>>");
            }
            break;
        case DOWN_ARROW: //    down
            if (corY != number_of_options - 1)
            {
                gotoxy(X, Y);
                printf("    ");
                Y += 2;
                corY++;
                gotoxy(X, Y);
                printf(">>>>");
            }
            break;
        case RIGHT_ARROW:
            if (corX == 0 && window == 3)
            {
                gotoxy(X, Y);
                printf("    ");
                X += 19;
                corX++;
                gotoxy(X, Y);
                printf(">>>>");
            }
            break;
        case LEFT_ARROW:
            if (corX == 1)
            {
                gotoxy(X, Y);
                printf("    ");
                X -= 19;
                corX--;
                gotoxy(X, Y);
                printf(">>>>");
            }
            break;
        default:
            break;
        }
                                                // the switch takes the window type as a value to decide which change is made
        switch (window)
        {
                                                // 0 is the pause menu window
        case 0:

            if (a == ENTER && Y == 13)
                return change_difficulty();
            if (a == ENTER && Y == 15)
                return change_snake_color();
            if (a == ENTER && Y == 17)
                return change_apple_color_shape();
            if (a == ENTER && Y == 19)
                exit(0);
            if (a == ESCAPE)
            {
                draw_grid();
                draw_apple();
                draw_snake();
                return 0;
            }
            break;
                                                // 1 is the speed menu window
        case 1:
            if (a == ENTER && Y == 13)
                speed = SLOW_SPEED;
            if (a == ENTER && Y == 15)
                speed = MEDIUM_SPEED;
            if (a == ENTER && Y == 17)
                speed = HIGH_SPEED;
            if (a == ESCAPE)
                return 1;

            break;
                                                // 2 is the snake color, the colors are printed in the same order as in the colors array, hence the cursor
                                                // location will work as the index
        case 2:
            if (a == ENTER)
                snake_color = colors[corY];
            if (a == ESCAPE)
                return 1;
            break;
                                                // 3 is the apple shape and color window, if corX=0, then we're changing the color, else corX=1 means we're changing 
                                                // the shape, the colors and shapes are printed in the same order as in the array hence corY will work as the index
        case 3:
            if (a == ENTER && corX == 0)
                apple_color = colors[corY];
            if (a == ENTER && corX == 1)
                apple_shape = apple_shapes[corY];
            if (a == ESCAPE)
                return 1;
            break;
                                                // 4 is the lost window, if the user presses enter where Y=25 which is the first option, which will return 1
                                                // and set reset=1 which indicates that the game will start all over again otherwise, reset=0 which will terminate the programme
        case 4:
            if (a == ENTER && Y == 25)
                return 1;
            if ((a == ENTER && Y == 27) || a == ESCAPE)
                return 0;
            break;

        default:
            break;
        }
        if (a == ENTER)
            return 1;
    }
}
                                        // we get 2 random numbers which are in the range of the of the barriers and set them to a local variable of type
                                        // position called 'apple', and return it, is represents the location for the new apple

position apple_location()
{
    position apple;

    apple.x = (rand() % (RIGHT_BARRIER - LEFT_BARRIER - 1)) + LEFT_BARRIER + 1;
    apple.y = (rand() % (LOWER_BARRIER - UPPER_BARRIER - 1)) + UPPER_BARRIER + 1;
    return apple;
}

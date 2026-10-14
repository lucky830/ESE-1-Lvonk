#include <stdio.h>

int main()
{
    int chances = 4;
    char *player_name = "{name}";

    printf("Welcome to the guessing game!\nThe object of the game is to guess the secret number.\nYou get %d chances\nTo address you personally, I would like to know your name.\n ", chances);
    printf("What is your name: %s\n\n", player_name);
    printf("Great %s, let's get started.\n\n", player_name);
    printf("There is a secret number between 0 and 100, what is your first try:");
}
//George Meisinger
//gem56
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

const int MAX_LENGTH = 100;
//const int SHORT_STR = 10;

// prints a message, reads a line of input, and chops off the newline.
// call it like
//    read_line("Type something in: ", buffer, sizeof(buffer));
void read_line(const char* message, char* buffer, int length)
{
	printf(message);
	fgets(buffer, length, stdin);
	if(strlen(buffer) != 0)
		buffer[strlen(buffer) - 1] = 0;
}

// sees if two strings are the same, ignoring case.
// call it like
//    if(streq_nocase(one, two))
int streq_nocase(const char* a, const char* b)
{
	for(; *a && *b; a++, b++)
		if(tolower(*a) != tolower(*b))
			return 0;

	return *a == 0 && *b == 0;
}

//generate random number from low to high
int random_range(int low, int high)
{
	int r_number = rand() % high + low;
	return r_number;
}

//gets input from the player until they type something valid
int get_player_choice(char* input)
{
	const char rock[10] = "rock";
	const char paper[10] = "paper";
	const char scissors[10] = "scissors";
	if(streq_nocase(rock, input))
		return 0;
	else if(streq_nocase(paper, input))
		return 1;
	else if(streq_nocase(scissors, input))
		return 2;
	else return 3;
}

//determines winner
//returns -1 if comp wins, 0 for tie, 1 if player wins
int get_winner(int p_choice, int c_choice)
{
	//rock = 0
	//paper = 1
	//scissors = 2
	int winner;
	if(p_choice == c_choice)
		winner = 0;
	else if(p_choice == 0)
	{
		if(c_choice == 1)
			winner = -1;
		else if(c_choice == 2)
			winner = 1;
	}
	else if(p_choice == 1)
	{
		if(c_choice == 2)
			winner = -1;
		else if(c_choice == 0)
			winner = 1;
	}
	else if(p_choice == 2)
	{
		if(c_choice == 0)
			winner = -1;
		else if(c_choice == 1)
			winner = 1;
	}
	return winner;
}

//return 1 for yes, 0 for no, -1 for invalid input
int go_again()
{
	char answer[MAX_LENGTH];
	char yes[10] = "yes";
	int go = -1;
	read_line("Play again? ", answer, sizeof(answer));
	if(streq_nocase(answer, yes))
		go = 1;
	else go = 0;
	return go;
}

int main()
{
	//seed random generator
	srand((unsigned int)time(NULL));

	int round = 1;
	int player_score = 0;
	int comp_score = 0;
	char player_input[MAX_LENGTH];
	char comp_text[10] = "no choice";
	int player_choice = 3;
	int comp_choice;
	int winner;
	int running = 1;
	while(running == 1)
	{
		printf("Round %d! ", round);
		//get player choice
		read_line("What's your choice? ", player_input, sizeof(player_input));
		player_choice = get_player_choice(player_input);
		while(player_choice > 2)
		{
			read_line("Huh? What's your choice? ", player_input, sizeof(player_input));
			player_choice = get_player_choice(player_input);
		}
		//get comp choice
		comp_choice = random_range(0, 3);
		if(comp_choice == 0)
			strcpy(comp_text, "rock");
		else if(comp_choice == 1)
			strcpy(comp_text, "paper");
		else if(comp_choice == 2)
			strcpy(comp_text, "scissors");
		//determine winner
		winner = get_winner(player_choice, comp_choice);
		printf("Computer chooses %s. ", comp_text);
		if(winner < 0)
		{
			//comp wins
			comp_score++;
			printf("You lose this round! You: %d Computer: %d\n", player_score, comp_score);
		}
		else if(winner == 0)
		{
			//tie
			printf("It's a tie! You: %d Computer: %d\n", player_score, comp_score);
		}
		else if(winner > 0)
		{
			//player wins
			player_score++;
			printf("You win this round! You: %d Computer: %d\n", player_score, comp_score);
		}
		//check for game over
		if(player_score == 3 || comp_score == 3)
		{
			char game_winner[20] = "empty";
			//figure out who won
			if(player_score == 3)
			{
				strcpy(game_winner,"You ");
			}
			else strcpy(game_winner,"The Computer ");
			//print congratulations/condolences
			printf("%s won the tournament! ", game_winner);
			//and ask to play again
			int again = go_again();
			if(again == 0)
				running = 0;
			else
			{
				round = 0;
				player_score = 0;
				comp_score = 0;
			}
		}
		round++;
	}

	return 0;
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hangman
{
    class Controller
    {
        private KeyPhrase phrase;

        private Player[] players;

        private int playerTurn;
        private int numRounds;
        private int numGuesses;

        //be case insensitive
        public Controller() : this(2, 1)
        {

        }

        public Controller(int numPlayers, int numRounds)
        {
            phrase = null;
            players = new Player[numPlayers];
            createPlayers();
            playerTurn = 0;
            numGuesses = 7; //was 5
            this.numRounds = numRounds;
        }

        private void createPlayers()
        {
            Console.WriteLine("Enter players, 0 for human, 1 for computer");
            for (int i = 0; i < players.Length;)
            {
                Console.Write("Player {0}:", i + 1);
                string input = Console.ReadLine();
                int num = int.Parse(input);
                Player added = null;
                switch(num)
                {
                    case 0: added = new HumanPlayer(); break;
                    case 1: added = new ComputerPlayer(); break;
                    default:
                        Console.WriteLine("Unrecognized Input");
                        break;
                }

                if(added != null)
                    players[i++] = added;
            }
        }

        public void run()
        {
            for (int count = 0; count < numRounds; count++)
                for (int player = 0; player < players.Length; player++)
                    playRound();

            Console.WriteLine("Game Over");
        }

        private void playRound()
        {
            bool validInput;
            do{
                Console.Write("Player {0}, enter phrase: ", playerTurn+1);
                string input = players[playerTurn].getPhrase().ToLower();
                validInput = true;
                
                //string[] words = input.Split(default(string[]), StringSplitOptions.RemoveEmptyEntries);
                foreach (string s in input.Split())
                {
                    if (!WordDictionary.isWord(s))
                    {
                        Console.WriteLine("{0} is not a real word", s);
                        validInput = false;
                        break;
                    }
                }

                phrase = new KeyPhrase(input);
            }
            while(!validInput);

            //the following player makes the guess
            playerTurn = (playerTurn + 1) % players.Length;
            Player player = players[playerTurn];
            player.signalStart();

            int guesses = 0;
            HashSet<int> guessedChars = new HashSet<int>();

            while(guesses < numGuesses)
            {
                Console.WriteLine("Word: {0}", phrase.getHidden());
                Console.WriteLine("Player {0}, make a guess, {1} / {2} guesses", playerTurn + 1, guesses, numGuesses);
                char guess = Char.ToLower(player.getGuess(phrase.getHidden()));

                Console.WriteLine("Player {0} guessed: {1}", playerTurn + 1, guess);

                if (!Char.IsLetter(guess))
                    Console.WriteLine("Guess is not a letter");
                else if (guessedChars.Contains(guess))
                    Console.WriteLine("You already guessed that");
                else if (!phrase.makeGuess(guess))
                {
                    guesses++;  //incorrect
                    player.guessResponse(false);
                }
                else
                {
                    player.guessResponse(true);
                    if (phrase.isGuessed())
                    {
                        //check for game winning
                        Console.WriteLine("Phrase guessed: {0}", phrase.getHidden());
                        break;
                    }
                }

                guessedChars.Add(guess);
            }

            if(guesses < numGuesses)
            {
                Console.WriteLine("Player {0} wins.", playerTurn + 1);
            }
            else
            {
                Console.WriteLine("Player {0} loses.", playerTurn + 1);
                Console.WriteLine("The phrase was: {0}", phrase.ToString());
            }
        }
    }
}

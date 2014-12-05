using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hangman
{
    class HumanPlayer : Player
    {
        public string name { get; set; }
        public char getGuess(string clue)
        {
            do
            {
                Console.Write("Enter guess:");
                string input = Console.ReadLine();

                if (input.Length == 1)
                {
                    if (Char.IsLetter(input[0]))
                        return input[0];
                    else
                        Console.WriteLine("Guess is not a letter");
                }
                else
                    Console.WriteLine("Guess only 1 character");
            }
            while (true);
        }

        public string getPhrase()
        {
            string temp = Console.ReadLine();
            return temp;
        }

        public void guessResponse(bool b)
        {

        }

        public void signalStart()
        {

        }
    }
}

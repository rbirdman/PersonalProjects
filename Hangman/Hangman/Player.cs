using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hangman
{
    interface Player
    {
        string name {get; set; }
        char getGuess(string clue);
        string getPhrase();

        void signalStart();
        void guessResponse(bool correctGuess);
    }
}

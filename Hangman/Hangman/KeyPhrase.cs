using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hangman
{
    class KeyPhrase
    {
        private string phrase;
        private string hiddenPhrase;
        public KeyPhrase(string phrase)
        {
            this.phrase = phrase;
            hiddenPhrase = "";

            for(int i = 0; i < phrase.Length; i++)
            {
                char c = phrase[i];
                if (Char.IsLetter(c))
                    c = '*';
                hiddenPhrase += c;
            }
        }

        public bool makeGuess(char c)
        {
            bool containsLetter = false;
            StringBuilder builder = new StringBuilder(hiddenPhrase);
            for (int i = 0; i < phrase.Length; i++)
            {
                char curr = phrase[i];
                if (c == curr)
                {
                    containsLetter = true;
                    builder[i] = c;
                }
            }

            hiddenPhrase = builder.ToString();

            return containsLetter;
        }

        public bool isGuessed()
        {
            return !hiddenPhrase.Contains('*');
            //for (int i = 0; i < hiddenPhrase.Length; i++)
            //    if (hiddenPhrase[i] == '*')
            //        return false;
            //    return true;
        }

        public bool alreadyGuessed(char c)
        {
            foreach(char curr in hiddenPhrase)
                if (curr == c)
                    return true;

            return false;
        }

        public string getHidden()
        {
            return hiddenPhrase;
        }

        override public string ToString()
        {
            return phrase;
        }
    }

    //only reveals letters that have been guessed
    //unguessed letters contain '*'
    class HiddenKeyPhrase : KeyPhrase
    {
        public HiddenKeyPhrase() : base("")
        {

        }
    }
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Hangman
{
    class ComputerPlayer : Player
    {
        HashSet<string> possibleWords;
        char previousGuess;
        bool guessWasCorrect;
        HashSet<char> guesses;
        double[] stats;

        public ComputerPlayer()
        {
            signalStart();
        }

        public string name { get; set; }
        public char getGuess(string clue)
        {
            if (possibleWords == null)
                getPossibleWords(clue);
            else if(guessWasCorrect)
                matchWords(clue);

            return previousGuess = smartGuessChar(clue);
            //return calculateGuess(clue);
        }

        public string getPhrase()
        {
            //pick a random word between 5 - MAX_LENGTH (WordDictionary.longestWordLength())
            Random rand = new Random();
            HashSet<string> words = null;

            do{
                int length = rand.Next(WordDictionary.longestWordLength());
                //length = Math.Max(length, 5);
                length = 5;
                words = WordDictionary.getWords(length);
            }
            while(words.Count == 0);

            return words.ElementAt(rand.Next(words.Count));
        }

        public void guessResponse(bool b)
        {
            if (b == false)
            {
                filterWords(previousGuess);
            }
            guessWasCorrect = b;
        }

        public void signalStart()
        {
            possibleWords = null;
            previousGuess = '\0';
            guessWasCorrect = false;
            guesses = new HashSet<char>();
            stats = new double[26];
        }

        private void getPossibleWords(string clue)
        {
            possibleWords = new HashSet<string>();
            foreach(string clueWord in clue.Split())
                foreach(string dictWord in WordDictionary.getWords(clueWord.Length))
                    possibleWords.Add(dictWord);
        }

        private void filterWords(char c)
        {
            HashSet<string> filterWords = new HashSet<string>();
            foreach(string word in possibleWords)
                if (word.Contains(c))
                    filterWords.Add(word);

            foreach (string filterWord in filterWords)
                possibleWords.Remove(filterWord);
        }

        private void matchWords(string clue)
        {
            HashSet<string> filterWords = new HashSet<string>();

            foreach(string word in possibleWords)
                if(!word.matchesClue(clue))
                    filterWords.Add(word);

            foreach(string filterWord in filterWords)
                possibleWords.Remove(filterWord);
        }

        //guesses the most common letter with possible guesses
        private char calculateGuess(string clue)
        {
            //be smarter with vowels? if no alpha chars in clue
            //guess a vowel?
            int[] charCount = new int[26];
            
            foreach(char c in guesses)
                charCount[(int)c - 'a'] = int.MinValue; //do not reuse

            int maxCount = 0;
            char guessChar = 'e';

            foreach(string word in possibleWords)
            {
                foreach (char c in word)
                {
                    int index = c - 'a';
                    charCount[index]++;
                    if(charCount[index] > maxCount)
                    {
                        guessChar = c;
                        maxCount = charCount[index];
                    }
                }
            }

            guesses.Add(guessChar);

            return guessChar;
        }

        //Smarter algorithm
        //instead of basing guess out of most common
        //letter, base it off of most probable letter

        //ie. *a*** *allo*een
        //          100% chance of h and w in halloween
        //          (ie. one word left over in possible words of that length)

        //another example:

        // *at  (only to show how measurement works)
        // 25% chance 'b' (examples are not exhaustive)
        // 25% chance 'c'
        // 25% chance 'r'
        // 25% chance 'm'

        //optimization, make possible words a Dictionary<int,HashSet<string>>
        //where int is the size of each string in hashset
        private char smartGuessChar(string clue)
        {
            if(guessWasCorrect)
                calcStats();
            
            //zero out anything already guessed
            foreach (char c in guesses)
                stats[c - 'a'] = -1;

            char guessChar = '\0';
            double max = -1;

            //what to do in case of tie?
            for(int i = 0; i < stats.Length; i++)
            {
                if(max < stats[i])
                {
                    max = stats[i];
                    guessChar = (char) (i + 'a');
                }
            }

            guesses.Add(guessChar);
            return guessChar;
        }

        private void calcStats()
        {
            //int[,] charCount = new int[WordDictionary.longestWordLength(),26]; //code was ugly
            Dictionary<int, int[]> charCount = new Dictionary<int, int[]>();

            foreach (string word in possibleWords)
            {
                if (!charCount.ContainsKey(word.Length))
                    charCount.Add(word.Length, new int[26]);
                int[] sumArray;
                charCount.TryGetValue(word.Length, out sumArray);
                foreach (char c in word)
                    sumArray[c - 'a']++;
                //charCount[word.Length, c - 'a']++;
            }
            stats = new double[26];

            foreach (KeyValuePair<int, int[]> pair in charCount)
            {
                int[] row = pair.Value;
                int sum = 0;
                foreach (int num in row)
                    sum += num;
                for (int i = 0; i < row.Length; i++)
                    stats[i] += ((double)row[i]) / sum;
            }
        }
    }

    //class extensions must be defined in a static class with a static method
    public static class StringExtend
    {
        public static bool matchesClue(this string word, string clue)
        {
            foreach(string clueWord in clue.Split())
            {
                if (word.Length != clueWord.Length)
                    continue;
               
                bool matchesClueWord = true;

                for (int i = 0; i < clueWord.Length; i++)
                {
                    if(clueWord[i] != '*' && clueWord[i] != word[i])
                    {
                        matchesClueWord = false;
                        break;
                    }
                }
                if (matchesClueWord)
                    return true;
            }

            return false;
        }
    }

     
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace Hangman
{
    
    class WordDictionary
    {
        //public const WordDictionary dictionary = new WordDictionary("dictionary.txt");
        private static WordDictionary dictionary = new WordDictionary("dictionary.txt");
        
        public static int longestWordLength()
        {
            return dictionary.dictionaries.Length;
        }

        //indexed by word size
        HashSet<string>[] dictionaries;

        private WordDictionary(string filename)
        {
            //dictionaries = null;
            //current dictionary.txt, longest word is 28 letters long
            resize(28);
            loadDictionary(filename);

        }

        private void resize(int size) 
        {
            HashSet<string>[] temp = new HashSet<string>[size];

            for (int i = 0; i < size; i++)
            {
                if (dictionaries == null || dictionaries.Length <= i)
                    temp[i] = new HashSet<string>();
                else
                    temp[i] = dictionaries[i];
            }

            dictionaries = temp;
        }

        private void loadDictionary(string filename)
        {
            StreamReader file = new StreamReader(filename);
            string input = "";

            while (input != null)
            {
                input = file.ReadLine();
                if (input != null)
                {
                    if (input.Length > dictionaries.Length)
                        resize(input.Length);

                    dictionaries[input.Length - 1].Add(input.ToLower());
                }
            }
        }

        public static bool isWord(string word)
        {
            HashSet<string>[] dictionaries = dictionary.dictionaries;
            if(word.Length - 1 < dictionaries.Length)
                return dictionaries[word.Length - 1].Contains(word);

            return false;
        }

        public static HashSet<string> getWords(int wordLength)
        {
            return dictionary.dictionaries[wordLength - 1];
        }
    }

    
}

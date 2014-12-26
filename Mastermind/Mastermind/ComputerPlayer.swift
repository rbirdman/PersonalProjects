//
//  ComputerPlayer.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

class ComputerPlayer: Player {
	var guesses:[GameBoard.ColorSequence] = []
	var results:[(Int,Int)] = []
	var previousGuess = GameBoard.ColorSequence()
	var permutations:[[GameBoard.Piece]] = []
	var mostTotalCorrect = 0
	
	init() {
		
	}
	
	func getRandSequence(length:Int) -> GameBoard.ColorSequence {
		var sequence = GameBoard.ColorSequence()
		
		for _ in 0..<length {
			var rand = Int(arc4random()) % GameBoard.Piece.maxValue()
			var piece = GameBoard.Piece(rawValue: rand)
			sequence.sequence.append(piece!)
		}
		
		return sequence
	}
	
	func getSecretSequence(sequenceLength:Int) -> GameBoard.ColorSequence {
		return getRandSequence(sequenceLength)
	}
	
	func getGuess(sequenceLength:Int) -> GameBoard.ColorSequence {
		if(guesses.count == 0) {
			previousGuess = getRandSequence(sequenceLength)
		}
		else {
			previousGuess = createGuess(sequenceLength)
		}
		
		return previousGuess
	}
	
	func guessResult(correctPlacement:Int, incorrectPlacement:Int) {
		results.append((correctPlacement, incorrectPlacement))
		guesses.append(previousGuess)
		
	}
	
	func allPermutations(data:[GameBoard.Piece], setSize:Int) -> [[GameBoard.Piece]] {
		var temp:[[GameBoard.Piece]] = []
		
		permutationHelper(data, setSize: setSize, index: 0, collection: [], results: &temp)
		
		return temp
	}
	
	func permutationHelper(data:[GameBoard.Piece], setSize:Int, index:Int,
		collection:[GameBoard.Piece], inout results:[[GameBoard.Piece]]) {
			if collection.count == setSize {
				results.append(collection)
			}
			else if collection.count < setSize && index < data.count {
				var nextCollect = collection
				
				for currIndex in index..<data.count {
					nextCollect.append(data[currIndex])
					permutationHelper(data, setSize: setSize, index: currIndex+1, collection: nextCollect, results: &results)
					nextCollect.removeLast() //shouldn't be needed, but might be
				}
				
			}
	}
	
	func possibleGuess(guess:GameBoard.ColorSequence) -> Bool {
		for index in (0..<guesses.count) {
			var prevGuess = guesses[index]
			var result = results[index]
			
//			| B  B  W  G | • 3 ◦ 0
//			| B  B  B  G | • 3 ◦ 0
//			| B  B  W  G | • 3 ◦ 0
//			| B  B  W  G | • 3 ◦ 0
			if prevGuess == guess {
				return false
			}
			
			var theoreticalResult = GameBoard.ColorSequence.compare(prevGuess, secret: guess)!
			if theoreticalResult.0 != result.0 || theoreticalResult.1 != theoreticalResult.1 {
				return false
			}
		}
		
		return true
	}
	
	func fillUnknownColors(inout guess:GameBoard.ColorSequence) {
		for index in (0..<guess.sequence.count) {
			if guess.sequence[index] == GameBoard.Piece.NoColor {
				guess.sequence[index] = GameBoard.Piece.randomColor()
			}
		}
	}
	
	func createGuess(sequenceLength:Int) -> GameBoard.ColorSequence {
		var guess = GameBoard.ColorSequence()
		
		var previousResults = results.last!
		var totalColorsCorrect = previousResults.0 + previousResults.1
		
		if guesses.count == 1 || mostTotalCorrect <= totalColorsCorrect {
			var tempPermutations = allPermutations(previousGuess.sequence, setSize: totalColorsCorrect)
			
			if mostTotalCorrect == totalColorsCorrect {
//				try to find cross multiply?
				permutations += tempPermutations
			}
			else {
				permutations = tempPermutations
			}
			
			mostTotalCorrect = totalColorsCorrect
		}
		else if mostTotalCorrect == 0 {
			return getRandSequence(sequenceLength)
		}
		
//		shuffle permutations to show 'guessing'
		permutations = permutations.sorted {_, _ in arc4random() % 2 == 0}
		
//		do by index to be able to delete permutations that cannot produce a correct anwer
		for index in 0..<permutations.count {
			var combination = permutations[index]
			
//			fill combination with 'no color', if sequence is chosen, fill 'no colors' with rand color
//			fill in possible colors (2/3 sequence -> 4 sequence)
			for _ in combination.count..<sequenceLength {
				combination.append(GameBoard.Piece.NoColor)
			}
			
			var permutator = AllOrderings(start_sequence: GameBoard.ColorSequence(pieces: combination))
			
			for guessSequence in permutator {
				if possibleGuess(guessSequence) {
					while true {
						//make a deep copy
						var temp = GameBoard.ColorSequence(other: guessSequence)
						
						fillUnknownColors(&temp)
						if possibleGuess(temp) {
							return temp
						}
					}
					
				}
			}
		}
		
		return getRandSequence(sequenceLength)
	}
	
	
}
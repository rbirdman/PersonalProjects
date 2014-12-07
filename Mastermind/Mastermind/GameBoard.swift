//
//  GameBoard.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

extension Array {
	func combine(separator: String) -> String{
		var str : String = ""
		for (idx, item) in enumerate(self) {
			str += "\(item)"
			if idx < self.count-1 {
				str += separator
			}
		}
		return str
	}
}

class GameBoard {
	enum Piece : Printable {
//		Colors to choose from
		case Red, Blue, Green, Yellow, Black, White
		case NoColor
		
		var description: String {
			switch(self) {
			case .Red:   return "R"
			case .Blue:  return "B"
			case .Green: return "G"
			case .Yellow:return "Y"
			case .Black: return "K"
			case .White: return "W"
			case .NoColor:return "*"
			}
		}
	}
	
	class ColorSequence : Printable {
		var sequence: [Piece]
		
		init(pieces:Piece...) {
			sequence = pieces
		}
		
		init(pieces:[Piece]) {
			sequence = pieces
		}
		
//		square bracket operator
		subscript(index:Int) -> Piece {
			return sequence[index]
		}
		
		var description:String {
//			return " ".join(sequence)
//			self defined method 'combine'
			return sequence.combine("  ")
		}
		
//		TODO: requires testing
//		returns (numMatch, colorInWrongLocation)
		class func compare(guess:ColorSequence, secret:ColorSequence) -> (Int, Int)? {
			var exactMatch = 0
			var correctColorWrongLocation = 0
			
			if(guess.sequence.count != secret.sequence.count) {
				return nil
			}
			
			var secretCopy = secret.sequence
			var guessCopy = guess.sequence
//			match exact indexes first
			for (index, value) in enumerate(guess.sequence) {
				if(value == secretCopy[index]) {
					exactMatch++
					secretCopy[index] = Piece.NoColor
					guessCopy[index] = Piece.NoColor
				}
			}
//			match wrong location indexes
			for (secretIndex, value) in enumerate(secretCopy) {
				if(value == Piece.NoColor) {
					continue
				}
				for(guessIndex, guessValue) in enumerate(guessCopy) {
					if(guessValue == value) {
						correctColorWrongLocation++
						secretCopy[secretIndex] = Piece.NoColor
						guessCopy[guessIndex] = Piece.NoColor
						break
					}
				}
			}
			
			return (exactMatch, correctColorWrongLocation)
		}
		
	}
	
	var sequenceLength:Int
	var maxGuesses:Int
	var guesses:[ColorSequence] = []
	var guessResults:[(Int,Int)] = []
	var secret:ColorSequence? = nil
	
	convenience init() {
		self.init(width: 4, guessLimit: 10)
	}
	
	init(width:Int, guessLimit:Int) {
		sequenceLength = width
		maxGuesses = guessLimit
	}
	
	func mayContinue() -> Bool {
		return guesses.count < maxGuesses && secret == guesses.last
	}
	
	func makeGuess(guess:ColorSequence) -> (Int,Int)? {
		if(mayContinue()) {
			var score = ColorSequence.compare(guess, secret: secret!)
			if let realScore = score {
				guesses.append(guess)
				guessResults.append(realScore)
			}
			
			return score
		}
		return nil
	}
	
	func printBoard() {
		var dynamicLine = ""
		var emptyLine = ""
		var emptyGuess = ColorSequence()
		
		for _ in 0..<sequenceLength {
			dynamicLine += "---"
			emptyLine += "   "
			emptyGuess.sequence.append(Piece.NoColor)
		}
		
		println("---\(dynamicLine)---")
		println("|  \(emptyLine  )  |")
		println("|  \(emptyLine  )  |")
		println("---\(dynamicLine)---")
		
		for count in 0..<maxGuesses {
			if(count >= guesses.count) {
				println("  | \(emptyGuess  ) |  ")
			}
			else {
				println("  | \(emptyGuess  ) |  ")
			}
			
		}
		
		
		println("---\(dynamicLine)---")
		println("|  \(emptyLine  )  |")
		println("|   \(emptyGuess )   |")
		println("---\(dynamicLine)---")
		
	}
	
}

func == (left:GameBoard.ColorSequence?, right:GameBoard.ColorSequence?) -> Bool {
	if(left == nil) {
		return right == nil
	}
	else if(right == nil) {
		return left == nil
	}
	else {
//		TODO: requires testing
		return left!.sequence == right!.sequence
	}
}

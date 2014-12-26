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
	enum Piece : Int, Printable {
//		Colors to choose from
		case Red, Blue, Green, Yellow, Black, White
		case NoColor
		
		static func randomColor() -> Piece {
			var rand = Int(arc4random()) % (Piece.maxValue() + 1)
			return Piece(rawValue: rand)!
		}
		
		func nextColor() -> Piece? {
			return Piece(rawValue: self.rawValue + 1)
		}
		
		static func maxValue() -> Int {
			return Piece.White.rawValue
		}
		
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
		
		static func stringToPiece(val:String) -> Piece {
			switch(val) {
				case "R": return Piece.Red
				case "B": return Piece.Blue
				case "G": return Piece.Green
				case "Y": return Piece.Yellow
				case "K": return Piece.Black
				case "W": return Piece.White
				default:  return Piece.NoColor
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
		
		init(other:ColorSequence) {
			sequence = []
			for val in other.sequence {
				sequence.append(val)
			}
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
		self.init(width: 4, guessLimit: 12)
	}
	
	init(width:Int, guessLimit:Int) {
		sequenceLength = width
		maxGuesses = guessLimit
	}
	
	func mayContinue() -> Bool {
		return guesses.count < maxGuesses && !victory()
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
	
	func victory() -> Bool {
		return secret! == guesses.last
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
				var result = guessResults[count]
				println("  | \(guesses[count]  ) | • \(result.0) ◦ \(result.1)")
			}
			
		}
		
		
		println("---\(dynamicLine)---")
		println("|  \(emptyLine  )  |")
		
		if victory() || guesses.count >= maxGuesses {
			println("|   \(secret! )   |")
		}
		else {
			println("|   \(emptyGuess )   |")
		}
		
		println("---\(dynamicLine)---")
		
	}
	
}

func == (left:GameBoard.ColorSequence, right:GameBoard.ColorSequence?) -> Bool {
	if(right == nil) {
		return false
	}
	else {
//		TODO: requires testing
//		doesn't work
//		return left.sequence == right!.sequence
		if left.sequence.count != right!.sequence.count {
			return false
		}
		else {
			for index in (0..<left.sequence.count) {
				if left.sequence[index] != right!.sequence[index] {
					return false
				}
			}
		}
	}
	return true
}

func != (left:GameBoard.ColorSequence, right:GameBoard.ColorSequence?) -> Bool {
	return !(left == right)
}

//
//  HumanPlayer.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

class HumanPlayer: Player {
	init() {
		
	}
	
	func readLine() -> String {
		var fh = NSFileHandle.fileHandleWithStandardInput()
		
		var str = NSString(data: fh.availableData, encoding: NSUTF8StringEncoding)
		var str_val = String(str!)
//		remove endline character
		return str_val.substringToIndex(str_val.endIndex.predecessor())
//		}
	}
	
	func lineToColorSequence(line:String) -> GameBoard.ColorSequence {
		var colors = line.componentsSeparatedByString(" ")
		var sequence = GameBoard.ColorSequence()
		
		for color in colors {
			var piece = GameBoard.Piece.stringToPiece(color)
			sequence.sequence.append(piece)
		}
		return sequence
	}
	
	func getSecretSequence() -> GameBoard.ColorSequence {
		print("What is the secret sequence? ")
		var input = readLine()
		return lineToColorSequence(input)
	}
	
	func getGuess() -> GameBoard.ColorSequence {
		print("What is your guess? ")
		var input = readLine()
		return lineToColorSequence(input)		
//		return GameBoard.ColorSequence(pieces:GameBoard.Piece.Blue, GameBoard.Piece.Red, GameBoard.Piece.Green, GameBoard.Piece.Yellow)
	}
	
	func guessResult(correctPlacement:Int, incorrectPlacement:Int) {
		
	}
}
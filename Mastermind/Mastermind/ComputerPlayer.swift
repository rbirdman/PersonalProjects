//
//  ComputerPlayer.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

class ComputerPlayer: Player {
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
		return GameBoard.ColorSequence()
	}
	
	func guessResult(correctPlacement:Int, incorrectPlacement:Int) {
		
	}
}
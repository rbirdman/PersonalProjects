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
	
	func getSecretSequence() -> GameBoard.ColorSequence {
		return GameBoard.ColorSequence(pieces:GameBoard.Piece.Red, GameBoard.Piece.Blue, GameBoard.Piece.Green, GameBoard.Piece.Yellow)
	}
	
	func getGuess() -> GameBoard.ColorSequence {
//		return GameBoard.ColorSequence()
//		return GameBoard.ColorSequence(pieces:GameBoard.Piece.Blue, GameBoard.Piece.Red, GameBoard.Piece.Green, GameBoard.Piece.Yellow)
		return GameBoard.ColorSequence(pieces:GameBoard.Piece.Black, GameBoard.Piece.Red, GameBoard.Piece.Green, GameBoard.Piece.Yellow)
	}
	
	func guessResult(correctPlacement:Int, incorrectPlacement:Int) {
		
	}
}
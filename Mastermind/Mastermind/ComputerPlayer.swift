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
	
	func getSecretSequence() -> GameBoard.ColorSequence {
		return GameBoard.ColorSequence()
	}
	
	func getGuess() -> GameBoard.ColorSequence {
		return GameBoard.ColorSequence()
	}
	
	func guessResult(correctPlacement:Int, incorrectPlacement:Int) {
		
	}
}
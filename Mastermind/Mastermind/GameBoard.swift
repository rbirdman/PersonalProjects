//
//  GameBoard.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

class GameBoard {
	enum Piece {
//		Colors to choose from
		case Red, Blue, Green, Yellow, Black, White
	}
	
	class ColorSequence {
		var sequence: [Piece] = []
	}
	
	var sequenceLength:Int
	var maxGuesses:Int
	var guesses:[ColorSequence] = []
	
	convenience init() {
		self.init(width: 4, guessLimit: 10)
	}
	
	init(width:Int, guessLimit:Int) {
		sequenceLength = width
		maxGuesses = guessLimit
	}
	
}
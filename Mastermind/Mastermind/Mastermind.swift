//
//  Mastermind.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

class Mastermind {
	var player1:Player
	var player2:Player
	var board:GameBoard? = nil
	
	convenience init() {
//		self.init(p1: PlayerType.Human, p2: PlayerType.Human)
		self.init(p1: PlayerType.Human, p2: PlayerType.Computer)
	}
	
	init(p1:PlayerType, p2:PlayerType) {
		player1 = PlayerFactory.createPlayer(p1)
		player2 = PlayerFactory.createPlayer(p2)
	}
	
	func start() {
		board = GameBoard()
		
		playRound(player1, secretChooser: player2)
		
	}
	
	func playRound(guesser:Player, secretChooser:Player) {
		var boardgame = board!
		var secret = secretChooser.getSecretSequence(boardgame.sequenceLength)
		boardgame.secret = secret
		
		boardgame.printBoard()
		println()
		
		do {
			var guess = guesser.getGuess(boardgame.sequenceLength)
			var result = boardgame.makeGuess(guess)
			if let realResult = result {
				guesser.guessResult(realResult.0, incorrectPlacement: realResult.1)
			}
			
			boardgame.printBoard()
			println()
		} while (boardgame.mayContinue())
	}
}
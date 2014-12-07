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
		self.init(p1: PlayerType.Human, p2: PlayerType.Human)
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
		var secret = secretChooser.getGuess()
		boardgame.secret = secret
		
		do {
			boardgame.printBoard()
			
			break
		} while (boardgame.mayContinue())
	}
}
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
	
	convenience init() {
		self.init(p1: PlayerType.Human, p2: PlayerType.Human)
	}
	
	init(p1:PlayerType, p2:PlayerType) {
		player1 = PlayerFactory.createPlayer(p1)
		player2 = PlayerFactory.createPlayer(p2)
	}
}
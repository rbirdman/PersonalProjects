//
//  Player.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

protocol Player {
	
}

enum PlayerType: Int {
	case Human, Computer
//	HumanGroup, ComputerGroup // could be fun :)
	
	func description() -> String {
		switch self {
		case .Human:
			return "Human"
		case .Computer:
			return "Computer"
		default:
			return String(self.rawValue)
		}
	}
	
	static func maxValue() -> Int {
		return PlayerType.Computer.rawValue
	}
}

class PlayerFactory {
	class func createPlayer(type:PlayerType) -> Player {
		switch type {
		case .Human:
			return HumanPlayer()
		case .Computer:
			return ComputerPlayer()
		}
	}
}
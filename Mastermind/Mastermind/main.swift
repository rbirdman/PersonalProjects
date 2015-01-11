//
//  main.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/7/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

//func sequenceFunc(value:Int, inout arr:[Int]) {
//	if value < 5 {
//		arr.append(value)
//		sequenceFunc(value + 1, &arr)
//	}
//}
//
//var test:[Int] = []
//println(test)
//sequenceFunc(0, &test)
//println(test)

//var colors = [GameBoard.Piece.Black, GameBoard.Piece.Blue, GameBoard.Piece.Yellow,
//			  GameBoard.Piece.Red, GameBoard.Piece.Green, GameBoard.Piece.White]
//
//var all_possible_combinations: [[GameBoard.Piece]] = []
//
//// 6^4 or colors.count ^ sequence length
//for first in 0..<colors.count {
//	for second in 0..<colors.count {
//		for third in 0..<colors.count {
//			for fourth in 0..<colors.count {
//				var sequence = [colors[first],colors[second],colors[third],colors[fourth]]
//				all_possible_combinations.append(sequence)
//			}
//		}
//	}
//}
//
////println("Size: \(all_possible_combinations.count)")
//for combination in all_possible_combinations {
//	println(combination)
//}
//
//println("Testing generator")
//
//var sequence_gen = AllCombinations(sequenceLength: 4) // tested on 2
//var count = 0
//
//for sequence in sequence_gen {
//	println(sequence)
//	count++
//}
//
//println("Total: \(count)")

// Test Permutations
//var array = [1,2,3,4]
//var nextArray:[Int]? = nil
//var ordering = AllOrderings(start_sequence: GameBoard.ColorSequence())
//var count = 1
//
//do {
//	println("\(count). \(array)")
//	nextArray = ordering.nextSequence(array)
//	
//	if nextArray == nil {
//		break
//	}
//	else {
//		array = nextArray!
//	}
//	count++
//} while true

var game = Mastermind()
game.start()

//
//  Combination.swift
//  Mastermind
//
//  Created by Ryan Bird on 12/10/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import Foundation

// generates all possible combinations of colors based on a specific length
class AllCombinations : SequenceType {
	var sequence:[GameBoard.Piece]
	var sequences:[[GameBoard.Piece]]? = nil
	var index = 0
	
	init(sequenceLength:Int) {
		sequence = Array(count:sequenceLength, repeatedValue: GameBoard.Piece(rawValue: 0)!)
	}
	
	func remove() {
//		remove by index when appropriate
//		self.sequences!.removeAtIndex(i)
//		remove last when appropriate
		self.sequences!.removeAtIndex(self.index)
		self.index--
		
	}
	
	func generate() -> GeneratorOf<[GameBoard.Piece]> {
		self.index = 0
		
		if self.sequences != nil {
			return GeneratorOf<[GameBoard.Piece]> {
				return self.sequences![self.index++]
			}
		}
		return GeneratorOf<[GameBoard.Piece]> {
			if self.sequences == nil {
				self.sequences = [self.sequence]
				self.index++
				return self.sequence
			}
			
			
			var temp = Array(count:4, repeatedValue: 0)
			
			var lastIndex:Int = Int(self.sequence.count - 1)
			temp[0] = lastIndex
//			increment color by one
			self.sequence[lastIndex] = self.sequence[lastIndex].nextColor()!
			
			for index in reverse(0..<self.sequence.count) {
				if self.sequence[index].rawValue > GameBoard.Piece.maxValue() {
					self.sequence[index] = GameBoard.Piece(rawValue: 0)!
					if index == 0 {
//						overflow
						return nil
					}
					else {
						self.sequence[index - 1] = self.sequence[index - 1].nextColor()!
					}
				}
			}
			
			self.index++
			self.sequences!.append(self.sequence)
			return self.sequence
		}
	}
}

// generates all possible combinations based on a given sequence
class AllOrderings : SequenceType {
	var sequence:GameBoard.ColorSequence
	var ordering:[Int]?
	
//	TODO: make sequence array of any_type
	init(start_sequence:GameBoard.ColorSequence) {
		sequence = start_sequence
		ordering = []
		for value in (0..<sequence.sequence.count) {
			ordering!.append(value)
		}
	}
	
	func generate() -> GeneratorOf<GameBoard.ColorSequence> {
		return GeneratorOf<GameBoard.ColorSequence> {
			if let order = self.ordering {
				var colorSequence = GameBoard.ColorSequence()
				
				for index in order {
					colorSequence.sequence.append(self.sequence[index])
				}
				
				self.ordering = self.nextSequence(order)
				
				return colorSequence
			}
			
			return nil
//			return GameBoard.ColorSequence()
		}
	}
	
//	takes in sequences of numbers: [ 1 2 3 4 ]
//	returns the next sequence for permutation: [ 1 2 4 3 ]
//	last permutation is: [4,3,2,1]
//	http://en.wikipedia.org/wiki/Permutation#Algorithms_to_generate_permutations
//	Generation in lexicographic order
	func nextSequence(numberSequence:[Int]) -> [Int]? {
		var k_val:Int? = nil
		for k in reverse(0..<numberSequence.count - 1) {
			if numberSequence[k] < numberSequence[k+1] {
				k_val = k
				break
			}
		}
		
		if let k = k_val {
//			make copy of array, inout can make this more efficient
			var newSequence = numberSequence
			
			for i in reverse(0..<numberSequence.count) {
				if numberSequence[k] < numberSequence[i] {
					swap(&newSequence[k], &newSequence[i])
					break
				}
			}
			
			var lower = k + 1
			var upper = newSequence.count - 1
			
			while lower < upper {
				swap(&newSequence[lower], &newSequence[upper])
				lower++
				upper--
			}
			return newSequence
		}
		
		
		return nil
	}
}
//
//  ViewController.swift
//  15 Puzzle UI
//
//  Created by Ryan Bird on 12/29/14.
//  Copyright (c) 2014 Ryan Bird. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
	
	@IBOutlet weak var gameView: UIView!
	@IBOutlet weak var tableView: UIView!
	@IBOutlet var tiles: [UIView]!
	
	weak var selectedTile:UIView? = nil
	
	@IBAction func drag(sender: UIPanGestureRecognizer) {
		var point = sender.locationInView(self.gameView)
		
		switch sender.state {
		case UIGestureRecognizerState.Began:
			println("Began")
			fallthrough
		case UIGestureRecognizerState.Changed:
			println("Changed")
		case UIGestureRecognizerState.Ended:
			println("Ended")
		default:
			println("done")
		}
		
		if(sender.state == UIGestureRecognizerState.Began) {
			
		}
//		find tile that is being moved
	}
	
	override func viewDidLoad() {
		super.viewDidLoad()
		// Do any additional setup after loading the view, typically from a nib.
		
//		for value in 1...tiles.count {
//			
//		}
		
//		println("viewDidLoad()")
//		println("\(tiles.count) tiles")
//		for (index, tile) in enumerate(tiles) {
////			var image = UIImage()
//			var text = UITextField()
//			text.text = String(index + 1)
//			
//			println(text.text!)
//			
//			tile.addSubview(text)
//			tile.needsUpdateConstraints()
//		}
		
	}

	override func didReceiveMemoryWarning() {
		super.didReceiveMemoryWarning()
		// Dispose of any resources that can be recreated.
	}


}


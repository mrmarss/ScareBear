//
//  ViewController.swift
//  GrabMeaJSON
//
//  Created by Brian Tan on 10/31/15.
//  Copyright © 2015 Brian Tan. All rights reserved.
//

import UIKit
import Foundation

var isCaring = true
var isPlaying = false

class ViewController: UIViewController, UITableViewDataSource, UITableViewDelegate
{
   //Class Fields
   @IBOutlet weak var soundClipTableView: UITableView!
   
   var soundClipsScare = ["I hate you!", "Boo", "Praise Satan!"]
   var soundClipsCare = ["I love you!", "You are my best friend!", "Lets play!"]
   
   let soundClipCellIdentifier = "SoundClipCell"

   //On Load
   override func viewDidLoad()
   {
   
      super.viewDidLoad()
      // Do any additional setup after loading the view, typically from a nib.
      
      //Make the view controller the delegate and datasource for our soundclipTableView
      
      soundClipTableView.delegate = self
      soundClipTableView.dataSource = self
      
      //let postEndpoint: String = "http://jsonplaceholder.typicode.com/posts/1"
      //An issue with getting access to this particular web page, the sample one works fine!
      let postEndpoint: String = "http://localhost:8888/getClips"
      let urlRequest = NSURLRequest(URL: NSURL(string: postEndpoint)!)
      let config = NSURLSessionConfiguration.defaultSessionConfiguration()
      let session = NSURLSession(configuration: config);
      
      let task = session.dataTaskWithRequest(urlRequest, completionHandler: {(data, response, error)
      in
         guard let responseData = data
         else
         {
            print("Error: did not receive data")
            return
         }
         guard error == nil
         else
         {
            print("error calling GET on /posts/1")
            //It is defaulted to get, maybe when i change to scarebear, say error in getting mp3 file
            print(error)
            return
         }
         //parse the result as JSON
         let post: NSDictionary
         do
         {
            post = try NSJSONSerialization.JSONObjectWithData(responseData, options: []) as! NSDictionary
         }
         catch
         {
            print("error trying to convert data to JSON")
            return
         }
         //if gotten to this point, we have the data, lets prin it out to prove we have access
         //print("The post is: " + post.description)

         if let mp3Clip = post["clips"] as? [String]
         {
            print("The clip is: " + mp3Clip[0])
         }
 
      })
         
      task.resume()

   }

   override func didReceiveMemoryWarning()
   {
      super.didReceiveMemoryWarning()
      // Dispose of any resources that can be recreated.
   }


//********************** The Emotion Switch *********************
   @IBAction func emotionButton(sender: UIButton)
   {
      //Name of webpage to grab information from
      let postEndpoint: String = "http://localhost:8888/getClips"
      
      // SCARE STATE
      if(isCaring == true)
      {
         
         
         print("Entered Scare Mode")
         let image0 = UIImage(named: "button-online-down.png")
         sender.setImage(image0, forState: UIControlState.Normal)
         isCaring = false
         
         //Change COMMAND to CHANGE-STATE
         //Change STATE to SCARE
         
         let config = NSURLSessionConfiguration.defaultSessionConfiguration()
         let session = NSURLSession(configuration: config);
         let postsUrlRequest = NSMutableURLRequest(URL: NSURL(string: postEndpoint)!)
         postsUrlRequest.HTTPMethod = "POST"
         
         let newPost: NSDictionary = ["command": "change-state", "state": "scare"];
         do
         {
            let jsonPost = try NSJSONSerialization.dataWithJSONObject(newPost, options: [])
            postsUrlRequest.HTTPBody = jsonPost
         }
         catch
         {
            print("Error: cannot create JSON from post")
         }
         
         let task = session.dataTaskWithRequest(postsUrlRequest)
         task.resume()
         
      }
      // CARE  STATE
      else
      {
         print("Entered Care Mode")
         let image1 = UIImage(named: "button-online-up.png")
         sender.setImage(image1, forState: UIControlState.Normal)
         isCaring = true;
         
         // Trigger the signal to the RaspberryPi
         let config = NSURLSessionConfiguration.defaultSessionConfiguration()
         let session = NSURLSession(configuration: config);
         let postsUrlRequest = NSMutableURLRequest(URL: NSURL(string: postEndpoint)!)
         postsUrlRequest.HTTPMethod = "POST"
         
         //Change COMMAND to CHANGE-STATE
         //Change STATE to CARE
         
         let newPost: NSDictionary = ["command": "change-state", "state": "care"];
         do
         {
            let jsonPost = try NSJSONSerialization.dataWithJSONObject(newPost, options: [])
            postsUrlRequest.HTTPBody = jsonPost
         }
         catch
         {
            print("Error: cannot create JSON from post")
         }
         
         let task = session.dataTaskWithRequest(postsUrlRequest)
         task.resume()
   
      }
      
      //After the change in state, reload the SoundClip Tableview for alternative options
      self.soundClipTableView.reloadData()
   }
   
//**********************PAUSE PLAY BUTTON**********************//

/*
   @IBAction func pausePlayButton(sender: UIButton)
   {
      
   }
  */ 
   
//*************************UITableViewDataSource Protocols********************

   func numberOfSectionsInTableView(tableView: UITableView) -> Int
   {
      return 1
   }
   
   func tableView(tableView: UITableView, numberOfRowsInSection section: Int) -> Int
   {
      return (isCaring ? soundClipsCare.count : soundClipsScare.count)
   }
   
   func tableView(tableView: UITableView, cellForRowAtIndexPath indexPath: NSIndexPath) -> UITableViewCell
   {
      let cell = tableView.dequeueReusableCellWithIdentifier(soundClipCellIdentifier, forIndexPath: indexPath) as UITableViewCell
 
      let row = indexPath.row
      cell.textLabel?.text = isCaring ? soundClipsCare[row] : soundClipsScare[row]
 
      return cell
   }


/************************UITableViewDelegate Behavior**********************/

   
   func tableView(tableView: UITableView, didSelectRowAtIndexPath indexPath: NSIndexPath)
   {
      tableView.deselectRowAtIndexPath(indexPath, animated: true)
 
      let row = indexPath.row
      
      var say: String = (isCaring ? soundClipsCare[row] : soundClipsScare[row])
      print(say)
   }

}


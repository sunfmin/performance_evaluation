package main

import (
  "launchpad.net/mgo"
  "launchpad.net/mgo/bson"
  "strings"
)

var col *mgo.Collection

func Connect(database string) {
  println("connecting db " + database)
  split := strings.Split(database, "/")
  session, _ := mgo.Dial(split[0])
  col = session.DB(split[1]).C(split[2])
}

func Track(referer string, remoteAddr string, path string) {
  // TODO : custom logic dependent cumulator
  col.Insert(bson.M{"referer": referer, "remoteAddr": remoteAddr, "path": path})
}

package main

import (
    "net/http"
    "launchpad.net/mgo"
    "text/template"
  "runtime"
)

type User struct {
    Name string
    Bio string
}

var users *mgo.Collection
var tpl *template.Template

func handler(w http.ResponseWriter, r *http.Request) {
    var result []User;
    users.Find(nil).Limit(100).All(&result);
    tpl.Execute(w, result)
}

func main() {
    runtime.GOMAXPROCS(8);

    session, err := mgo.Dial("localhost")
    if err != nil {
            panic(err)
    }
    defer session.Close()

    // Optional. Switch the session to a monotonic behavior.
    session.SetMode(mgo.Monotonic, true)

    users = session.DB("nodehello").C("users")
    tpl, _ = template.ParseFiles("index.html")

    http.HandleFunc("/", handler)
    http.ListenAndServe(":8080", nil)
}

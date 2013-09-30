package main

import (
  "encoding/json"
  "io/ioutil"
  "net/http"
  "strings"
)

func readConfig() map[string]string {
  bytes, _ := ioutil.ReadFile("config.json")
  var config map[string]string
  json.Unmarshal(bytes, &config)
  return config
}

func main() {
  config := readConfig()
  server := config["server"]

  Connect(config["database"])

  println("starting http on " + server)
  http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
    if strings.ToLower(r.Method) != "get" || strings.ToLower(r.Host) != server {
      // println("not match")
      return
    }
    Track(r.Referer(), r.RemoteAddr, r.RequestURI)
  })
  http.ListenAndServe(server, nil)
}

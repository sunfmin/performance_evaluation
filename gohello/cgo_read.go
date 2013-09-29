package main

/*
#define MONGO_HAVE_STDINT 1
#include <stdlib.h>
#include <mongo.h>
#cgo LDFLAGS: -lmongoc
*/
import "C"
import (
	"fmt"
	"net/http"
	"runtime"
	"text/template"
)

var (
	NAMESPACE = C.CString("nodehello.users")
	CSTR_NAME = C.CString("name")
	CSTR_BIO  = C.CString("bio")
)

type User struct {
	Name string
	Bio  string
}

func main() {
	runtime.GOMAXPROCS(8)

	tpl, err := template.ParseFiles("index.html")
	if err != nil {
		panic(err)
	}

	poolSize := 8
	conns := make(chan *C.mongo, poolSize)
	for i := 0; i < poolSize; i++ {
		conn := new(C.mongo)
		C.mongo_init(conn)
		C.mongo_set_op_timeout(conn, 1000)
		status := C.mongo_client(conn, C.CString("localhost"), 27017)
		if status != C.MONGO_OK {
			panic("mongo client")
		}
		conns <- conn
	}

	nResult := 100

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		conn := <-conns
		defer func() {
			conns <- conn
		}()
		result := make([]User, 0, nResult)
		cursor := new(C.mongo_cursor)
		C.mongo_cursor_init(cursor, conn, NAMESPACE)
		for i := 0; i < nResult; i++ {
			if C.mongo_cursor_next(cursor) != C.MONGO_OK {
				break
			}
			iter := new(C.bson_iterator)
			C.bson_find(iter, &cursor.current, CSTR_NAME)
			name := C.GoString(C.bson_iterator_string(iter))
			C.bson_find(iter, &cursor.current, CSTR_BIO)
			bio := C.GoString(C.bson_iterator_string(iter))
			result = append(result, User{
				Name: name,
				Bio:  bio,
			})
		}
		C.mongo_cursor_destroy(cursor)

		tpl.Execute(w, result)
	})
	fmt.Printf("serving\n")
	http.ListenAndServe(":8080", nil)
}

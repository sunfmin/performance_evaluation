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
	"unsafe"
)

var (
	NAMESPACE = C.CString("nodehello.users")
	CSTR_NAME = C.CString("name")
	CSTR_BIO  = C.CString("bio")
)

func main() {
	runtime.GOMAXPROCS(8)

	poolSize := 16
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

	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		conn := <-conns
		defer func() {
			conns <- conn
		}()
		b := new(C.bson)
		C.bson_init(b)
		cName := C.CString("Joe")
		defer C.free(unsafe.Pointer(cName))
		C.bson_append_string(b, CSTR_NAME, cName)
		cBio := C.CString("Bio")
		defer C.free(unsafe.Pointer(cBio))
		C.bson_append_string(b, CSTR_BIO, cBio)
		C.bson_finish(b)
		C.mongo_insert(conn, NAMESPACE, b, nil)
		C.bson_destroy(b)
	})
	fmt.Printf("serving\n")
	http.ListenAndServe(":8080", nil)
}

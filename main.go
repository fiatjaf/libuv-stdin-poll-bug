package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os/exec"
	"strings"
	"time"
)

func main() {
	cmd := exec.Command("target/scala-3.1.3/libuv-stdin-poll-bug-out")
	// cmd := exec.Command("./a.out")

	stdin, err := cmd.StdinPipe()
	if err != nil {
		log.Printf("failed to open stdin: %s", err.Error())
	}
	writer := json.NewEncoder(stdin)

	stdout, err := cmd.StdoutPipe()
	if err != nil {
		log.Printf("failed to open stdout: %s", err.Error())
	}

	go func() {
		reader := bufio.NewReader(stdout)

		for {
			line, err := reader.ReadBytes('\n')
			if err != nil {
				log.Printf("failed to read from stdout: %s", err.Error())
				if err == io.EOF || err == io.ErrUnexpectedEOF {
					return
				}
				time.Sleep(30 * time.Second)
				continue
			}

			fmt.Println("subprocess: ", strings.TrimSpace(string(line)))
		}
	}()

	if err = cmd.Start(); err != nil {
		log.Printf("failed to start: %s", err.Error())
	}

	// fmt.Print("send anything to proceed\n")
	// var v string
	// fmt.Scan(&v)

	fmt.Println("writing 'hello' 4 times to subprocess with intervals of 200ms.")
	time.Sleep(time.Second * 3)

	writer.Encode("hello0")
	time.Sleep(time.Millisecond * 200)
	writer.Encode("hello1")
	time.Sleep(time.Millisecond * 200)
	writer.Encode("hello2")
	time.Sleep(time.Millisecond * 200)
	writer.Encode("hello3")

	fmt.Println("this worked well.")
	time.Sleep(time.Second * 1)
	fmt.Println("now writing 'world' 4 times to subprocess with no interval.")
	time.Sleep(time.Second * 4)

	writer.Encode("world0")
	writer.Encode("world1")

	time.Sleep(time.Second * 1)
	fmt.Println("at this point 4 'world' should have been written, but we only saw the first. now writing the other 5.")
	time.Sleep(time.Second * 5)

	writer.Encode("world2")
	writer.Encode("world3")

	time.Sleep(time.Second * 1)
	fmt.Println("after a while we will write a last 'flush' to subprocess.")
	time.Sleep(4 * time.Second)
	writer.Encode("flush")
	time.Sleep(1 * time.Second)
}

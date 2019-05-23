// Copyright 2019 Tai Groot, SIOTLab. All rights reserved.
// Please see the license file at the root of the project.

// Prepared for the San Jose Water District

package main

import (
	"encoding/json"
	"log"
	"net/http"

	//	"github.com/bvinc/go-sqlite-lite/sqlite3" // database import
	guuid "github.com/google/uuid"
	"github.com/gorilla/mux"
)

// An Event contains all the data correlated with a received packet
type Event struct {
	TS       string `json:"ts"`       // timestamp
	Client   string `json:"client"`   // who sent this event
	Opcode   int    `json:"opcode"`   // what kind of event
	Location string `json:"location"` // where the event was sent from
	Value    string `json:"value"`    // the value of the packet
	ID       string `json:"id"`       // unique event id
}

// An Auth struct is used to parse out the API Key for authentication
type Auth struct {
	Secret string `json:"APIKEY"`
}

var mod, length int
var events []Event
var APIKEY string

// Get all events
func getEvents(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(events)
}

// Add new event
func createEvent(w http.ResponseWriter, r *http.Request) {
	var event Event
	var auth Auth
	_ = json.NewDecoder(r.Body).Decode(&auth)
	if auth.Secret != APIKEY {
		http.Error(w, "Not authorized", 401)
		return
	}
	_ = json.NewDecoder(r.Body).Decode(&event)
	event.ID = guuid.New().String()
	//TODO save new events to database
	if length <= 10 {
		length += 1
		events = append(events, event)
	} else {
		events[mod] = event
		mod = (mod + 1) % 10
	}
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(event)
}

func init() {
	if APIKEY == "" {
		log.Fatalf("Must compile with an API Key!")
	}
}

// Main function
func main() {
	mod = 0
	length = 0

	//TODO database connection & slice init
	// conn, err := sqlite3.Open("mydatabase.db")
	//if err != nil {
	//	os.Exit(1)
	//}
	// defer conn.Close()
	// conn.BusyTimeout(5 * time.Second)

	// Init router
	r := mux.NewRouter()

	// Routes
	r.HandleFunc("/events", getEvents).Methods("GET")
	r.HandleFunc("/events", createEvent).Methods("POST")

	// Start server
	log.Fatal(http.ListenAndServe(":8000", r))
}

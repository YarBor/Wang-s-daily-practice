package main

import (
	"SchoolGuideMap/SchoolGuideMap"
	"fmt"
)

func main() {
	for {
		SchoolGuideMap.LogMune()
		if SchoolGuideMap.ManageLogInChoices() {
			fmt.Printf("Welcome\n")
			break
		} else {
			fmt.Printf("LogIn Fail\n")
		}
	}
	for {
		SchoolGuideMap.MapMune()
		if !SchoolGuideMap.ManageMapMune() {
			fmt.Printf("Input error\n")
		}
	}
}

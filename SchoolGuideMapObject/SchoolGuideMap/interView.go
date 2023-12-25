package SchoolGuideMap

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"os/exec"
	"strconv"
	"strings"
)

func LogMune() {
	fmt.Print("1. LogIn \n" +
		"2. Signup \n")
}
func ManageLogInChoices() bool {
	input := getLine()
	switch input {
	case "1":
		fmt.Print("Enter Username:>")
		username := getLine()
		fmt.Print("Enter Password:>")
		password := getLine()
		ok := checkAccount(username, password)
		if ok {
			mapGuide.MapReload()
		}
		return ok
	case "2":
		fmt.Print("Enter Username:>")
		username := getLine()
		fmt.Print("Enter Password:>")
		password := getLine()
		return signupAccount(username, password)
	case "q":
		os.Exit(0)
		return false
	default:
		return false
	}
}

func MapMune() {
	mune := "1. show Map \n" +
		"2. Change your Position\n" +
		"3. query how to go\n" +
		"4. manage Map Node\n" +
		"5. manage Path Between Nodes\n" +
		"6. where am i \n" +
		"7. get Position Description\n"
	fmt.Printf(mune)
}

func ManageMapMune() bool {
	input := getLine()
	switch input {
	case "1":
		ShowMap()
		break
	case "2":
		fmt.Printf("Where are you\n")
		str, err := mapGuide.getTargetPosition()
		if err != nil {
			fmt.Printf(err.Error())
			return false
		}
		UserPosition = str
		break
	case "3":
		str, err := mapGuide.getTargetPosition()
		if err != nil {
			fmt.Printf(err.Error())
			return false
		}
		paths, err := mapGuide.find(UserPosition, str)
		if err != nil {
			fmt.Printf(err.Error())
			return false
		}
		for p := range paths {
			fmt.Printf("%d step , goto "+paths[p]+"\n", p)
		}
		break
	case "4":
		println("1. Delete")
		println("2. Create")
		println("3. Add Description")
		switch getLine() {
		case "1":
			println("delete Where ?")
			str, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			mapGuide.DeleteNode(str)
			break
		case "2":
			println("Input Name :>")
			mapGuide.AddNode(getLine())
			break
		case "3":
			println("Add Where ?")
			str, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("input Description :> ")
			mapGuide.AddDesc(str, getLine())
			break
		default:
			return false
		}
		break
	case "5":
		println("do What ?")
		println("1. reLength")
		println("2. delete path")
		println("3. add path")
		switch getLine() {
		case "1":
			println("position1 :>")
			position1, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("position2 :>")
			position2, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("Length :>")
			num, err := strconv.Atoi(getLine())
			if err != nil {
				return false
			}
			mapGuide.UpdatePath(position1, position2, num)
			break
		case "2":
			println("position1 :>")
			position1, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("position2 :>")
			position2, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			mapGuide.DeletePath(position2, position1)
			break
		case "3":
			println("position1 :>")
			position1, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("position2 :>")
			position2, err := mapGuide.getTargetPosition()
			if err != nil {
				return false
			}
			println("Length :>")
			num, err := strconv.Atoi(getLine())
			if err != nil {
				return false
			}
			mapGuide.AddPath(position1, position2, num)
			break
		default:
			return false
		}
		break
	case "6":
		fmt.Printf(":> %s\n", UserPosition)
		break
	case "7":
		str, err := mapGuide.getTargetPosition()
		if err != nil {
			fmt.Printf("%s \n", err)
			return false
		}
		res, err := mapGuide.getInfo(str)
		if err != nil {
			fmt.Printf("%s \n", err)
			return false
		}
		println("Position :>" + res.Name)
		for _, i := range res.Paths {
			println("beside " + i)
		}
		println("Description :>" + res.Desc)
		break
	case "q":
		os.Exit(0)
	default:
		return false
	}
	return true
}

func getLine() string {
	reader := bufio.NewReader(os.Stdin)
	input, _ := reader.ReadString('\n')
	return strings.Trim(input, "\n \t")
}

func ShowMap() {
	fmt.Printf(mapGuide.Show() + "\n")
	i := exec.Command("open", "./data/SchoolMap.png")
	err := i.Run()
	if err != nil {
		log.Fatal("show Map failed")
	}
	i.Wait()
}

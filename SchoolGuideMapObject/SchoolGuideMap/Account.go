package SchoolGuideMap

import (
	"crypto/md5"
	"encoding/gob"
	"log"
	"os"
)

var loginData map[interface{}]interface{}

var userId string

func persistentAccout() {
	file, err := os.OpenFile("./data/LogInPersistent.data", os.O_RDWR|os.O_TRUNC, 0777)
	if err != nil {
		panic(err.Error())
	}
	encode := gob.NewEncoder(file)
	err = encode.Encode(loginData)
	if err != nil {
		panic(err.Error())
	}
}
func signupAccount(key string, passward string) bool {
	_, ok := loginData[key]
	if ok {
		return false
	}
	m := md5.New()
	_, err := m.Write([]byte(passward))
	data := string(m.Sum(nil))
	if err != nil {
		panic(err.Error())
	}
	loginData[key] = data
	persistentAccout()
	userId = key
	return true
}
func deleteAccount(key string, passward string) bool {
	if checkAccount(key, passward) {
		delete(loginData, key)
		persistentAccout()
		return true
	} else {
		return false
	}
}
func checkAccount(key string, password string) bool {
	v, ok := loginData[key]
	if !ok {
		return false
	}
	m := md5.New()
	_, err := m.Write([]byte(password))
	if err != nil {
		panic(err.Error())
	}
	if string(m.Sum(nil)) == v {
		userId = key
		return true
	} else {
		return false
	}
}
func init() {
	file, err := os.OpenFile("./data/LogInPersistent.data", os.O_RDWR, 0777)
	defer file.Close()
	if err != nil {
		os.Mkdir("data", 0755)
		file, err = os.OpenFile("./data/LogInPersistent.data", os.O_RDWR, 0777)
		if err != nil {
			panic(err)
		}
	}
	//ioutil.ReadAll(file)
	LogInPersistentDecode := gob.NewDecoder(file)
	loginData = map[interface{}]interface{}{}
	err = LogInPersistentDecode.Decode(&loginData)
	if err != nil {
		log.Printf("Error decode to log info ")
	}
}

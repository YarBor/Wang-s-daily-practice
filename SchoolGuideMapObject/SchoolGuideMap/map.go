package SchoolGuideMap

import (
	"encoding/json"
	"errors"
	"fmt"
	"math"
	"os"
	"sort"
	"strconv"
)

type MapGuide struct {
	M           map[string]map[string]int
	description map[string]string
	nodes       []string
}

var mapGuide MapGuide
var UserPosition string

const (
	NotFound   string = "can't find in mapGuide"
	CantArrive string = "Can't Arrive"
)

type Node struct {
	name   string
	valNow int
	path   []string
}

func NodeSort(nodes []Node) {
	sort.Slice(nodes, func(i, j int) bool {
		return nodes[i].valNow < nodes[j].valNow
	})
}

func (m *MapGuide) getShortestPath(t string, src Node, target []Node) []string {
	cache := []Node{src}
	for {
		if len(target) == 0 {
			return nil
		}
		if src.path == nil {
			return nil
		}
		v := m.M[src.name]
		for t := range target {
			if i, ok := v[target[t].name]; ok {
				tmpV := src.valNow + i
				if tmpV < target[t].valNow {
					target[t].valNow = tmpV
					path := make([]string, len(src.path))
					copy(path, src.path)
					target[t].path = append(path, src.name)
				}
			}
		}
		NodeSort(target)
		src = target[0]
		if src.name == t {
			return append(target[0].path, t)
		} else if src.valNow == math.MaxInt {
			return nil
		}
		cache = append(cache, target[0])
		target = append(target[:0], target[1:]...)
	}
}

func (m *MapGuide) find(src, target string) ([]string, error) {
	err := errors.New(CantArrive)
	_, ok := mapGuide.M[src]
	if !ok {
		panic(NotFound)
	}
	t := []Node{}
	for i := range m.nodes {
		if m.nodes[i] != src {
			t = append(t, Node{name: m.nodes[i], valNow: math.MaxInt})
		}
	}
	val := m.getShortestPath(target, Node{name: src, valNow: 0, path: append(make([]string, 0), src)}, t)
	if val != nil {
		return val[1:], nil
	}
	return nil, err
}

func (m *MapGuide) getInfo(target string) (*struct {
	Name  string
	Paths []string
	Desc  string
}, error) {
	info := struct {
		Name  string
		Paths []string
		Desc  string
	}{}
	if v, ok := m.M[target]; !ok {
		return nil, errors.New(NotFound)
	} else {
		info.Name = target
		info.Desc = m.description[target]
		for k, asdf := range v {
			info.Paths = append(info.Paths, k+" : "+strconv.Itoa(asdf)+" m")
		}
	}
	return &info, nil
}

func init() {
	mapGuide.M = make(map[string]map[string]int)
	mapGuide.nodes = make([]string, 0)
	mapGuide.description = make(map[string]string)
	file, err := os.OpenFile("./data/map.json", os.O_RDONLY, 0)
	if err != nil {
		panic(err.Error())
	}
	defer file.Close()
	tmp := map[string]interface{}{}
	if err := json.NewDecoder(file).Decode(&tmp); err != nil {
		panic(err.Error())
	}
	for k, v := range tmp {
		name := k
		if m, ok := v.(map[string]interface{}); ok {
			if description, ok := m["description"].(string); ok {
				mapGuide.description[name] = description
			}
			if path, ok := m["path"].(map[string]interface{}); ok {
				for k, v := range path {
					if val, ok := v.(float64); ok {
						i, ok := mapGuide.M[name]
						if !ok {
							mapGuide.M[name] = make(map[string]int)
							i = mapGuide.M[name]
						}
						i[k] = int(val)
					}
				}
			}
		}
	}
	for node, _ := range mapGuide.description {
		mapGuide.nodes = append(mapGuide.nodes, node)
	}
	UserPosition = mapGuide.nodes[0]
}
func (m *MapGuide) MapReload() {
	mapGuide.M = make(map[string]map[string]int)
	mapGuide.nodes = make([]string, 0)
	mapGuide.description = make(map[string]string)
	file, err := os.OpenFile("./data/"+userId+"-s-SchoolMap.json", os.O_RDONLY, 0)
	if err != nil {
		panic(err.Error())
	}
	defer file.Close()
	tmp := map[string]interface{}{}
	if err := json.NewDecoder(file).Decode(&tmp); err != nil {
		panic(err.Error())
	}
	for k, v := range tmp {
		name := k
		if m, ok := v.(map[string]interface{}); ok {
			if description, ok := m["description"].(string); ok {
				mapGuide.description[name] = description
			}
			mapGuide.M[name] = make(map[string]int)
			if path, ok := m["path"].(map[string]interface{}); ok {
				for k, v := range path {
					if val, ok := v.(float64); ok {
						mapGuide.M[name][k] = int(val)
					}
				}
			}
		}
	}
	for node, _ := range mapGuide.description {
		mapGuide.nodes = append(mapGuide.nodes, node)
	}
	UserPosition = mapGuide.nodes[0]
}
func persistentMap() {
	file, err := os.OpenFile("./data/"+userId+"-s-SchoolMap.json", os.O_RDWR|os.O_CREATE, 0755)
	if err != nil {
		panic(err.Error())
	}
	tmp := map[string]map[string]interface{}{}
	for k, v := range mapGuide.M {
		_, ok := tmp[k]
		if !ok {
			tmp[k] = make(map[string]interface{})
		}
		tmp[k]["path"] = v
		tmp[k]["description"] = mapGuide.description[k]
	}
	defer file.Close()
	err = json.NewEncoder(file).Encode(tmp)
	if err != nil {
		panic(err.Error())
	}
}

func (m *MapGuide) DeleteNode(nodeName string) {
	for k, v := range m.M {
		if k != nodeName {
			delete(v, nodeName)
		}
	}
	delete(m.M, nodeName)
	delete(m.description, nodeName)
	for i := range m.nodes {
		if m.nodes[i] == nodeName {
			m.nodes = append(m.nodes[:i], m.nodes[i+1:]...)
			break
		}
	}
	persistentMap()
}
func (m *MapGuide) AddNode(name string) {
	m.M[name] = make(map[string]int)
	m.description[name] = ""
	m.nodes = append(m.nodes, name)
	persistentMap()
}
func (m *MapGuide) DeletePath(Src, tar string) {
	if _, ok := m.M[Src]; ok {
		if _, ok := m.M[Src][tar]; ok {
			delete(m.M[Src], tar)
		}
	}
	if _, ok := m.M[tar]; ok {
		if _, ok := m.M[tar][Src]; ok {
			delete(m.M[tar], Src)
		}
	}
	persistentMap()
}

func (m *MapGuide) ShowNodes() {
	for i := range m.nodes {
		fmt.Printf("%d : %s\n", i, m.nodes[i])
	}
}

func (m *MapGuide) UpdatePath(Src, tar string, val int) {
	if _, ok := m.M[Src]; ok {
		if _, ok := m.M[Src][tar]; ok {
			m.M[Src][tar] = val
		}
	}
	if _, ok := m.M[tar]; ok {
		if _, ok := m.M[tar][Src]; ok {
			m.M[tar][Src] = val
		}
	}
	persistentMap()
}

func (m *MapGuide) AddPath(Src, tar string, val int) {
	if _, ok := m.M[Src]; ok {
		m.M[Src][tar] = val
	}
	if _, ok := m.M[tar]; ok {
		m.M[tar][Src] = val
	}
	persistentMap()
}

func (m *MapGuide) AddDesc(Src string, desc string) {
	m.description[Src] = desc
	persistentMap()
}
func (m *MapGuide) Show() string {
	res := ""
	for _, v := range m.nodes {
		data, _ := m.getInfo(v)
		res += fmt.Sprintf("Name:  %s\n", data.Name) +
			fmt.Sprintf("Beside Pos: %v\n", data.Paths) +
			fmt.Sprintf("Desc:  %s\n", data.Desc)
	}
	return res
}
func (m *MapGuide) getTargetPosition() (string, error) {
	for i, tar := range m.nodes {
		fmt.Printf("%d : %s\n", i, tar)
	}
	input := getLine()
	i, err := strconv.Atoi(input)
	if err != nil || i >= len(mapGuide.nodes) {
		return "", errors.New("Wrong Input")
	}
	return m.nodes[i], nil
}

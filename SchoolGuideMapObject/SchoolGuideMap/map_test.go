package SchoolGuideMap

import (
	"fmt"
	"testing"
)

func TestMapGuide_find(t *testing.T) {
	type fields struct {
		M     map[string]map[string]int
		nodes []string
	}
	type args struct {
		src    string
		target string
	}
	tests := []struct {
		name    string
		fields  fields
		args    args
		want    []string
		wantErr bool
	}{
		{name: "1", fields: struct {
			M     map[string]map[string]int
			nodes []string
		}{M: nil, nodes: nil}, args: struct {
			src    string
			target string
		}{src: "A", target: "B"}, want: nil, wantErr: false},
	}
	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			got, err := mapGuide.find(tt.args.src, tt.args.target)
			if err == nil {
				println(got)
			}
		})
	}
}

func TestMapGuide_getInfo(t *testing.T) {
	type fields struct {
		M           map[string]map[string]int
		description map[string]string
		nodes       []string
	}
	type args struct {
		target string
	}
	tests := []struct {
		args args
	}{
		{args: struct{ target string }{target: "图书馆"}},
	}
	for _, tt := range tests {
		t.Run("1", func(t *testing.T) {
			got, err := mapGuide.getInfo(tt.args.target)
			if err != nil {
				panic(err)
			}
			fmt.Printf("%+v", got)
		})
	}
}

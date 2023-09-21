package main

import (
	"os/exec"
	"strings"
)

func LsTree(repository, revision string) {
	args := []string{"ls-tree", "-r", "--name-only", revision}
	cmd := exec.Command("git", args...)
	cmd.Dir = repository

	fileNames, err := cmd.Output()
	if err != nil {
		panic("LsTree: cmd.Output() does not work properly")
	}
	if len(fileNames) == 0 {
		return
	}

	lsTree := strings.TrimSpace(string(fileNames))
	filesLsTree = strings.Split(lsTree, "\n")
}

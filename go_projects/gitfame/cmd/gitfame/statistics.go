package main

import (
	"log"
	"strconv"
	"strings"
)

func parseLine(line string, authorInfo map[string][]string, hashCounts map[string]int,
	currentHash *string, cntAlterations *int, committer bool) {
	lineSplit := strings.Split(line, " ")

	switch {
	case *cntAlterations == 0:
		*currentHash = lineSplit[0]
		var err error
		*cntAlterations, err = strconv.Atoi(lineSplit[len(lineSplit)-1])
		if err != nil {
			log.Printf("Error converting string to integer: %v", err)
			return
		}
		hashCounts[*currentHash] += *cntAlterations
	case lineSplit[0] == "author" && !committer:
		authorInfo[line[7:]] = append(authorInfo[line[7:]], *currentHash)
	case lineSplit[0] == "committer" && committer:
		authorInfo[line[10:]] = append(authorInfo[line[10:]], *currentHash)
	case line[0] == '\t':
		*cntAlterations--
	}
}

func updateAuthorStatistics(authorInfo map[string][]string, hashCounts map[string]int,
	uniqueHashCounts map[string]map[string]struct{}, filesCounts map[string]int, totalAlterationsCounts map[string]int) {
	for author, hashes := range authorInfo {
		filesCounts[author]++
		for _, hash := range hashes {
			if _, ok := uniqueHashCounts[author]; !ok {
				uniqueHashCounts[author] = make(map[string]struct{})
			}
			uniqueHashCounts[author][hash] = struct{}{}
			totalAlterationsCounts[hash] += hashCounts[hash]
		}
	}
}

func InitializeStatsForEmptyLines(lines []string, repository, revision, file string) (map[string]int, map[string][]string) {
	commitCounts := make(map[string]int)
	authorDetails := make(map[string][]string)

	if len(lines) == 0 {
		NoLines(repository, revision, file, &authorDetails)
	}

	return commitCounts, authorDetails
}

func NoLines(repository, revision, file string, authorDetails *map[string][]string) {
	hash, author := Log(repository, revision, file)
	(*authorDetails)[author] = append((*authorDetails)[author], hash)
}

func ParseLines(lines []string, commitCounts map[string]int, authorDetails map[string][]string, isCommitter bool) {
	currentHash := ""
	editCount := 0
	for _, line := range lines {
		parseLine(line, authorDetails, commitCounts, &currentHash, &editCount, isCommitter)
	}
}

func AuthorStatistics(uniqueCommitCounts map[string]map[string]struct{}, totalEditsCounts map[string]int,
	fileStats map[string]int, fileNames []string, repository, revision string, isCommitter bool) {

	for _, file := range fileNames {
		lines := Blame(repository, revision, file)
		commitCounts, authorDetails := InitializeStatsForEmptyLines(lines, repository, revision, file)

		ParseLines(lines, commitCounts, authorDetails, isCommitter)

		updateAuthorStatistics(authorDetails, commitCounts, uniqueCommitCounts, fileStats, totalEditsCounts)
	}

	for i, j := range hashCounts {
		lines := 0
		for k := range j {
			lines += alterationCounts[k]
		}

		authors[i] = &Stats{
			Lines:   lines,
			Commits: len(j),
			Files:   fileCounts[i],
		}
	}
}

#!/bin/bash

# After running the tests, the file "./data/wrong" is created but it
# has to be deleted each time the testFiles is used otherwise one
# test fill fail
if [ $(find ./data/wrong) ]
then
	rm ./data/wrong
fi


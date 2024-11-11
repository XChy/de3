#!/usr/bin/bash
DIR=$(dirname $0)
$DIR/../build/deoptgen -pipeline-type=1 -m 15 $1 -o $2

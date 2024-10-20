#!/bin/bash

mo_build_dir=$1
func_name=$2
output_dir=$3
config_dir=$4
ir=$5

set -e

mkdir -p $output_dir
rm -rf $output_dir/*.ll

cp $ir $output_dir/original.ll


$mo_build_dir/unoptgen $output_dir/original.ll -s $config_dir/seed -p $config_dir/pipeline -o $output_dir/mutated.ll

# Check UB in unoptimized pair
$mo_build_dir/mochecker --only-ub $output_dir/mutated.ll $output_dir/original.ll -func="$func_name" | grep "OK"

opt -O3 -S $output_dir/original.ll -o $output_dir/original_opt.ll &&
opt -O3 -S $output_dir/mutated.ll -o $output_dir/mutated_opt.ll


# Generate diff file for unreduced pair
if [ ! -e $output_dir/diff ]; then
    $mo_build_dir/mo-diff $output_dir/original_opt.ll $output_dir/mutated_opt.ll -o $output_dir/diff -func="$func_name"
fi

original_diff=$(cat $output_dir/diff)
cur_diff=$($mo_build_dir/mo-diff $output_dir/original_opt.ll $output_dir/mutated_opt.ll -func="$func_name")

if [ "$cur_diff" != "$original_diff" ]; then
    exit 1
else
    $mo_build_dir/mochecker $output_dir/mutated_opt.ll $output_dir/original_opt.ll -func="$func_name" | grep "OK"
fi


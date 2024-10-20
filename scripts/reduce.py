#!python3
import argparse
import os
import logging

global args

parser = argparse.ArgumentParser()
parser.add_argument("-d", "--dir", type=str, help="Input dir to be reduce")

if __name__ == "__main__":
    args = parser.parse_args()
    mo_dir = os.path.split(os.path.split(__file__)[0])[0]
    mo_build_dir = os.path.join(mo_dir, "build")
    reduce_oracle = os.path.join(mo_dir, "scripts", "reduce_driver.sh")

    better_dir = os.path.join(args.dir, "better")
    original_ir = os.path.join(args.dir, "original.ll")

    for d in os.listdir(better_dir):
        output_dir = os.path.join(better_dir, d, "reduced")
        with open(os.path.join(better_dir, d, "func_name")) as f:
            func_name = f.read().strip()
            os.system(
                f"llvm-reduce -test={reduce_oracle} --test-arg={mo_build_dir} \
                --test-arg=\"{func_name}\" --test-arg={output_dir} --test-arg={args.dir} \
                {original_ir} --ir-passes=helloworld")

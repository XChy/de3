#!python3
import toml
import os
import sys
import argparse
import tempfile
import logging
import os.path as path

global unoptgen
global moconfirm
root_dir = path.split(path.split(__file__)[0])[0]
unoptgen_build = path.join(root_dir, "build")
unoptgen = path.join(root_dir, "build", "deoptgen")
moclassify = path.join(root_dir, "build", "moclassify")
global args

global index_missed_better
global index_crashed
index_missed_better = 0
index_crashed = 0

logging.basicConfig(
    level=logging.DEBUG,
    filename="run.log",
    filemode="w",
    format="%(asctime)s - %(name)s - %(levelname)-9s - %(filename)-8s : %(lineno)s line - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)

parser = argparse.ArgumentParser()
parser.add_argument("input", type=str, help="Input dir for llvmirs")

parser.add_argument(
    "-o",
    "--output",
    type=str,
    help="dir to output the results",
    required=True,
)

parser.add_argument(
    "-c",
    "--config",
    type=str,
    help="config file",
    required=True,
)


def mine(original_path, config_path):
    working_dir = tempfile.mkdtemp()
    config = toml.load(os.path.expanduser(config_path))

    global index_crashed
    # Mutate IR
    os.system(f"cp {original_path} {working_dir}/original.ll")
    os.system(f"cp {config_path} {working_dir}/config.toml")
    success = os.system(
        "{0} -m {1} {2} -o {3}/mutated.ll -p {3}/pipeline -s {3}/seed -pipeline-type={4}".format(unoptgen, config["mutate"]["max_passes"], original_path, working_dir, config["mutate"]["type"]))
    if success != 0:
        logging.error(f"UnoptGen crashed when mutating {original_path}")
        crash_dir = os.path.join(args.output, "crashed", str(index_crashed))
        os.system(f"mv {working_dir} {crash_dir}")
        logging.error(f"Mutated {original_path} crashed opt")
        index_crashed += 1
        return

    success = os.system(
        "opt -S {0} {1}/mutated.ll -o {1}/mutated_opt.ll".format(config["optimize"]["mutant"], working_dir))
    if success != 0:
        crash_dir = os.path.join(args.output, "crashed", str(index_crashed))
        os.system(f"mv {working_dir} {crash_dir}")
        logging.error(f"Mutated {original_path} crashed opt")
        index_crashed += 1
        return

    success = os.system(
        "opt -S {0} {1}/original.ll -o {1}/original_opt.ll".format(config["optimize"]["original"], working_dir))
    if success != 0:
        crash_dir = os.path.join(args.output, "crashed", str(index_crashed))
        os.system(f"mv {working_dir} {crash_dir}")
        logging.error(f"Original {original_path} crashed opt")
        index_crashed += 1
        return

    success = os.system(
        "{0} {1}/mutated_opt.ll {1}/original_opt.ll --output {1}".format(moclassify, working_dir))

    if success != 0:
        return

    global index_missed_better
    missed_dir = os.path.join(
        args.output, "missed-opt", str(index_missed_better))
    os.system(f"mv {working_dir} {missed_dir}")
    os.system(f"echo {original_path} > {missed_dir}/source_path.txt")

    index_missed_better += 1


if __name__ == "__main__":
    args = parser.parse_args()

    os.makedirs(os.path.join(args.output, "missed-opt"), exist_ok=True)
    os.makedirs(os.path.join(args.output, "crashed"), exist_ok=True)

    os.system(f"cp {args.config} {args.output}/config.toml")

    check_per_file = 3
    total_checks = 0
    cur_checks = 0

    for root, _, filenames in os.walk(args.input):
        for filename in filenames:
            if filename.endswith(".ll"):
                total_checks += check_per_file

    for root, _, filenames in os.walk(args.input):
        for filename in filenames:
            for i in range(check_per_file):
                if filename.endswith(".ll"):
                    mine(os.path.join(root, filename), args.config)
                    cur_checks += 1
                    print(
                        f"\rProgress: [{cur_checks}/{total_checks}],  Found: {index_missed_better}")
                    sys.stdout.flush()

    print("=======Result=======")
    print(f"Found {index_missed_better} candidates")
    print(f"Found {index_crashed} crashed cases")

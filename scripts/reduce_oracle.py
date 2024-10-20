#!python3
import sys
import os
import os.path as path
import filecmp
import toml
import tempfile

root_dir = path.split(path.split(__file__)[0])[0]

if __name__ == "__main__":
    mo_build_dir = path.join(root_dir, "build")
    func_name = sys.argv[2]
    output_dir = sys.argv[3]
    config_dir = sys.argv[4]
    ir = sys.argv[5]

    config = toml.load([path.join(root_dir, "config", "default.toml"),
                       path.join(config_dir, "config.toml")])

    os.system(f"mkdir -p {output_dir}")
    os.system(f"rm -rf {output_dir}/*.ll")
    os.system(f"cp {ir} {output_dir}/original.ll")
    os.system(f"cp {ir} {output_dir}/original.ll")

    os.system("{0}/unoptgen {1}/original.ll -s {2}/seed -p {2}/pipeline -pipeline-type={3} -o {1}/mutated.ll".format(
        mo_build_dir, output_dir, config_dir, config["mutate"]["type"])) and exit(1)
    # Check UB in unoptimized pair
    os.system(f"{mo_build_dir}/mochecker --only-ub {output_dir}/mutated.ll {output_dir}/original.ll -func={func_name} | grep OK") and exit(1)
    os.system("opt {0} -S {1}/original.ll -o {1}/original_opt.ll".format(
        config["optimize"]["original"], output_dir)) and exit(1)
    os.system("opt {0} -S {1}/mutated.ll -o {1}/mutated_opt.ll".format(
        config["optimize"]["mutant"], output_dir)) and exit(1)

    # Generate diff file of unreduced pair
    diff_path = os.path.join(output_dir, "diff")
    cur_diff_path = tempfile.mktemp()

    if not os.path.exists(diff_path):
        os.system(
            f"{mo_build_dir}/mo-diff {output_dir}/original_opt.ll {output_dir}/mutated_opt.ll -o {diff_path} -func={func_name}")

    os.system(
        f"{mo_build_dir}/mo-diff {output_dir}/original_opt.ll {output_dir}/mutated_opt.ll -o {cur_diff_path} -func={func_name}")

    if filecmp.cmp(diff_path, cur_diff_path, False):
        os.system(
            f"{mo_build_dir}/mochecker {output_dir}/mutated_opt.ll {output_dir}/original_opt.ll -func={func_name} | grep OK") and exit(1)
    else:
        exit(1)

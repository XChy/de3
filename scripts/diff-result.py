#!python3
import os
import argparse

global args

parser = argparse.ArgumentParser()
parser.add_argument("result", nargs='?', type=str, help="Original result")
parser.add_argument("-o", "--output", type=str, help="Diff result")


def git_init(output_dir):
    if os.system(f"git init {output_dir}") != 0:
        print("Fail to initialize git repo")
        exit(-1)


def put_file(file_path, dest_dir):
    os.system(f"mkdir -p {dest_dir}")
    os.system(f"cp -f {file_path} {dest_dir}/optimized.ll")


if __name__ == "__main__":
    args = parser.parse_args()
    dir_to_diff = args.result

    git_init(args.output)

    for file_no in os.listdir(dir_to_diff):
        file_result_dir = os.path.join(dir_to_diff, file_no, "better")
        if not os.path.isdir(file_result_dir):
            continue

        for func_no in os.listdir(file_result_dir):
            reduced_function_dir = os.path.join(
                file_result_dir, func_no, "reduced")
            if not os.path.isdir(reduced_function_dir):
                continue

            # Put optimized original into corresponding dir
            dest_dir = os.path.join(
                args.output, f"{file_no}__{func_no}")
            original_path = os.path.join(
                reduced_function_dir, "original_opt.ll")
            print(f"{dest_dir} {original_path}")
            put_file(original_path, dest_dir)

    os.system(f"mkdir -p {args.output}")
    os.system(f"cd {args.output} && git add .")
    os.system(f"cd {args.output} && git commit -m \"original\" ")

    for file_no in os.listdir(dir_to_diff):
        file_result_dir = os.path.join(dir_to_diff, file_no, "better")
        if not os.path.isdir(file_result_dir):
            continue

        for func_no in os.listdir(file_result_dir):
            reduced_function_dir = os.path.join(
                file_result_dir, func_no, "reduced")
            if not os.path.isdir(reduced_function_dir):
                continue

            # Put optimized original into corresponding dir
            dest_dir = os.path.join(
                args.output, f"{file_no}__{func_no}")
            original_path = os.path.join(
                reduced_function_dir, "mutated_opt.ll")
            print(f"{dest_dir} {original_path}")
            put_file(original_path, dest_dir)

    os.system(f"cd {args.output} && git add .")
    os.system(f"cd {args.output} && git commit -m \"mutated\" ")

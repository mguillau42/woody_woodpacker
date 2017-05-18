import sys
import os
import signal
import subprocess
import argparse
import textwrap


# absolute path to your nm_otool project
PROJECT_PATH = '/home/jed/woody_woodpacker'

BIN_PATH = os.path.join(PROJECT_PATH, 'woody_woodpacker')


def execute(cmd):
    proc = subprocess.Popen(cmd, shell=True, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, error = proc.communicate()
    output = output.decode().rstrip()
    error = error.decode().rstrip()
    rc = proc.returncode
    return (output, rc, error)

def tests_woody(tests_array, args):

    errors = 0

    # get the longest test name, useful for padding
    max_len = max(tests_array, key=len)
    max_len = len(max_len)

    for t in tests_array:

        # execute nm and ft_nm
        out , rc , err = execute("{} {}".format(BIN_PATH, t))
        woody_out , woody_rc , woody_err = execute("{}".format("./woody"))

        # compare their return value and output
        if rc == -signal.SIGSEGV:
            result = "\033[91mwoody_woodpacker: SEGMENTATION FAULT\033[0m"
            errors += 1
        elif woody_rc == -signal.SIGSEGV or "Segmentation fault" in woody_err:
            result = "\033[91mwoody: SEGMENTATION FAULT\033[0m"
            errors += 1
        elif rc != 0:
            result = "\033[91mERROR\033[0m: woody_woodpacker returned {}".format(rc)
            errors += 1
        else:
            result = "\033[92mOK\033[0m"

        if args.errors and "OK" in result:
           continue
        print("\t+ {:{length}} {result}".format(t, length=max_len, result=result))

    return errors

def tests_main(args):

    errors = 0
    files_to_test = []

    # fill the files_to_test array according to the given parameters
    for arg in args.files:

        absolute_path = os.path.abspath(arg)
        if os.path.isdir(absolute_path):
            path = absolute_path

            for root, dirs, files in os.walk(path):
                path = root.split(os.sep)

                for file in files:
                    f = os.path.join(root, file)
                    out, rc, _ = execute("file {}".format(f))

                    if args.noignore or not rc and "ELF 64-bit" in out:
                        files_to_test.append(f)

                if not args.recursive:
                    break
        else:
            files_to_test.append(absolute_path)

    print("[+] total amount of files to process: {}".format(len(files_to_test)))

    # # launch tests
    errors += tests_woody(files_to_test, args)

    if errors:
        print("\n[!] total amount of errors: \033[91m{}\033[0m".format(errors))
    else:
        print("\n\[T]/ no error occured, \033[92mgood job!\033[0m".format(errors))

    return errors


if __name__ == '__main__':

    # argument parsing
    parser = argparse.ArgumentParser(formatter_class=argparse.RawDescriptionHelpFormatter, description=textwrap.dedent('''
    Unit testing script made for the 42 school project woody_woodpacker.

    In order to run this script, you must edit the following configuration
    variable located at the beginning of this file:

        PROJECT_PATH -> absolute path to your project

    made by mguillau
    '''))

    parser.add_argument('files', nargs='+', help="files and/or folders to process")
    parser.add_argument('--recursive', '-R',  dest='recursive', action='store_true', help="recursively test binaries encountered for any folder passed as a parameter")
    parser.add_argument('--no-ignore', '-N',  dest='noignore', action='store_true', help="test every file encountered, even non binary ones")
    parser.add_argument('--errors', '-e',  dest='errors', action='store_true', help="only output errors")
    parser.set_defaults(recursive=False)
    parser.set_defaults(noignore=False)
    parser.set_defaults(errors=False)

    if len(sys.argv[1:]) == 0:
        parser.print_usage() # for just the usage line
        parser.exit()

    args = parser.parse_args()

    # check configuration
    if not os.path.exists(BIN_PATH):
        if os.path.exists(PROJECT_PATH):
            print("[!] BIN_PATH not found but PROJECT_PATH exists")
            print("[?] attempting to compile project")
            out, rc, err = execute("make -C {}".format(PROJECT_PATH))
            if rc != 0:
                print(err)
                sys.exit(1)
            else:
                print(out)
        else:
            print("[!] PROJECT_PATH \033[91mnot found\033[0m. It needs to be the absolute_path to your project")
            sys.exit(1)

    sys.exit(tests_main(args))

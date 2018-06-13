# openOCD command:
# 	target remote localhost:3333
#	monitor program xxx.bin
#	monitor reset init
import os
import shutil
import sys
import config

if len(sys.argv) > 1:
	program_name = sys.argv[1]
else:
	program_name = config.program_name_default

bin_file_name = program_name + ".bin"

program_build_path = "./BUILD/" + config.board_name + "/GCC_ARM/"
# The file path in which the openOCD server's start command running

program_bin_file = program_build_path + bin_file_name
debug_bin_file = config.openocd_server_path + bin_file_name

cmd_compile = "mbed compile -m " + config.board_name + " -t GCC_ARM " + config.macro_spec
cmd_gdb_start = "arm-none-eabi-gdb.exe"

def remove_build_files():
	if os.path.exists(program_bin_file):
		os.remove(program_bin_file)

	if os.path.exists(debug_bin_file):
		os.remove(debug_bin_file)

def compile_source_code():
	os.system(cmd_compile)

def copy_bin_file_to_openOCD_server_path():
	if os.path.exists(program_bin_file):
		shutil.copyfile(program_bin_file, debug_bin_file)
		return "true"
	else:
		print("err: Program bin file [%s] doesn't exist"%(program_bin_file))
		return "false"

def gdb_start():
	os.system(cmd_gdb_start)

def main():
	remove_build_files()

	compile_source_code()

	rs = copy_bin_file_to_openOCD_server_path()
	if rs:
		print("Try to connect to openOCD server")
		gdb_start()

main()
from gen_rand_matrix import generate_matrix
import random
import subprocess
import base64
import json

VMS = {
    "high": {
        "name": "vm-high",
        "l-range": 50,
        "h-range": 100
    },
    "med": {
        "name": "vm-med",
        "l-range": 10,
        "h-range": 50
    },
    "low": {
        "name": "vm-low",
        "l-range": 1,
        "h-range": 10
    }
}
MAX_MATRIX_SIZE = 100
NUMBER_OF_JOBS = 15

def assign_job(matrix_size : int, mat1 : str, mat2 : str, vm : str) -> None:
    virsh_command = """virsh -c qemu:///system qemu-agent-command %s '{"execute": "guest-exec", "arguments":{"path" : "/home/aarav/mat_mult", "arg" : [%s, %s, %s], "capture-output":true}}'""" % (vm, matrix_size, mat1, mat2)

    output = subprocess.run(virsh_command, stdout=subprocess.PIPE, text=True, shell = True)
    stdout_string = output.stdout

    out = json.loads(stdout_string)

    virsh_command = """virsh -c qemu:///system qemu-agent-command %s {"execute": "guest-exec-status", "arguments": {"pid": %s}}""" % (vm, str(out["return"]["pid"]))

    output = subprocess.run(virsh_command, stdout=subprocess.PIPE, text=True, shell = True )
    stdout_string = output.stdout

    out = json.loads(stdout_string) 

    encoded_data = str(out["return"]["out-data"])
    decoded_data = base64.b64decode(encoded_data)
    string_output = str(decoded_data, 'UTF-8')

    print(string_output)


for i in range(NUMBER_OF_JOBS):
    matrix_size = random.randint(1, MAX_MATRIX_SIZE)

    mat1 = generate_matrix(matrix_size)
    mat2 = generate_matrix(matrix_size)

    for key in VMS:
        if VMS[key]['l-range'] <= matrix_size <= VMS[key]['h-range']:
            assign_job(matrix_size, mat1, mat2, VMS[key]['name'])
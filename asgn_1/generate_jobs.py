import libvirt

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

def assign_job(mat1, mat2):
    virsh_command = """virsh -c qemu:///system qemu-agent-command newUbuntu '{"execute": "guest-exec", "arguments":{"path" : "/home/vboxuser/exe", "arg" : ["1","1$","2$"], "capture-output":true}}'"""

    output  = subprocess.run(virsh_command, stdout=subprocess.PIPE, text=True, shell = True )
    stdout_string = output.stdout

    out = json.loads(stdout_string)
    # print(out["return"]["pid"])

    virsh_command = """virsh -c qemu:///system qemu-agent-command newUbuntu \'{"execute": "guest-exec-status", "arguments": {"pid":"""+ str(out["return"]["pid"]) +"""}}\'"""

    output  = subprocess.run(virsh_command, stdout=subprocess.PIPE, text=True, shell = True )
    stdout_string = output.stdout

    out = json.loads(stdout_string)
    

    encoded_data = str(out["return"]["out-data"])

    # Decode the Base64 data
    decoded_data = base64.b64decode(encoded_data)

    # Convert the decoded data to a string
    string_output = str(decoded_data)

    # Print the decoded data
    print(string_output)




for i in range(NUMBER_OF_JOBS):
    matrix_size = random.randint(1, MAX_MATRIX_SIZE)


    mat1 = generate_matrix(matrix_size)
    mat2 = generate_matrix(matrix_size)

    for key in VMS:
        if VMS[key]['l-range'] <= matrix_size <= VMS[key]['h-range']:
            assign_job(mat1, mat2)
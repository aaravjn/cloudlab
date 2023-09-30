import libvirt
from gen_rand_matrix import generate_matrix
import random

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
    conn = libvirt.open('qemu:///system')
    if conn is None:
        raise Exception('Failed to open connection to the hypervisor')

    domain_name = 'your_vm_name_or_uuid'
    domain = conn.lookupByName(domain_name)

    cmd = '{"execute":"guest-info"}'
    result = domain.qemuAgentCommand(cmd, 0)
    if result[0] == 0:
        data = result[1]['return']
        print(f"Guest Hostname: {data['host-name']}")
    else:
        print("Failed to get guest info")

    conn.close() 



for i in range(NUMBER_OF_JOBS):
    matrix_size = random.randint(MAX_MATRIX_SIZE)

    mat1 = generate_matrix(matrix_size)
    mat2 = generate_matrix(matrix_size)

    for key in VMS:
        if VMS[key]['l-range'] <= matrix_size <= VMS[key]['h-range']:
            assign_job(mat1, mat2)
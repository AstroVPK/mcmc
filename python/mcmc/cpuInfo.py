import os
import re
import pdb


class cpu(object):
    def __init__(self, logical_id=0, core_id=0, physical_package_id=0, core_siblings_list=None, thread_siblings_list=None):
        self.logical_id = logical_id
        self.logical_id_doc = "Logical CPU ID of this cpu (i.e. the kernel's cpu id)."
        self.core_id = core_id
        self.core_id_doc = "The CPU core ID of this cpu. Typically it is the hardware platform's identifier (rather than the kernel's). The actual value is architecture and platform dependent."
        self.physical_package_id = physical_package_id
        self.physical_package_id_doc = "Physical package id of this cpu. Typically corresponds to a physical socket number, but the actual value is architecture and platform dependent."
        if core_siblings_list:
            self.core_siblings_list = core_siblings_list
        else:
            self.core_siblings_list = list()
        self.core_siblings_list_doc = "Human-readable list of the logical CPU numbers within the same physical_package_id as this cpu."
        if thread_siblings_list:
            self.thread_siblings_list = thread_siblings_list
        else:
            self.thread_siblings_list = list()
        self.thread_siblings_list_doc = "Human-readable list of thsi cpu's hardware threads within the same core as this cpu."

class system(object):
    def __init__(self, system_path='/sys/devices/system/'):
        self.system_path = system_path
        self.cpu_path = os.path.join(self.system_path, 'cpu')
        self.memory_path = os.path.join(self.system_path, 'memory')
        self.cpu = list()
        cpu_dir_list = self._get_cpu_dirs()
        for cpu_dir in cpu_dir_list:
            self.cpu.append(self._get_core_info(cpu_dir))

    def _get_cpu_dirs(self):
        dir_list = os.listdir(self.cpu_path)
        cpu_dir_list = list()
        for dir_name in dir_list:
            matchObj = re.match(r'cpu\d+', dir_name)
            if matchObj:
                cpu_dir_list.append(dir_name)
        return cpu_dir_list

    def _get_core_info(self, cpu_dir):
        logical_id = int(cpu_dir[3:])
        current_cpu_topology_path = os.path.join(self.cpu_path, cpu_dir, 'topology')
        with open(os.path.join(current_cpu_topology_path, 'core_id'), 'rb') as fopen:
            core_id = int(fopen.readline())
        with open(os.path.join(current_cpu_topology_path, 'physical_package_id'), 'rb') as fopen:
            physical_package_id = int(fopen.readline())
        with open(os.path.join(current_cpu_topology_path, 'core_siblings_list'), 'rb') as fopen:
            core_siblings_start_end = [int(x) for x in fopen.readline().rstrip('\n').split('-')]
        core_siblings_list = [i for i in xrange(core_siblings_start_end[0], core_siblings_start_end[-1] + 1)]
        with open(os.path.join(current_cpu_topology_path, 'thread_siblings_list'), 'rb') as fopen:
            thread_siblings_list = [int(x) for x in fopen.readline().rstrip('\n').split(',')]
        current_cpu = cpu(logical_id, core_id, physical_package_id, core_siblings_list, thread_siblings_list)
        return current_cpu


    def add_cpu(cpu_obj):
        self.cpu.append(cpu)

if __name__ == "__main__":
    my_system = system()
    pdb.set_trace()
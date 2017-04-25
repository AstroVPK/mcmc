import os
import re
import pdb


class cpu(object):
    def __init__(self, cpu, package=0, core_siblings_list=None, thread_siblings_list=None):
        self.core_id = core_id
        self.physical_package_id = physical_package_id
        if core_siblings_list:
            self.core_siblings_list = core_siblings_list
        else:
            self.core_siblings_list = list()
        if thread_siblings_list:
            self.thread_siblings_list = thread_siblings_list
        else:
            self.thread_siblings_list = list()

class system(object):
    def __init__(self, system_path='/sys/devices/system/'):
        self.system_path = system_path
        self.cpu_path = os.path.join(self.system_path, 'cpu')
        self.memory_path = os.path.join(self.system_path, 'memory')
        self.cpu = list()
        cpu_dir_list = self._get_cpu_dirs()
        for cpu_dir in cpu_dir_list:
            self._get_core_info(cpu_dir)

    def _get_cpu_dirs(self):
        dir_list = os.listdir(self.cpu_path)
        cpu_dir_list = list()
        for dir_name in dir_list:
            matchObj = re.match(r'cpu\d+', dir_name)
            if matchObj:
                cpu_dir_list.append(dir_name)
        return cpu_dir_list

    def _get_core_info(self, cpu_dir):
        current_cpu_topology_path = os.path.join(self.cpu_path, cpu_dir, 'topology')
        with open(os.path.join(current_cpu_topology_path, 'core_id'), 'rb') as fopen:
            core_id = int(fopen.readline())
        with open(os.path.join(current_cpu_topology_path, 'core_siblings_list'), 'rb') as fopen:
            core_siblings_list = fopen.readline().rstrip('\n').split(',')
        pdb.set_trace()


    def add_cpu(cpu_obj):
        self.cpu.append(cpu)

if __name__ == "__main__":
    my_system = system()
    pdb.set_trace()
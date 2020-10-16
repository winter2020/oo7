#!/usr/bin/env python
import sys
import os
import re

def profile_incidents(inci_path):
    incid_file = open(inci_path, 'r')
    s1 = {}
    s2 = {}
    s3 = {}
    dist_dic = {}
    taint_addrs = {}
    last_addr = ''

    s1_pt_str = r'\(S1\s\(cond\s([a-f0-9]+)\)'
    s2_pt_str = r'\(([0-9]{1,10})\s\(S2(.+)cond\s([a-f0-9]+)\)\s\(load\s([a-f0-9]+)\)'
    s3_pt_str = r'\(([0-9]{1,10})\s\(S3\s\(cond\s([a-f0-9]+)\)\s\(load\s([a-f0-9]+)\)\s\(last\s([a-f0-9]+)\)'
    pc_pt_str = r'0x([A-F0-9]+):'

    s1_pattern = re.compile(s1_pt_str)
    s2_pattern = re.compile(s2_pt_str)
    s3_pattern = re.compile(s3_pt_str)
    pc_pattern = re.compile(pc_pt_str)
    
    for line in incid_file.readlines():
        if '(S1' in line:
            result = re.search(s1_pattern, line)
            if result:
                gs = result.groups()
                if gs[0]:
                    addr = gs[0].strip()
                    s1_str = 'S1#' + addr.strip()
                    if s1_str not in s1.keys():
                        s1[s1_str] = 1
        elif '(S2' in line:
            result = re.search(s2_pattern, line)
            if result:
                gs = result.groups()
                if len(gs) >= 4:
                    distance = gs[0].strip()
                    cond = gs[2].strip()
                    load = gs[3].strip()
                    s2_str = 'S2#' + cond
                    if s2_str not in s2.keys():
                        s2[s2_str] = [load + '#' + distance]
                    else:
                        s2[s2_str].append(load + '#' + distance)
                    if cond + '#' + load not in dist_dic.keys():
                        dist_dic[cond + '#' + load] = distance
        elif '(S3' in line:
            result = re.search(s3_pattern, line)
            if result:
                gs = result.groups()
                if len(gs) >= 4:
                    distance = gs[0].strip()
                    cond = gs[1].strip()
                    load = gs[2].strip()
                    last = gs[3].strip()
                    s3_str = 'S3#' + cond.strip() + '#' + load.strip() + '#' + last.strip() +'#'+distance.strip()
                    if s3_str not in s3.keys():
                        s3[s3_str] = 1
        elif 'pc-changed' in line:
            result = re.search(pc_pattern, line)
            if result:
                gs = result.groups()
                if gs[0]:
                    addr = gs[0].lower()
                    last_addr = addr
        elif 'taint-attached' in line:
            if last_addr:
                if last_addr not in taint_addrs.keys():
                    taint_addrs[last_addr] = 1 
                    last_addr = ''
 
    return s1, s2, s3, taint_addrs

def count_condition_branch(asm):
    asm_file = open(asm)
    count = 0;

    inst_regex = r'([a-f0-9]{2,10}):\s+(([a-f0-9]{2}\s)+)\s+([a-z]{1,10})*'
    pattern = re.compile(inst_regex)

    for line in asm_file.readlines():
        if line is not None:
            result = re.search(pattern, line)
            if result:
                gs = result.groups()
                if gs[3] is not None:
                    inst = gs[3].strip()
                    if inst.startswith('j'):
                        if not inst.startswith('jmp'):
                            count += 1
                    elif inst.startswith('cmov'):
                        count += 1 
                    elif inst.startswith('fcmov'):
                        count += 1

    asm_file.close()
    return count

if  __name__ == '__main__':

    out_file = 'incidents_profile.txt'

    if (len(sys.argv) < 3):
        print 'Please give the incidents file'
        exit(0)
    if not os.path.isfile(sys.argv[1]):
        print 'Error: Incidents file %s does not exist!' %sys.argv[1]
        exit(0)
    if not os.path.isfile(sys.argv[2]):
        print 'Error: Asm file %s does not exist!' %sys.argv[2]
        exit(0)
        
        
    s1, s2, s3, taint = profile_incidents(sys.argv[1])
    branches =  count_condition_branch(sys.argv[2])

    total_distance = 0
    s2_avg_dis = 0
    s3_avg_dis = 0
    for key, value in s2.iteritems():
        #total_distance += int(value.split('#')[1])
        for load_dist in value:
            total_distance += int(load_dist.split('#')[1])

    if len(s2.keys()):
        s2_avg_dis = total_distance / len(s2.keys())

    total_distance = 0
    for key in s3.keys():
        total_distance += int(key.split('#')[4])
    if len(s3.keys()):
        s3_avg_dis = total_distance / len(s3.keys())
        

    out = open(out_file, 'w+')
    temp_str = "@branches: %d\n" %branches
    out.write(temp_str)
    temp_str = "@S1: %d (%.3f%%)\n" %(len(s1.keys()), len(s1.keys()) * 100.0 / branches)
    out.write(temp_str)
    temp_str = "@S2: %d (%.3f%%)\n" %(len(s2.keys()), len(s2.keys()) * 100.0 / branches)
    out.write(temp_str)
    temp_str = "@S2_avg_dis: %d\n" %s2_avg_dis
    out.write(temp_str)
    temp_str = "@S3: %d (%.3f%%)\n" %(len(s3.keys()), len(s3.keys()) * 100.0 / branches)
    out.write(temp_str)
    temp_str = "@S3_avg_dis: %d\n" %s3_avg_dis
    out.write(temp_str)
    out.write('\n')

    for key in s1.keys():
        out.write(key + '\n')
    out.write('\n')

    for key, value in s2.iteritems():
        for load_dist in value:
            out.write(key + '#' + load_dist + '\n')
    out.write('\n')

    for key in s3.keys():
        out.write(key + '\n')
    out.write('\n')

    for key in taint.keys():
        out.write('taint#' + key + '\n')
    
    out.close()
        
    


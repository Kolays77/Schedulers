import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import figure
from collections import defaultdict

def draw(filename, segments, idle_ts, max_ticks):
    num_of_threads = len(segments)
    assert num_of_threads > 0

    y_labels = ["IDLE", "I", "II", "III", "IV", "V", "VI"]
    y_labels = y_labels[:num_of_threads+1]
    colors = {0: 'k', 1: 'r', 2: 'g', 3: 'b', 4: 'y', 5: 'dodgerblue'}
    figure(figsize=(6.5, 3.5), dpi=200)
    
    plt.xlabel('Time')
    plt.ylabel('Tasks')

    plt.yticks([0,1,2,3,4,5], y_labels, fontsize=10)
    plt.xticks(np.arange(max_ticks+5), fontsize=6)
    plt.grid(color='k', linestyle='-', linewidth=0.5)
    
    for y in segments:
        col = colors.get(y, 'b')
        for x in segments[y]:
            plt.plot([x, x+1], [y,y], color=col, marker="o", markersize=5)
    
    plt.savefig(filename + ".png", dpi=(250), bbox_inches='tight')

def parse_segments(filename, max_ticks=999):
    with open(filename) as file:
        segments = defaultdict(list)
        idle_ts = []
        for tick, line in enumerate(file):
            if tick > max_ticks:
                 return segments, idle_ts 
            splitted = line.split()
            segments[int(splitted[-1])].append(tick)
        return segments

def draw_queue_size(filename, max_ticks):
    lst = []
    with open(filename) as file:
        for line in file:
            n = int(line.split()[1])
            lst.append(n)
    plt.plot(lst[:max_ticks])
    plt.savefig(filename + ".png", dpi=(250), bbox_inches='tight')


MAX_TICKS = 40
TRACES = ["out/trace_edf.txt", "out/trace_round_robin.txt", "out/trace_sjf.txt"]
QUEUE_SIZE =  ["out/queue_size_edf.txt", "out/queue_size_round_robin.txt", "out/queue_size_sjf.txt"]

if __name__ == "__main__":
    for trace, queue_size in zip(TRACES, QUEUE_SIZE):
        segments, idle_ts = parse_segments(trace, MAX_TICKS)
        draw(trace, segments, idle_ts, MAX_TICKS)
        draw_queue_size(queue_size, 100)

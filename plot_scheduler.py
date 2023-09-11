import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import figure
from collections import defaultdict
import sys

def draw(segments, idle_ts, max_ticks):
    num_of_threads = len(segments)
    assert num_of_threads > 0
    assert num_of_threads < 5

    y_labels = ["I", "II", "III", "IDLE", "IV", "V"]
    y_labels = y_labels[:num_of_threads+1]
    colors = {1: 'b', 2: 'r', 3: 'g', 4: 'k'}
    figure(figsize=(6.5, 3.5), dpi=200)
    
    plt.xlabel('Time')
    plt.ylabel('Tasks')

    plt.yticks(np.arange(len(y_labels)) + 1, y_labels, fontsize=10)
    plt.xticks(np.arange(max_ticks+5), fontsize=6)
    plt.grid(color='k', linestyle='-', linewidth=0.5)
    for y in segments:
        col = colors.get(y, 'k')
        for x in segments[y]:
            plt.plot([x, x+1], [y,y], color=col, marker="o", markersize=5)

    for idle_t in idle_ts:
        plt.plot([idle_t, idle_t+1], [num_of_threads+1,num_of_threads+1], color='k', marker="o", markersize=5)
        
    plt.show()

def parse_segments(filename, max_ticks=999):
    with open(filename) as file:
        indexes_by_ticks = []
        segments = defaultdict(list)
        idle_ts = []
        for tick, line in enumerate(file):
            if tick > max_ticks:
                 return segments, idle_ts 
            splitted = line.split()
            if (splitted[-1] == "IDLE"):
                idle_ts.append(tick)
            else:
                segments[int(splitted[-1])].append(tick)
        return segments, idle_ts

if __name__ == "__main__":
    assert len(sys.argv) > 1
    max_ticks=20
    segments, idle_ts = parse_segments(sys.argv[1], max_ticks)
    draw(segments, idle_ts, max_ticks)